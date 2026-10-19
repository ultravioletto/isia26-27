// =====================================================================
//  Prova Sonora CoreS3
//  Per ascoltare in stanza cosa sa fare l'altoparlante del CoreS3: timbri,
//  registri, volumi, inviluppi, accordi, e la risposta in frequenza.
//
//  Schermo: tre pagine, scelte con le schede in alto o scorrendo col dito.
//    SUONA     quattro tasti grandi
//              NOTA      la tonica, una volta
//              SCALA     pentatonica maggiore su due ottave
//              ACCORDO   tonica, terza, quinta, ottava insieme
//              SWEEP     una sinusoide che sale da 100 a 5000 Hz in 8 s
//    SUONO     timbro, tonica, inviluppo, con i tasti - e +
//    USCITA    volume e Tutti, con i tasti - e +
//  I tasti di SUONA vanno in loop: il suono si ripete finché non si ripreme
//  lo stesso tasto (il tasto in loop è giallo); un altro tasto cambia loop.
//  Le impostazioni si possono cambiare mentre suona: valgono dalla ripetizione
//  successiva. Senza loop, ogni cambio fa sentire subito la tonica.
//  I tasti di SUONA diventano verdi quando Tutti è attivo.
//
//  Tutti: con "on", ogni tasto viene mandato via ESP-NOW agli altri dispositivi
//  con lo stesso sketch, che adottano le stesse impostazioni e suonano insieme,
//  ognuno con note diverse ma sempre nella pentatonica:
//    NOTA      ogni dispositivo un grado diverso, due gradi sopra la voce
//              precedente; a ogni ripetizione le note cambiano tutte insieme
//    SCALA     ognuno sale partendo da un grado diverso: scale in parallelo
//    ACCORDO   ognuno una nota dell'accordo
//  La voce di un dispositivo è la sua posizione per id tra quelli presenti:
//  ognuno si annuncia ogni secondo. Lo stop, premuto su qualunque dispositivo,
//  ferma tutti.
//
//  Come suona: ogni nota è generata dal programma in un buffer (16 bit, 24 kHz)
//  con la forma d'onda e l'inviluppo scelti, poi suonata con Speaker.playRaw su
//  uno degli 8 canali del mixer di M5Unified. Così si sentono timbri e attacchi
//  che Speaker.tone da solo non fa.
//
//  Cosa ascoltare:
//    - sotto che nota il suono sparisce (SWEEP, poi SCALA dal registro grave)
//    - quali timbri restano chiari a volume basso e quali gracchiano al massimo
//    - se gli attacchi secchi fanno clic e se l'inviluppo morbido li toglie
//    - quanto si somma il volume di tre altoparlanti con Tutti
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (con M5Unified e M5GFX)
// =====================================================================

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <M5CoreS3.h>
#include <math.h>

// ---------------------------------------------------------------- parametri
static constexpr uint32_t SR = 24000;            // campioni al secondo dei buffer
static constexpr float    DURATA_MAX = 1.6f;     // s: la nota più lunga (inviluppo "lungo")
static constexpr int      CAMPIONI_MAX = (int)(SR * DURATA_MAX);
static constexpr int      CANALI = 8;            // canali del mixer di M5Unified
static constexpr float    SWEEP_DA = 100, SWEEP_A = 5000, SWEEP_S = 8;
static constexpr int      CAMPIONI_SWEEP = (int)(SR * SWEEP_S);
static constexpr uint32_t PASSO_SCALA = 333;     // ms tra le note della scala (90 bpm in ottavi)
static constexpr uint8_t  CANALE_RADIO = 1;

// ---------------------------------------------------------------- schermo
// tre pagine, scelte con le schede in alto o con uno scorrimento orizzontale:
// SUONA (i quattro tasti grandi), SUONO (timbro, tonica, inviluppo), USCITA (volume, tutti)
static constexpr int W = 320, H = 240, SCHEDE = 40;          // altezza della barra delle schede
static constexpr int TW = 148, TH = 94, TG = 8;              // tasti di SUONA: larghezza, altezza, spazio
static constexpr int RIGA = 64, BW = 72, BH = 56;            // righe di impostazione e tasti − +
enum Pagina { P_SUONA, P_SUONO, P_USCITA, N_PAGINE };
const char* NOMI_PAGINE[N_PAGINE] = { "SUONA", "SUONO", "USCITA" };
int pagina = P_SUONA;
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), BIANCO = rgb(255, 255, 255), GRIGIO = rgb(120, 120, 120);
const uint16_t SCURO = rgb(40, 40, 40), GIALLO = rgb(255, 210, 63), AZZURRO = rgb(63, 167, 255);
const uint16_t VERDE = rgb(46, 194, 126);

