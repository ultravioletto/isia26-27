// =====================================================================
//  Test Sincronia CoreS3
//  Prova dell'orologio condiviso dell'unisono (PROGETTO.md, par. 8.2-8.3)
//  su tre o più dispositivi con lo stesso sketch.
//
//  Uso:
//    tocco             da libero: entra nella sessione che sente; se non sente
//                      niente ne apre una nuova. Durante il riposo non fa niente
//                      in sessione: accende o spegne il clic
//
//  Una sessione dura 180 s (comando "durata" per cambiarla) e finisce solo
//  allora: ogni dispositivo calcola la fine sul proprio orologio, l'origine
//  manda un FINE come conferma. Poi 30 s di riposo senza sessioni nuove.
//  Non si entra in una sessione a cui mancano meno di 10 s.
//
//  Cosa si vede: un lampo a ogni quarto (giallo sul primo della battuta),
//  un clic e quattro quadrati con la posizione nella battuta. Se gli
//  schermi lampeggiano e i clic suonano insieme, l'orologio funziona.
//  Una ripresa al rallentatore col telefono mostra scarti di pochi ms.
//
//  Come funziona, in breve:
//    - i dispositivi non hanno un orologio comune: ognuno conta il tempo
//      da quando si è acceso, e i conteggi non c'entrano tra loro
//    - una sessione ha un'ORIGINE che ogni 2 s manda in broadcast un SYNC:
//      "sono passati tot ms dall'inizio della sessione"
//    - chi lo riceve ricava quando è iniziata la sessione nel PROPRIO
//      orologio (t0) e da lì calcola da solo in che passo si trova
//    - i SYNC successivi correggono piano la deriva dei quarzi
//    - se l'origine sparisce, il partecipante con l'id più basso prende
//      l'orologio; se ci sono due origini, vince quella con l'id più basso
//
//  Prove da fare:
//    1. ingresso: avvia su A, B e C entrano da soli entro 2 s
//    2. deriva: lascia girare qualche minuto, err resta sotto qualche ms
//    3. continuità: spegni l'origine, dopo 6 s l'id più basso prende
//       l'orologio e la musica non si ferma
//    4. hop: allontana C da A finché non sente più l'origine (hop 1)
//    5. riavvio: riaccendi un dispositivo, rientra da solo
//    6. due origini: avvia due sessioni insieme, vince l'origine con
//       l'id più basso e tutti si uniscono a quella
//    7. durata: tutti escono allo stesso passo, anche chi in quel momento
//       non sente la radio; durante il riposo il tocco non fa niente
//
//  Seriale a 115200: ogni SYNC ricevuto e ogni cambio di ruolo. Se nessuno
//  legge la porta le righe si perdono, ma il loop non si ferma.
//  Comandi da seriale, per i test: stato, avvia, fine, durata S, riposo MS,
//  pausa N, isola N, ignora ID, battiti 0|1, clic 0|1, riavvia (N in ms).
//  "fine" interrompe solo il dispositivo che lo riceve: è uno strumento di
//  test, la specifica non prevede uscite prima della durata.
//  banco.py, nella stessa cartella, li usa per provare tre dispositivi dal PC.
//
//  Il clic parte spento (variabile audio).
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (con M5Unified e M5GFX)
// =====================================================================

#include <WiFi.h>        // per mettere la radio in modalità stazione
#include <esp_now.h>     // ESP-NOW: pacchetti radio senza rete Wi-Fi
#include <esp_wifi.h>    // canale e indirizzo MAC
#include <esp_timer.h>   // orologio in microsecondi a 64 bit
#include <M5CoreS3.h>
#include <math.h>

// ---------------------------------------------------------------- parametri
// Tutti i numeri del comportamento stanno qui (valori dell'Appendice A).
// constexpr: il compilatore li scrive direttamente nel codice, non occupano memoria.
//
// A 90 bpm con griglia 4: un battito dura 666,7 ms, un passo 166,7 ms.
// Il lampo scatta sul battito, cioè ogni 4 passi.
static constexpr uint8_t  CANALE         = 1;      // tutti i dispositivi devono stare sullo stesso canale
static constexpr uint8_t  BPM            = 90;
static constexpr uint8_t  GRIGLIA        = 4;      // passi per battito
static constexpr uint32_t OGNI_SYNC      = 2000;   // ms tra due SYNC dell'origine
static constexpr uint8_t  HOP_MAX        = 2;      // un SYNC si inoltra al massimo due volte
static constexpr uint32_t RITARDO_MIN    = 50, RITARDO_MAX = 300;   // inoltro, ms
static constexpr uint32_t SILENZIO_ELEZIONE = 6000;  // origine muta: si elegge un'altra
static constexpr uint16_t DURATA         = 180;    // secondi: la sessione finisce solo qui
static constexpr uint32_t RIPOSO         = 30000;  // ms dopo la fine: niente sessioni nuove
static constexpr uint32_t RESIDUA_MIN    = 10000;  // ms: se ne mancano meno, l'invito non si accetta
static constexpr double   CORREZIONE_MAX = 10.0;   // ms di correzione per secondo
static constexpr double   SALTO          = 150.0;  // oltre questo scarto si salta subito
                                                   // (a 10 ms/s, 150 ms richiederebbero 15 s)