// ---------------------------------------------------------------- impostazioni
const char* TIMBRI[] = { "sinusoide", "triangolo", "quadra", "dente di sega", "impulso",
                         "organo", "campana (FM)", "pizzico", "rumore" };
static constexpr int N_TIMBRI = 9;
enum { T_SIN, T_TRI, T_QUADRA, T_SEGA, T_IMPULSO, T_ORGANO, T_CAMPANA, T_PIZZICO, T_RUMORE };

const char* INVILUPPI[] = { "secco", "morbido", "lungo", "percussivo" };
static constexpr int N_INVILUPPI = 4;
const float DURATE[] = { 0.25f, 0.6f, 1.5f, 0.6f };   // s, per inviluppo

const char* NOMI_NOTE[] = { "do", "do#", "re", "re#", "mi", "fa", "fa#", "sol", "sol#", "la", "la#", "si" };

struct __attribute__((packed)) Impostazioni { uint8_t timbro, tonica, volume, inviluppo; };
Impostazioni imp = { T_ORGANO, 60, 160, 1 };   // organo, do4, volume 160, morbido
bool tutti = false;

// A_FERMA: ferma il loop in corso (mandato agli altri quando si ripreme il tasto)
enum Azione : uint8_t { A_FERMA = 0, A_NOTA, A_SCALA, A_ACCORDO, A_SWEEP };
// seme: scelto da chi preme e uguale per tutti; insieme alla voce di ognuno decide le note
struct __attribute__((packed)) Pacchetto { char firma[4]; uint8_t azione, mittente, seme; Impostazioni imp; };
struct __attribute__((packed)) Presenza { char firma[4]; uint8_t id; };   // "ci sono", ogni secondo

uint8_t mioId = 0;

// chi c'è nella stanza: per ogni id, quando l'ho sentito l'ultima volta (millis)
volatile uint32_t visto[256];
uint32_t tPresenza = 0;

// la mia voce: quanti dispositivi presenti hanno un id più basso del mio (0, 1, 2...).
// Ognuno la calcola da solo; se tutti si sentono, le voci sono tutte diverse
bool presente(int id) { return id != mioId && visto[id] && millis() - visto[id] < 5000; }
int miaVoce() { int v = 0; for (int id = 1; id < mioId; id++) v += presente(id); return v; }
int presenti() { int n = 1; for (int id = 1; id < 256; id++) n += presente(id); return n; }

// il loop: l'azione che si ripete finché non si ripreme il suo tasto (A_FERMA = nessuna)
uint8_t inLoop = A_FERMA;
uint32_t tProssima = 0;        // quando ripetere NOTA o ACCORDO

// ---------------------------------------------------------------- buffer
// le forme d'onda periodiche sono tabelle di un ciclo, calcolate all'avvio
static constexpr int TAB = 1024;
float tabelle[6][TAB];
int16_t* voce[CANALI];          // un buffer per canale, in PSRAM
int16_t* sweep = nullptr;       // calcolato al primo uso
int prossimoCanale = 0;

float freqMidi(float m) { return 440.0f * powf(2.0f, (m - 69.0f) / 12.0f); }

String nomeNota(int m) { return String(NOMI_NOTE[m % 12]) + String(m / 12 - 1); }

void preparaTabelle() {
  for (int i = 0; i < TAB; i++) {
    float p = (float)i / TAB, w = 2 * PI * p;
    tabelle[T_SIN][i] = sinf(w);
    tabelle[T_TRI][i] = 4 * fabsf(p - 0.5f) - 1;
    tabelle[T_QUADRA][i] = p < 0.5f ? 0.8f : -0.8f;
    tabelle[T_SEGA][i] = 0.8f * (2 * p - 1);
    tabelle[T_IMPULSO][i] = p < 0.25f ? 0.9f : -0.3f;
    tabelle[T_ORGANO][i] = (sinf(w) + 0.5f * sinf(2 * w) + 0.33f * sinf(3 * w) + 0.25f * sinf(4 * w)) / 2.08f;
  }
}