static constexpr double   LATENZA        = 2.0;    // ms stimati di volo radio; è uguale per
                                                   // tutti, quindi sposta tutti allo stesso modo
static constexpr uint32_t OGNI_ANNUNCIO  = 1000;
static constexpr uint32_t DURATA_LAMPO   = 90;

// ---------------------------------------------------------------- pacchetti (Appendice B)
// packed: senza, il compilatore inserirebbe byte vuoti per allineare i campi e
// la struct non corrisponderebbe più ai byte del protocollo. Con packed la struct
// È il pacchetto: si spedisce così com'è e si rilegge con un cast dei byte ricevuti.
static constexpr uint16_t MAGIC = 0xC7EA;    // pacchetti di altri sistemi sullo stesso canale vengono ignorati
static constexpr uint8_t  VERSIONE = 1;      // e anche quelli di una versione diversa del protocollo
static constexpr uint8_t  SPECIE_TEST = 0;
enum { T_ANNUNCIO = 1, T_SYNC = 3, T_FINE = 5 };

// id e seq identificano un pacchetto in modo unico: quando un SYNC viene inoltrato
// l'intestazione resta quella dell'origine, così chi lo riceve due volte riconosce la copia
struct __attribute__((packed)) Intestazione { uint16_t magic; uint8_t versione, tipo, specie, id; uint16_t seq; };

// ANNUNCIO: "ci sono, e sono in questa sessione". Qui servono solo id e sessione;
// gli altri campi restano a zero ma ci sono, per essere compatibili con la piattaforma
struct __attribute__((packed)) Annuncio {
  Intestazione h; uint8_t stato, bisogni[5], flag, generazione; uint16_t sessione;
};

// SYNC: il cuore del test. trascorso non è un'ora assoluta (i dispositivi non ne
// hanno una comune) ma quanti ms sono passati dall'inizio della sessione,
// nell'orologio di chi lo manda
struct __attribute__((packed)) Sync {
  Intestazione h; uint16_t sessione; uint8_t origine, hop, bpm, griglia, scala, tonica;
  uint32_t trascorso; uint16_t durata;
};

struct __attribute__((packed)) Fine { Intestazione h; uint16_t sessione; uint8_t motivo, riservato; };

// controllo a compilazione: se qualcuno aggiunge un campo per sbaglio, non compila
// invece di trasmettere pacchetti sbagliati
static_assert(sizeof(Annuncio) == 18 && sizeof(Sync) == 22 && sizeof(Fine) == 12, "pacchetti");

// un pacchetto ricevuto, con l'istante d'arrivo in microsecondi
struct Arrivo { uint8_t dati[32]; int len; int64_t t; uint8_t da; };   // da: id ricavato dal MAC di chi ha trasmesso
                                                                       // (diverso da h.id se è un inoltro)

enum Ruolo { LIBERO, ORIGINE, PARTECIPANTE };

const uint8_t BROADCAST[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };   // "a tutti quelli in ascolto"

// ---------------------------------------------------------------- schermo
static constexpr int W = 320, H = 240, BARRA = 40;   // BARRA: altezza della barra informazioni in alto

// colore a 16 bit (5 bit rosso, 6 verde, 5 blu), il formato del display
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), BIANCO = rgb(255, 255, 255);
const uint16_t GRIGIO = rgb(120, 120, 120), SCURO = rgb(40, 40, 40);
const uint16_t CORALLO = rgb(255, 90, 78), VERDE = rgb(46, 194, 126);
const uint16_t AZZURRO = rgb(63, 167, 255), GIALLO = rgb(255, 210, 63);

// ---------------------------------------------------------------- stato
const char* NOMI_RUOLO[] = { "libero", "ORIGINE", "partecipante" };

uint8_t mioMac[6], mioId = 0;   // id 1..250, ricavato dall'ultimo byte del MAC
uint16_t mioSeq = 0;            // contatore dei miei pacchetti

Ruolo ruolo = LIBERO;
uint16_t sessione = 0;          // numero casuale scelto dall'origine
uint16_t sessioneChiusa = 0;    // sessione finita: i suoi SYNC e FINE in ritardo si ignorano
uint8_t origine = 0, bpm = BPM, griglia = GRIGLIA;
uint16_t durata = DURATA;         // durata della sessione corrente, s (arriva col SYNC)
uint16_t durataNuove = DURATA;    // durata delle sessioni che apro io (comando "durata")
uint32_t riposoMs = RIPOSO;       // lunghezza del riposo (comando "riposo")
uint32_t riposoFino = 0;          // fino a quando dura il riposo, millis
bool eraInRiposo = false;