// inviluppo: ampiezza da 0 a 1 al tempo t (s) di una nota lunga d (s)
float inviluppo(int tipo, float t, float d) {
  float rilascio;
  switch (tipo) {
    case 0:   // secco: attacco e rilascio brevissimi, quasi come Speaker.tone
      rilascio = 0.01f;
      if (t < 0.003f) return t / 0.003f;
      break;
    case 1:   // morbido
      rilascio = 0.2f;
      if (t < 0.03f) return t / 0.03f;
      if (t < 0.13f) return 1 - 0.3f * (t - 0.03f) / 0.1f;
      if (t < d - rilascio) return 0.7f;
      return 0.7f * (d - t) / rilascio;
    case 2:   // lungo
      rilascio = 0.6f;
      if (t < 0.2f) return t / 0.2f;
      if (t < 0.5f) return 1 - 0.4f * (t - 0.2f) / 0.3f;
      if (t < d - rilascio) return 0.6f;
      return 0.6f * (d - t) / rilascio;
    default:  // percussivo: attacco secco, decadimento esponenziale
      if (t < 0.003f) return t / 0.003f;
      return expf(-(t - 0.003f) / 0.12f);
  }
  return t < d - rilascio ? 1 : (d - t) / rilascio;
}

uint32_t semeRumore = 12345;
float rumore() {   // xorshift: veloce e senza bisogno di esp_random a ogni campione
  semeRumore ^= semeRumore << 13; semeRumore ^= semeRumore >> 17; semeRumore ^= semeRumore << 5;
  return (int32_t)semeRumore / 2147483648.0f;
}

// riempie buf con una nota; restituisce quanti campioni ha scritto
int genera(int16_t* buf, float f, int timbro, int inv, float ampiezza) {
  float d = DURATE[inv];
  int n = min((int)(d * SR), CAMPIONI_MAX);
  float fase = 0, passo = f / SR;

  // pizzico: Karplus-Strong, una linea di ritardo lunga un periodo riempita di rumore
  static float linea[SR / 30 + 2];
  int lungo = constrain((int)(SR / f), 2, SR / 30);
  if (timbro == T_PIZZICO) for (int i = 0; i < lungo; i++) linea[i] = rumore();
  int k = 0;

  for (int i = 0; i < n; i++) {
    float t = (float)i / SR, s;
    switch (timbro) {
      case T_CAMPANA: {   // FM: modulante a 1,4 volte la portante, indice che si spegne
        float indice = 5.0f * expf(-t / 0.3f);
        s = sinf(2 * PI * f * t + indice * sinf(2 * PI * 1.4f * f * t));
        break;
      }
      case T_PIZZICO: {
        int k2 = (k + 1) % lungo;
        s = linea[k];
        linea[k] = 0.996f * 0.5f * (linea[k] + linea[k2]);
        k = k2;
        break;
      }
      case T_RUMORE:
        s = rumore() * 0.7f;
        break;
      default: {   // tabella d'onda con interpolazione lineare
        float x = fase * TAB;
        int j = (int)x;
        float fr = x - j;
        s = tabelle[timbro][j % TAB] * (1 - fr) + tabelle[timbro][(j + 1) % TAB] * fr;
        fase += passo;
        if (fase >= 1) fase -= 1;
      }
    }
    buf[i] = (int16_t)(32767 * ampiezza * inviluppo(inv, t, d) * s);
  }
  return n;
}

// genera e suona una nota sul prossimo canale libero (o sul più vecchio)
void suona(float f, float ampiezza) {
  int ch = prossimoCanale;
  for (int i = 0; i < CANALI; i++) {   // prima cerca un canale che non stia suonando
    int c = (prossimoCanale + i) % CANALI;
    if (!CoreS3.Speaker.isPlaying(c)) { ch = c; break; }
  }
  prossimoCanale = (ch + 1) % CANALI;
  if (!voce[ch]) return;     // senza PSRAM i buffer non ci sono: niente suono, ma niente blocco
  CoreS3.Speaker.stop(ch);   // il buffer si riscrive solo a canale fermo
  int n = genera(voce[ch], f, imp.timbro, imp.inviluppo, ampiezza);
  CoreS3.Speaker.playRaw(voce[ch], n, SR, false, 1, ch, true);
}

// ---------------------------------------------------------------- azioni
uint32_t tScala = 0;
int notaScala = -1;            // -1: nessuna scala in corso
int partenzaScala = 0;         // grado da cui parte la mia scala (con Tutti, diverso per ogni voce)
static constexpr int NOTE_SCALA = 11;   // due ottave di pentatonica, tonica in cima compresa