// t0 è LA variabile del test: l'istante in cui è iniziata la sessione, misurato
// nel mio orologio. Ogni dispositivo ha il suo t0, diverso in valore ma che indica
// lo stesso momento reale. Da t0 si ricava il passo corrente senza parlarsi.
double t0 = 0;         // inizio della sessione nel mio orologio, ms
double scarto = 0;     // correzione di t0 ancora da applicare, ms
double ultimoErrore = 0;   // quanto t0 era sbagliato all'ultimo SYNC: è "err" a schermo
int ultimoHop = 0;
Sync ultimoSentito;              // l'ultimo SYNC sentito, di qualsiasi sessione
double tUltimoSentito = -1e9;    // e quando è arrivato, ms
uint32_t tUltimoSync = 0, tInvioSync = 0, tAnnuncio = 0, tUltimoInoltro = 0;
double tCorrezione = 0;          // ultimo istante in cui è stata applicata la correzione

// inoltro: un SYNC ricevuto aspetta qui il suo ritardo casuale prima di ripartire
bool inoltroPendente = false;
Sync daInoltrare;
uint32_t tInoltro = 0;

long ultimoPasso = 0;   // l'ultimo passo già "suonato", per non ripeterlo
bool audio = false;   // clic: spento per ora; il tocco o il comando "clic 1" lo riaccendono
bool lampoAcceso = false, ridisegna = true;
uint32_t tLampo = 0, tInfo = 0;

// vicini, indicizzati direttamente per id (0..255): circa 3 KB, ma la ricerca è
// immediata, senza scorrere una lista
uint32_t tVivo[256];        // quando ho sentito l'ultimo annuncio di quell'id
uint16_t sessVicino[256];   // in che sessione dice di essere
uint16_t ultimoSeq[256];    // ultimo seq visto da quell'id, per scartare i duplicati
uint32_t tVisto[256];       // quando l'ho visto
volatile bool idDoppio = false;   // volatile: lo scrive la callback, lo legge il loop

// per i test: radio spenta fino a un istante, battiti stampati su seriale
uint32_t isolaFino = 0;
uint8_t ignoraId = 0;   // simula un vicino fuori portata: i suoi pacchetti diretti si buttano
bool stampaBattiti = false;
bool isolato() { return (int32_t)(isolaFino - millis()) > 0; }
bool inRiposo() { return (int32_t)(riposoFino - millis()) > 0; }

// Orologio di precisione. esp_timer_get_time conta i microsecondi su 64 bit: non torna
// a zero dopo 49 giorni come millis() né dopo 71 minuti come micros(). In double
// conserva le frazioni di millisecondo, che servono a misurare scarti di 0,1 ms.
// millis() resta solo per i timer grossolani: annunci, silenzi, lampo.
double oraMs() { return esp_timer_get_time() / 1000.0; }

// la seriale non deve mai fermare il loop: con la porta USB aperta ma non letta
// Serial.printf si blocca a buffer pieno, quindi se non c'è posto la riga si perde.
// Per un test perdere un log va bene, fermare l'orologio no.
void stampa(const char* formato, ...) {
  char buf[128];
  va_list ap;
  va_start(ap, formato);
  int n = vsnprintf(buf, sizeof(buf), formato, ap);
  va_end(ap);
  n = min(n, (int)sizeof(buf) - 1);
  if (n > 0 && Serial.availableForWrite() >= n) Serial.write((const uint8_t*)buf, n);
}

// ---------------------------------------------------------------- ricezione
// la callback gira nel task del Wi-Fi: copia il pacchetto in coda con l'istante d'arrivo.
// Gira in un altro thread rispetto al loop, quindi fa il minimo indispensabile e lascia
// tutta la logica al loop. La coda è circolare: testa è dove scrive la callback, fondo
// è dove legge il loop; se testa raggiungerebbe fondo la coda è piena e il pacchetto si perde.
static constexpr int CODA = 16;
Arrivo coda[CODA];
int testa = 0, fondo = 0;
portMUX_TYPE muxCoda = portMUX_INITIALIZER_UNLOCKED;   // il lucchetto condiviso da callback e loop

void ricevi(const esp_now_recv_info_t* info, const uint8_t* dati, int len) {
  // l'istante d'arrivo si prende come PRIMA cosa: è il motivo della precisione sotto il ms.
  // Preso nel loop, ci finirebbe dentro il ritardo del loop (fino a 30 ms se sta disegnando)
  int64_t t = esp_timer_get_time();
  if (len < (int)sizeof(Intestazione) || len > 32) return;
  const Intestazione* h = (const Intestazione*)dati;
  if (h->magic != MAGIC || h->versione != VERSIONE) return;
  // un annuncio col mio id ma da un altro MAC: due dispositivi con lo stesso id
  if (h->tipo == T_ANNUNCIO && h->id == mioId && memcmp(info->src_addr, mioMac, 6) != 0) idDoppio = true;
  portENTER_CRITICAL(&muxCoda);
  int prossima = (testa + 1) % CODA;
  if (prossima != fondo) {
    memcpy(coda[testa].dati, dati, len);
    coda[testa].len = len;
    coda[testa].t = t;
    coda[testa].da = info->src_addr[5] % 250 + 1;
    testa = prossima;
  }
  portEXIT_CRITICAL(&muxCoda);
}