// k-esimo grado della pentatonica maggiore, in semitoni dalla tonica (5 gradi per ottava)
int grado(int k) {
  const int P5[] = { 0, 2, 4, 7, 9 };
  return 12 * (k / 5) + P5[k % 5];
}
int notaMidi(int k) { return constrain(imp.tonica + grado(k), 0, 108); }

uint8_t seme = 0;              // del loop in corso, uguale su tutti i dispositivi
uint16_t ripetizione = 0;      // quante volte il loop si è ripetuto

// grado di partenza comune a tutti per questa ripetizione: cambia a ogni giro,
// ma è lo stesso su ogni dispositivo perché dipende solo da seme e ripetizione
int gradoComune() { return (int)(((seme + 1) * 2654435761u + ripetizione * 40503u) >> 16) % NOTE_SCALA; }
uint32_t tSweep = 0;
bool sweepInCorso = false;

void preparaSweep() {
  if (sweep) return;
  sweep = (int16_t*)heap_caps_malloc(CAMPIONI_SWEEP * sizeof(int16_t), MALLOC_CAP_SPIRAM);
  if (!sweep) return;
  float fase = 0;
  for (int i = 0; i < CAMPIONI_SWEEP; i++) {
    float t = (float)i / SR;
    float f = SWEEP_DA * powf(SWEEP_A / SWEEP_DA, t / SWEEP_S);   // salita logaritmica: un'ottava ogni tanto
    fase += f / SR;
    if (fase >= 1) fase -= 1;
    float bordi = min(1.0f, min(t, SWEEP_S - t) / 0.05f);         // niente clic all'inizio e alla fine
    sweep[i] = (int16_t)(32767 * 0.8f * bordi * sinf(2 * PI * fase));
  }
}

// diviso: suonano più dispositivi insieme (Tutti) e ognuno prende una nota diversa
// della scala, secondo la propria voce
void esegui(uint8_t azione, bool diviso) {
  CoreS3.Speaker.setVolume(imp.volume);
  int v = diviso ? miaVoce() : 0;
  switch (azione) {
    case A_NOTA: {
      // da solo: la tonica. Diviso: un grado della pentatonica, due gradi sopra quello
      // della voce precedente. 11 è primo, quindi fino a 11 voci le note restano diverse
      int m = diviso ? notaMidi((gradoComune() + 2 * v) % NOTE_SCALA) : imp.tonica;
      suona(freqMidi(m), 0.8f);
      Serial.printf("nota %s %.1f Hz, %s, %s, volume %u%s\n", nomeNota(m).c_str(), freqMidi(m),
                    TIMBRI[imp.timbro], INVILUPPI[imp.inviluppo], imp.volume, diviso ? ", diviso" : "");
      break;
    }
    case A_SCALA:
      // diviso: ognuno parte due gradi sopra la voce precedente e sale insieme agli altri
      partenzaScala = diviso ? 2 * v : 0;
      notaScala = 0;
      tScala = millis();
      break;
    case A_ACCORDO: {
      const int intervalli[] = { 0, 4, 7, 12 };   // tonica, terza maggiore, quinta, ottava
      if (diviso) {
        // accordo diviso: ognuno suona una nota, secondo la propria voce
        int i = v % 4;
        suona(freqMidi(imp.tonica + intervalli[i]), 0.8f);
        Serial.printf("accordo diviso: voce %d, suono %s\n", v, nomeNota(imp.tonica + intervalli[i]).c_str());
      } else {
        for (int i = 0; i < 4; i++) suona(freqMidi(imp.tonica + intervalli[i]), 0.3f);   // 4 voci: più piano, per non saturare
        Serial.println("accordo: 4 voci su questo dispositivo");
      }
      break;
    }
    case A_SWEEP:
      preparaSweep();
      if (!sweep) { Serial.println("sweep: memoria insufficiente"); break; }
      CoreS3.Speaker.stop();
      CoreS3.Speaker.playRaw(sweep, CAMPIONI_SWEEP, SR, false, 1, 0, true);
      sweepInCorso = true;
      tSweep = millis();
      break;
  }
}