// il lato loop della coda: un pacchetto alla volta, sotto lo stesso lucchetto
bool estrai(Arrivo& a) {
  bool c = false;
  portENTER_CRITICAL(&muxCoda);
  if (fondo != testa) { a = coda[fondo]; fondo = (fondo + 1) % CODA; c = true; }
  portEXIT_CRITICAL(&muxCoda);
  return c;
}

// ---------------------------------------------------------------- invio
// tutto va in broadcast: nessuna connessione, nessuna conferma di ricezione.
// Per questo tutto ciò che conta si ripete (annuncio ogni secondo, SYNC ogni 2 s, FINE tre volte).
void intestazione(Intestazione& h, uint8_t tipo) {
  h.magic = MAGIC; h.versione = VERSIONE; h.tipo = tipo;
  h.specie = SPECIE_TEST; h.id = mioId; h.seq = ++mioSeq;
}

void invia(const void* p, size_t n) {
  if (!isolato()) esp_now_send(BROADCAST, (const uint8_t*)p, n);
}

void inviaAnnuncio() {
  Annuncio a = {};
  intestazione(a.h, T_ANNUNCIO);
  a.flag = ruolo != LIBERO ? 1 : 0;   // bit 0: in unisono
  a.sessione = ruolo != LIBERO ? sessione : 0;
  invia(&a, sizeof(a));
}

void inviaSync() {
  Sync s = {};
  intestazione(s.h, T_SYNC);
  s.sessione = sessione; s.origine = mioId; s.hop = 0;
  s.bpm = bpm; s.griglia = griglia; s.scala = 0; s.tonica = 60;
  s.trascorso = (uint32_t)(oraMs() - t0);   // calcolato nel momento in cui parte
  s.durata = durata;   // con trascorso, dice a tutti quando finisce
  invia(&s, sizeof(s));
}

void inviaFine(uint8_t motivo) {
  Fine f = {};
  intestazione(f.h, T_FINE);
  f.sessione = sessione; f.motivo = motivo;
  // tre volte: il broadcast non ha conferma, e un FINE perso lascerebbe qualcuno a suonare da solo
  for (int i = 0; i < 3; i++) { invia(&f, sizeof(f)); delay(15); }
}

// ---------------------------------------------------------------- sessione
// Tutto il sistema si riduce a questa riga: dal tempo trascorso dall'inizio della
// sessione si ricava il numero del passo. Due dispositivi con lo stesso t0 (ognuno
// nel suo orologio) calcolano lo stesso passo nello stesso istante, senza parlarsi.
// I pacchetti servono solo a mettersi d'accordo su t0.
long passoCorrente() { return (long)floor((oraMs() - t0) * bpm * griglia / 60000.0); }

void cambiaRuolo(Ruolo r) {
  if (r != ruolo) stampa("[%lu] %s -> %s  sessione %u\n", millis(), NOMI_RUOLO[ruolo], NOMI_RUOLO[r], sessione);
  ruolo = r;
  ridisegna = true;
}

// divento origine di una sessione nuova: per me la sessione inizia adesso
void apriSessione() {
  do { sessione = random(1, 65536); } while (sessione == sessioneChiusa);
  origine = mioId; bpm = BPM; griglia = GRIGLIA; durata = durataNuove;
  t0 = oraMs(); scarto = 0; ultimoErrore = 0; ultimoHop = 0;
  ultimoPasso = -1;
  tInvioSync = 0;   // il primo SYNC parte subito e vale da invito
  cambiaRuolo(ORIGINE);
}

// torno libero e non seguo più questa sessione
void chiudi() {
  sessioneChiusa = sessione;
  inoltroPendente = false;
  cambiaRuolo(LIBERO);
}

// la fine vera: la durata è raggiunta (o è arrivato il FINE che lo conferma).
// Dopo, il riposo: per un po' non si apre e non si accetta nessuna sessione
void finisci(const char* perche) {
  stampa("[%lu] fine sessione %u (%s)\n", millis(), sessione, perche);
  if (ruolo == ORIGINE) inviaFine(1);   // conferma, motivo 1: durata raggiunta
  chiudi();
  riposoFino = millis() + riposoMs;
  eraInRiposo = true;
}

// millisecondi che mancano alla fine della sessione, nel mio orologio
double residuoMs() { return durata * 1000.0 - (oraMs() - t0); }