// ---------------------------------------------------------------- loop dei suoni
// NOTA e ACCORDO si ripetono ogni durata dell'inviluppo (almeno 0,4 s, per lasciare
// un respiro tra una e l'altra); SCALA e SWEEP ripartono appena finiscono.
uint32_t periodo() { return (uint32_t)(max(DURATE[imp.inviluppo], 0.4f) * 1000) + 100; }

void fermaLoop() {
  inLoop = A_FERMA;
  notaScala = -1;
  sweepInCorso = false;
  CoreS3.Speaker.stop();
  Serial.println("loop fermato");
}

bool loopDiviso = false;       // il loop suona insieme ad altri dispositivi (note divise)

void avviaLoop(uint8_t azione, bool diviso, uint8_t s) {
  const char* nomi[] = { "NOTA", "SCALA", "ACCORDO", "SWEEP" };
  fermaLoop();
  inLoop = azione;
  loopDiviso = diviso;
  seme = s;
  ripetizione = 0;
  esegui(azione, diviso);
  tProssima = millis() + periodo();
  Serial.printf("loop: %s%s, voce %d di %d\n", nomi[azione - 1], diviso ? " diviso" : "", miaVoce(), presenti());
}

// tasto di SUONA premuto qui: se il suo loop è già in corso lo ferma, altrimenti lo avvia
// (al posto di un eventuale altro loop). Con Tutti lo stesso succede sugli altri dispositivi,
// con lo stesso seme, così tutti calcolano le stesse note di partenza.
// Lo stop di un loop suonato insieme ferma tutti, da qualunque dispositivo venga premuto,
// anche da uno che il loop l'ha ricevuto e ha Tutti spento
void premi(uint8_t azione) {
  uint8_t nuova = (inLoop == azione) ? A_FERMA : azione;
  bool eraDiviso = inLoop != A_FERMA && loopDiviso;
  uint8_t s = esp_random() & 0xFF;
  if (nuova == A_FERMA) fermaLoop(); else avviaLoop(nuova, tutti, s);
  if (tutti || (nuova == A_FERMA && eraDiviso)) manda(nuova, s);
}

// da chiamare a ogni giro del loop principale
void aggiornaLoop() {
  switch (inLoop) {
    case A_NOTA:
    case A_ACCORDO:
      if ((int32_t)(millis() - tProssima) >= 0) {
        ripetizione++;
        esegui(inLoop, loopDiviso);
        tProssima += periodo();
      }
      break;
    case A_SCALA:
      if (notaScala < 0) esegui(A_SCALA, loopDiviso);   // finita: riparte
      break;
    case A_SWEEP:
      if (!sweepInCorso) esegui(A_SWEEP, loopDiviso);
      break;
  }
}