// un id più basso tra i partecipanti vivi della mia sessione?
// Non serve votare: ognuno guarda la propria tabella dei vicini e sa se è il più basso
bool sonoIlPiuBasso() {
  uint32_t ora = millis();
  for (int id = 1; id < mioId; id++)
    if (sessVicino[id] == sessione && ora - tVivo[id] < SILENZIO_ELEZIONE) return false;
  return true;
}

int viciniInSessione() {
  int n = 0;
  uint32_t ora = millis();
  for (int id = 1; id < 256; id++)
    if (id != mioId && tVivo[id] && ora - tVivo[id] < 3000 && sessione && sessVicino[id] == sessione) n++;
  return n;
}

void gestisciSync(const Sync& s, double tArrivo) {
  if (s.hop > HOP_MAX) return;
  // lo ricordo prima di decidere se ignorarlo: così chi è uscito sa comunque che c'è
  // una sessione in corso, e il tocco lo fa rientrare
  ultimoSentito = s; tUltimoSentito = tArrivo;   // anche se lo ignoro: serve a rientrare col tocco
  if (s.sessione == sessioneChiusa) return;

  // da libero non entro durante il riposo, in una sessione senza durata (firmware vecchio)
  // o in una sessione che sta per finire: entrerei per uscire subito
  if (ruolo == LIBERO) {
    if (inRiposo() || s.durata == 0) return;
    if (s.durata * 1000.0 - s.trascorso < RESIDUA_MIN) return;
  }

  // Accetto questo SYNC? La regola "vince l'id più basso" è deterministica: ogni
  // dispositivo la applica da solo e tutti arrivano alla stessa conclusione
  bool accetta;
  if (ruolo == LIBERO)              accetta = true;   // da libero entro in qualsiasi sessione
  else if (s.sessione == sessione)  accetta = ruolo == PARTECIPANTE || s.origine < mioId;   // doppia elezione: cede l'id più alto
  else                              accetta = s.origine < origine;                          // due sessioni: vince l'origine più bassa
  if (!accetta) return;

  // Esempio: l'origine dice "sono passati 5000 ms" e il pacchetto mi è arrivato quando
  // il mio orologio segnava 12000: per me la sessione è iniziata a 7000 (meno il volo radio)
  double stimato = tArrivo - (double)s.trascorso - LATENZA;   // t0 secondo questo SYNC
  bool nuova = ruolo != PARTECIPANTE || s.sessione != sessione;
  if (nuova) {
    // entro: prendo t0 così com'è, e allineo ultimoPasso a adesso,
    // altrimenti "suonerei" tutti i passi dall'inizio della sessione
    sessione = s.sessione;
    t0 = stimato; scarto = 0; ultimoErrore = 0;
    ultimoPasso = passoCorrente();
    cambiaRuolo(PARTECIPANTE);
  } else {
    // sono già dentro: misuro quanto mi ero spostato
    ultimoErrore = stimato - t0;
    if (fabs(ultimoErrore) > SALTO) { t0 = stimato; scarto = 0; ultimoPasso = passoCorrente(); }   // troppo lontano: salto
    else scarto = ultimoErrore;   // applicato piano nel loop; sostituisce la correzione precedente,
                                  // perché l'ultimo SYNC è l'informazione più fresca
  }
  origine = s.origine; bpm = s.bpm; griglia = s.griglia; durata = s.durata;
  ultimoHop = s.hop;
  tUltimoSync = millis();
  stampa("[%lu] sync s=%u o=%u hop=%u err=%+.2f ms\n", millis(), s.sessione, s.origine, s.hop, ultimoErrore);

  // inoltro: una volta ogni 2 s, dopo un ritardo casuale, fino a hop 2.
  // Il ritardo casuale evita che tutti i partecipanti ritrasmettano nello stesso istante
  // e si coprano a vicenda. Così l'orologio arriva anche a chi non sente l'origine.
  if (s.hop < HOP_MAX && !inoltroPendente && millis() - tUltimoInoltro >= OGNI_SYNC * 3 / 4) {   // margine: l'inoltro precedente è partito in ritardo
    daInoltrare = s;
    daInoltrare.hop = s.hop + 1;
    tInoltro = millis() + random(RITARDO_MIN, RITARDO_MAX + 1);
    inoltroPendente = true;
  }
}

// tocco da libero: se una sessione è in corso ci entra subito, usando l'ultimo SYNC
// sentito; altrimenti ne apre una nuova. Il SYNC si rielabora col suo istante d'arrivo
// vero, quindi la stima di t0 resta esatta. Durante il riposo non fa niente
void avvia() {
  if (ruolo != LIBERO) return;
  if (inRiposo()) { stampa("[%lu] riposo: ancora %lu ms\n", millis(), riposoFino - millis()); return; }
  if (oraMs() - tUltimoSentito < OGNI_SYNC + 1000) {
    sessioneChiusa = 0;
    gestisciSync(ultimoSentito, tUltimoSentito);
  }
  if (ruolo == LIBERO) apriSessione();
}

// lo smistatore: scarta ciò che non serve e passa il resto a chi lo gestisce
void gestisciPacchetto(const Arrivo& a) {
  const Intestazione* h = (const Intestazione*)a.dati;
  if (h->id == mioId) return;   // i miei, tornati indietro da un inoltro

  // copie già viste (un riavvio azzera seq: dopo 10 s di silenzio si riparte).
  // Il confronto con segno (int16_t) funziona anche quando seq torna da 65535 a 0
  uint32_t ora = millis();
  if (tVisto[h->id] && ora - tVisto[h->id] < 10000 && (int16_t)(h->seq - ultimoSeq[h->id]) <= 0) return;
  ultimoSeq[h->id] = h->seq;
  tVisto[h->id] = ora;

  double tArrivo = a.t / 1000.0;   // da microsecondi a millisecondi
  switch (h->tipo) {
    case T_ANNUNCIO:   // aggiorna la tabella dei vicini
      if (a.len == sizeof(Annuncio)) {
        const Annuncio* an = (const Annuncio*)a.dati;
        tVivo[h->id] = ora;
        sessVicino[h->id] = an->sessione;
      }
      break;
    case T_SYNC:
      if (a.len == sizeof(Sync)) gestisciSync(*(const Sync*)a.dati, tArrivo);
      break;
    case T_FINE:
      if (a.len == sizeof(Fine)) {
        const Fine* f = (const Fine*)a.dati;
        if (f->sessione == sessioneChiusa) break;
        invia(a.dati, a.len);   // inoltro una volta, i duplicati li scarta chi riceve
        // il FINE parte solo alla scadenza: copre chi ha l'orologio un istante indietro
        if (ruolo != LIBERO && f->sessione == sessione) finisci("FINE ricevuto");
        else sessioneChiusa = f->sessione;   // non ero dentro, ma non devo entrarci più
      }
      break;
  }
}

// ---------------------------------------------------------------- comandi da seriale
// per i test: stato, avvia, fine, durata S, riposo MS, pausa N, isola N, ignora ID,
// battiti 0|1, clic 0|1, riavvia
// fine interrompe la sessione SOLO su questo dispositivo, senza FINE né riposo: serve a
// rimettere a zero i test, la specifica non prevede uscite prima della durata;
// durata imposta i secondi delle sessioni che questo dispositivo apre; riposo i ms del riposo;
// pausa ferma il loop per N ms; isola spegne invio e ricezione per N ms;
// ignora butta i pacchetti trasmessi da quell'id (0 per nessuno), per provare l'inoltro.
// battiti 1 stampa a ogni battito "B sessione battito ritardo", dove ritardo è quanto
// il loop è arrivato dopo l'istante teorico del battito
String riga;

// "isola 10000" -> nome = "isola", n = 10000
void eseguiComando(String c) {
  c.trim();
  int sp = c.indexOf(' ');
  String nome = sp < 0 ? c : c.substring(0, sp);
  long n = sp < 0 ? 0 : c.substring(sp + 1).toInt();
  if (nome == "stato")
    stampa("[%lu] stato %s sess %u orig %u passo %ld err %+.2f scarto %+.2f vicini %d resta %ld riposo %ld\n", millis(),
           NOMI_RUOLO[ruolo], sessione, origine, ruolo != LIBERO ? passoCorrente() : -1L,
           ultimoErrore, scarto, viciniInSessione(), ruolo != LIBERO ? (long)residuoMs() : -1L,
           inRiposo() ? (long)(riposoFino - millis()) : 0L);
  else if (nome == "avvia")   avvia();   // come il tocco
  else if (nome == "fine")    { if (ruolo != LIBERO) chiudi(); riposoFino = millis(); }   // solo test
  else if (nome == "durata")  { if (n > 0 && n < 65536) durataNuove = n; stampa("[%lu] durata %u s\n", millis(), durataNuove); }
  else if (nome == "riposo")  { if (n >= 0) riposoMs = n; stampa("[%lu] riposo %lu ms\n", millis(), riposoMs); }
  else if (nome == "pausa")   { stampa("[%lu] pausa %ld ms\n", millis(), n); delay(n); }
  else if (nome == "isola")   { isolaFino = millis() + n; stampa("[%lu] isolato per %ld ms\n", millis(), n); }
  else if (nome == "ignora")  { ignoraId = n; stampa("[%lu] ignoro l'id %ld\n", millis(), n); }
  else if (nome == "battiti") stampaBattiti = n;
  else if (nome == "clic")    { audio = n; ridisegna = true; }
  else if (nome == "riavvia") ESP.restart();
  else stampa("comando sconosciuto: %s\n", c.c_str());
}

// accumula i caratteri fino all'a capo, senza mai aspettare
void leggiComandi() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') { if (riga.length()) eseguiComando(riga); riga = ""; }
    else if (riga.length() < 40) riga += ch;
  }
}