// ---------------------------------------------------------------- radio
const uint8_t BROADCAST[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
volatile bool arrivato = false;
Pacchetto ricevuto;

void ricevi(const esp_now_recv_info_t* info, const uint8_t* dati, int len) {
  if (len == sizeof(Presenza) && memcmp(dati, "PRES", 4) == 0) {
    visto[((const Presenza*)dati)->id] = millis();
    return;
  }
  if (len != sizeof(Pacchetto) || memcmp(dati, "SUON", 4) != 0) return;
  memcpy(&ricevuto, dati, sizeof(Pacchetto));   // il loop lo esegue: qui niente audio
  visto[ricevuto.mittente] = millis();
  arrivato = true;
}

void manda(uint8_t azione, uint8_t s) {
  Pacchetto p = { { 'S', 'U', 'O', 'N' }, azione, mioId, s, imp };
  esp_now_send(BROADCAST, (const uint8_t*)&p, sizeof(p));
}

void mandaPresenza() {
  Presenza p = { { 'P', 'R', 'E', 'S' }, mioId };
  esp_now_send(BROADCAST, (const uint8_t*)&p, sizeof(p));
}

// ---------------------------------------------------------------- disegno
const char* TASTI[] = { "NOTA", "SCALA", "ACCORDO", "SWEEP" };

// posizione del tasto i (0..3) nella griglia 2 x 2 di SUONA
int tastoX(int i) { return TG / 2 + (i % 2) * (TW + TG); }
int tastoY(int i) { return SCHEDE + 4 + (i / 2) * (TH + 6); }

// frequenza dello sweep in questo momento
float frequenzaSweep() { return SWEEP_DA * powf(SWEEP_A / SWEEP_DA, (millis() - tSweep) / 1000.0f / SWEEP_S); }

// il tasto in loop è giallo e dice come fermarlo; durante lo sweep mostra la frequenza
void disegnaTasto(int i) {
  auto& d = CoreS3.Display;
  bool attivo = inLoop == A_NOTA + i;
  uint16_t col = attivo ? GIALLO : (tutti ? VERDE : AZZURRO);
  d.fillRoundRect(tastoX(i), tastoY(i), TW, TH, 10, col);
  d.setTextColor(NERO, col);
  d.setTextDatum(textdatum_t::middle_center);
  d.setFont(&fonts::DejaVu24);
  d.drawString(TASTI[i], tastoX(i) + TW / 2, tastoY(i) + TH / 2 - (attivo ? 12 : 0));
  if (attivo) {
    d.setFont(&fonts::DejaVu18);
    String sotto = (i == 3 && sweepInCorso) ? String(frequenzaSweep(), 0) + " Hz" : String("tocca: stop");
    d.drawString(sotto, tastoX(i) + TW / 2, tastoY(i) + TH / 2 + 18);
  }
}

// una riga di impostazione: tasto −, etichetta e valore al centro, tasto +
void disegnaRiga(int r, const char* etichetta, const String& valore, uint16_t colValore) {
  auto& d = CoreS3.Display;
  int y = SCHEDE + 4 + r * (RIGA + 4);
  d.fillRoundRect(4, y, BW, BH, 10, SCURO);
  d.fillRoundRect(W - 4 - BW, y, BW, BH, 10, SCURO);
  d.setFont(&fonts::DejaVu40);
  d.setTextColor(BIANCO, SCURO);
  d.setTextDatum(textdatum_t::middle_center);
  d.drawString("-", 4 + BW / 2, y + BH / 2);
  d.drawString("+", W - 4 - BW / 2, y + BH / 2);
  d.setFont(&fonts::DejaVu12);
  d.setTextColor(GRIGIO, NERO);
  d.drawString(etichetta, W / 2, y + 10);
  d.setFont(&fonts::DejaVu18);
  d.setTextColor(colValore, NERO);
  d.drawString(valore, W / 2, y + 36);
}

void disegna() {
  auto& d = CoreS3.Display;
  d.fillScreen(NERO);

  // schede: la pagina attiva in chiaro
  int sw = W / N_PAGINE;
  d.setFont(&fonts::DejaVu18);
  d.setTextDatum(textdatum_t::middle_center);
  for (int p = 0; p < N_PAGINE; p++) {
    bool attiva = p == pagina;
    d.fillRect(p * sw, 0, sw - 2, SCHEDE, attiva ? BIANCO : SCURO);
    d.setTextColor(attiva ? NERO : GRIGIO, attiva ? BIANCO : SCURO);
    d.drawString(NOMI_PAGINE[p], p * sw + sw / 2, SCHEDE / 2);
  }

  switch (pagina) {
    case P_SUONA:
      for (int i = 0; i < 4; i++) disegnaTasto(i);
      break;
    case P_SUONO:
      disegnaRiga(0, "timbro", TIMBRI[imp.timbro], BIANCO);
      disegnaRiga(1, "tonica", nomeNota(imp.tonica) + "  " + String(freqMidi(imp.tonica), 0) + " Hz", BIANCO);
      disegnaRiga(2, "inviluppo", INVILUPPI[imp.inviluppo], BIANCO);
      break;
    case P_USCITA:
      disegnaRiga(0, "volume", String(imp.volume), BIANCO);
      disegnaRiga(1, "tutti i dispositivi", tutti ? "on" : "off", tutti ? VERDE : BIANCO);
      d.setFont(&fonts::DejaVu12);
      d.setTextColor(GRIGIO, NERO);
      d.setTextDatum(textdatum_t::middle_center);
      d.drawString("id " + String(mioId) + "   voce " + String(miaVoce() + 1) + " di " + String(presenti()), W / 2, H - 20);
      break;
  }
}

// ---------------------------------------------------------------- tocchi
// dopo un cambio di impostazione: si sente subito la tonica, se non c'è un loop in corso
// (il loop prende da solo le nuove impostazioni alla ripetizione successiva)
void anteprima() {
  CoreS3.Speaker.setVolume(imp.volume);
  if (inLoop == A_FERMA) esegui(A_NOTA, false);
}

void cambiaPagina(int p) {
  pagina = (p + N_PAGINE) % N_PAGINE;
  disegna();
}

void tocco(int x, int y) {
  if (y < SCHEDE) { cambiaPagina(x / (W / N_PAGINE)); return; }

  if (pagina == P_SUONA) {
    for (int i = 0; i < 4; i++) {
      if (x >= tastoX(i) && x < tastoX(i) + TW && y >= tastoY(i) && y < tastoY(i) + TH) {
        premi(A_NOTA + i);   // avvia il loop, o lo ferma se era già questo
        disegna();
        return;
      }
    }
    return;
  }

  // pagine di impostazione: quale riga, e se il tocco è sul tasto − o sul +
  int r = (y - SCHEDE - 4) / (RIGA + 4);
  int verso = x < 4 + BW + 8 ? -1 : (x > W - 4 - BW - 8 ? 1 : 0);   // un po' di margine attorno ai tasti
  if (verso == 0) return;
  if (pagina == P_SUONO) {
    switch (r) {
      case 0: imp.timbro = (imp.timbro + N_TIMBRI + verso) % N_TIMBRI; anteprima(); break;
      case 1: imp.tonica = constrain(imp.tonica + verso, 36, 96); anteprima(); break;   // da do2 a do7
      case 2: imp.inviluppo = (imp.inviluppo + N_INVILUPPI + verso) % N_INVILUPPI; anteprima(); break;
      default: return;
    }
  } else {
    switch (r) {
      case 0: imp.volume = constrain(imp.volume + verso * 16, 0, 255); anteprima(); break;
      case 1: tutti = !tutti; break;
      default: return;
    }
  }
  disegna();   // ogni cambio si sente subito, con la nota della tonica
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);
  CoreS3.Display.setBrightness(160);

  CoreS3.Speaker.begin();
  CoreS3.Speaker.setVolume(imp.volume);
  CoreS3.Speaker.setAllChannelVolume(255);   // il volume si regola solo col master

  preparaTabelle();
  for (int i = 0; i < CANALI; i++)
    voce[i] = (int16_t*)heap_caps_malloc(CAMPIONI_MAX * sizeof(int16_t), MALLOC_CAP_SPIRAM);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  esp_wifi_set_channel(CANALE_RADIO, WIFI_SECOND_CHAN_NONE);
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);
  mioId = mac[5] % 250 + 1;   // stesso id del TestSincronia
  if (esp_now_init() == ESP_OK) {
    esp_now_register_recv_cb(ricevi);
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, BROADCAST, 6);
    peer.ifidx = WIFI_IF_STA;
    esp_now_add_peer(&peer);
  }
  Serial.printf("prova sonora: id %u, buffer %s\n", mioId, voce[CANALI - 1] ? "ok" : "MANCANO (PSRAM?)");
  disegna();
}