// ---------------------------------------------------------------- disegno
// Si disegna direttamente sul display, senza tela fuori schermo: copiare una tela
// intera costa circa 30 ms a ogni fotogramma e sposterebbe i lampi. Si ridisegna
// solo quando serve: al lampo, alla fine del lampo, quando cambia qualcosa.

// barra in alto: id, ruolo, sessione, origine; sotto vicini, hop, err, clic
void disegnaInfo() {
  auto& d = CoreS3.Display;
  d.fillRect(0, 0, W, BARRA, SCURO);
  d.setFont(&fonts::DejaVu12);
  d.setTextDatum(textdatum_t::top_left);

  String r1 = "id " + String(mioId) + "   " + NOMI_RUOLO[ruolo];
  if (ruolo != LIBERO) r1 += "   sess " + String(sessione) + "   orig " + String(origine);
  d.setTextColor(ruolo == ORIGINE ? GIALLO : BIANCO, SCURO);   // giallo se sono io a tenere l'orologio
  d.drawString(r1, 8, 5);

  String r2;
  if (ruolo == LIBERO) {
    r2 = "vicini " + String(viciniInSessione()) + "   mac " + String(mioMac[4], HEX) + ":" + String(mioMac[5], HEX);
    if (inRiposo()) r2 += "   riposo " + String((riposoFino - millis() + 999) / 1000) + " s";
  } else {
    // il conto alla rovescia: ogni dispositivo lo calcola sul proprio orologio
    long resta = (long)ceil(max(0.0, residuoMs()) / 1000.0);
    r2 = "resta " + String(resta / 60) + ":" + (resta % 60 < 10 ? "0" : "") + String(resta % 60) + "   vicini " + String(viciniInSessione());
    if (ruolo == PARTECIPANTE) r2 += "   hop " + String(ultimoHop) + "   err " + String(ultimoErrore, 1) + " ms";
    r2 += audio ? "   clic on" : "   clic off";
  }
  d.setTextColor(GRIGIO, SCURO);
  d.drawString(r2, 8, 22);
  if (idDoppio) { d.setTextColor(CORALLO, SCURO); d.setTextDatum(textdatum_t::top_right); d.drawString("ID DOPPIO", W - 8, 5); }
}

// sotto la barra: "tocca per iniziare" da libero, i quattro quadrati in sessione
void disegnaCorpo(long battito) {
  auto& d = CoreS3.Display;
  d.fillRect(0, BARRA, W, H - BARRA, NERO);
  if (ruolo == LIBERO) {
    d.setFont(&fonts::DejaVu18);
    d.setTextDatum(textdatum_t::middle_center);
    d.setTextColor(GRIGIO, NERO);
    d.drawString(inRiposo() ? "riposo" : "tocca per iniziare", W / 2, (H + BARRA) / 2);
    return;
  }
  // quattro quadrati: la posizione nella battuta
  int lato = 48, gap = 16, x0 = (W - 4 * lato - 3 * gap) / 2, y = (H + BARRA) / 2 - lato / 2;
  int corrente = battito >= 0 ? battito % 4 : -1;
  for (int i = 0; i < 4; i++) {
    int x = x0 + i * (lato + gap);
    if (i == corrente) d.fillRect(x, y, lato, lato, i == 0 ? GIALLO : AZZURRO);
    else d.drawRect(x, y, lato, lato, GRIGIO);
  }
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);
  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(NERO);
  CoreS3.Speaker.begin();
  CoreS3.Speaker.setVolume(120);

  // radio in modalità stazione, senza collegarsi a nessuna rete, fissata sul canale 1
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(CANALE, WIFI_SECOND_CHAN_NONE);
  esp_wifi_get_mac(WIFI_IF_STA, mioMac);
  mioId = mioMac[5] % 250 + 1;
  randomSeed(esp_random());   // numeri casuali veri dalla radio: sessioni e ritardi diversi su ogni dispositivo

  // ESP-NOW: avvio, callback di ricezione, e il broadcast come unico destinatario
  bool ok = esp_now_init() == ESP_OK;
  if (ok) {
    esp_now_register_recv_cb(ricevi);
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, BROADCAST, 6);
    peer.channel = 0;   // il canale corrente
    peer.ifidx = WIFI_IF_STA;
    peer.encrypt = false;
    ok = esp_now_add_peer(&peer) == ESP_OK;
  }
  stampa("test sincronia: id %u, mac %02X:%02X:%02X:%02X:%02X:%02X, esp-now %s\n", mioId,
                mioMac[0], mioMac[1], mioMac[2], mioMac[3], mioMac[4], mioMac[5], ok ? "ok" : "ERRORE");
  tCorrezione = oraMs();
}

void loop() {
  CoreS3.update();   // legge il touch
  uint32_t ora = millis();

  // tocchi (nessun gesto fa uscire dalla grazia: finisce solo alla durata)
  auto t = CoreS3.Touch.getDetail();
  if (t.wasClicked()) {
    if (ruolo == LIBERO) avvia();
    else { audio = !audio; ridisegna = true; }
  }

  leggiComandi();

  // pacchetti arrivati (isolato: si buttano)
  Arrivo a;
  while (estrai(a)) if (!isolato() && a.da != ignoraId) gestisciPacchetto(a);

  // correzione lisciata di t0: al massimo 10 ms al secondo.
  // passoMax è quanto posso correggere in questo giro: 10 ms per i secondi passati
  // dal giro precedente. Con un loop ogni 2 ms sono 0,02 ms per giro, impercettibili
  double adesso = oraMs();
  double passoMax = CORREZIONE_MAX * (adesso - tCorrezione) / 1000.0;
  tCorrezione = adesso;
  if (scarto != 0) {
    double c = constrain(scarto, -passoMax, passoMax);
    t0 += c; scarto -= c;
  }

  // fine: ognuno la calcola sul proprio orologio, quindi tutti escono allo stesso passo
  // anche senza sentire il FINE dell'origine
  if (ruolo != LIBERO && residuoMs() <= 0) finisci("durata raggiunta");

  // fine del riposo: torna "tocca per iniziare"
  if (eraInRiposo && !inRiposo()) { eraInRiposo = false; ridisegna = true; }

  // annuncio, con 100 ms casuali in più o in meno perché i dispositivi non trasmettano sempre insieme
  if (ora - tAnnuncio >= OGNI_ANNUNCIO) {
    tAnnuncio = ora + random(0, 200) - 100;
    inviaAnnuncio();
  }

  // origine: SYNC ogni 2 s
  if (ruolo == ORIGINE && ora - tInvioSync >= OGNI_SYNC) { tInvioSync = ora; inviaSync(); }

  // inoltro pendente, col trascorso aggiornato al mio orologio:
  // quello originale sarebbe vecchio di 50..300 ms
  if (inoltroPendente && (int32_t)(ora - tInoltro) >= 0) {
    inoltroPendente = false;
    if (ruolo == PARTECIPANTE && daInoltrare.sessione == sessione) {
      daInoltrare.trascorso = (uint32_t)(oraMs() - t0);
      invia(&daInoltrare, sizeof(daInoltrare));
      tUltimoInoltro = ora;
    }
  }

  // continuità: origine muta da 6 s, l'id più basso prende l'orologio (stessa sessione,
  // stesso t0, stessa durata: la musica non si ferma). Chi non è il più basso continua
  // sul proprio orologio finché non arriva un SYNC, o fino alla fine della durata
  if (ruolo == PARTECIPANTE) {
    int32_t muta = (int32_t)(millis() - tUltimoSync);   // con segno: tUltimoSync può essere più recente di ora
    if (muta > (int32_t)SILENZIO_ELEZIONE && sonoIlPiuBasso()) {
      stampa("[%lu] origine %u muta, prendo l'orologio\n", millis(), origine);
      origine = mioId; scarto = 0;
      tInvioSync = 0;
      cambiaRuolo(ORIGINE);
    }
  }

  // battito: lampo e clic a ogni quarto.
  // p > ultimoPasso (e non !=): se t0 viene corretto all'indietro, un passo già
  // suonato non si risuona; si aspetta semplicemente il prossimo
  if (ruolo != LIBERO) {
    long p = passoCorrente();
    if (p > ultimoPasso) {
      ultimoPasso = p;
      if (p % griglia == 0) {   // un passo multiplo di 4 è un battito
        long battito = p / griglia;
        bool primo = battito % 4 == 0;   // primo battito della battuta
        if (stampaBattiti)   // ritardo: quanto il loop è arrivato dopo l'istante del battito
          stampa("B %u %ld %.2f\n", sessione, battito, (oraMs() - t0) - p * 60000.0 / (bpm * griglia));
        if (audio) CoreS3.Speaker.tone(primo ? 1760 : 880, 40);   // prima il clic: tone non blocca
        CoreS3.Display.fillRect(0, BARRA, W, H - BARRA, primo ? GIALLO : AZZURRO);
        lampoAcceso = true;
        tLampo = ora;
      }
    }
  }
  // fine del lampo: torna il nero con i quadrati
  if (lampoAcceso && ora - tLampo >= DURATA_LAMPO) {
    lampoAcceso = false;
    disegnaInfo();
    disegnaCorpo(ultimoPasso / griglia);
    ridisegna = false;
    tInfo = ora;
  }

  // da libero, o se qualcosa è cambiato, ridisegna senza disturbare il lampo
  if (!lampoAcceso && (ridisegna || ora - tInfo >= 1000)) {
    disegnaInfo();
    if (ridisegna) disegnaCorpo(ruolo == LIBERO ? -1 : ultimoPasso / griglia);
    ridisegna = false;
    tInfo = ora;
  }
  delay(1);
}