void loop() {
  CoreS3.update();
  auto t = CoreS3.Touch.getDetail();
  if (t.wasFlicked()) {   // scorrimento orizzontale: pagina accanto
    int dx = t.distanceX();
    if (dx < -40) cambiaPagina(pagina + 1);
    else if (dx > 40) cambiaPagina(pagina - 1);
  } else if (t.wasClicked()) tocco(t.x, t.y);

  // comando da un altro dispositivo: adotta le sue impostazioni e suona
  if (arrivato) {
    arrivato = false;
    if (ricevuto.azione == A_FERMA) fermaLoop();
    else { imp = ricevuto.imp; avviaLoop(ricevuto.azione, true, ricevuto.seme); }
    disegna();
  }

  // presenza: ogni secondo, con un po' di caso perché i dispositivi non si sovrappongano
  if ((int32_t)(millis() - tPresenza) >= 0) {
    tPresenza = millis() + 900 + (esp_random() % 200);
    mandaPresenza();
  }

  aggiornaLoop();

  // scala: una nota ogni PASSO_SCALA ms
  if (notaScala >= 0 && millis() - tScala >= (uint32_t)notaScala * PASSO_SCALA) {
    suona(freqMidi(notaMidi(partenzaScala + notaScala)), 0.8f);
    if (++notaScala >= NOTE_SCALA) notaScala = -1;
  }

  // sweep: il tasto mostra la frequenza mentre sale
  if (sweepInCorso) {
    static uint32_t tScritta = 0;
    if (pagina == P_SUONA && millis() - tScritta > 100) { tScritta = millis(); disegnaTasto(3); }
    if (millis() - tSweep > SWEEP_S * 1000) { sweepInCorso = false; disegna(); }
  }
  delay(2);
}
