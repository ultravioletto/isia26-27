// Generato da aggiorna.py a partire da ProvaSciame/ProvaSciame.ino: non modificare qui.
#pragma once
#include <Arduino.h>
#include <M5CoreS3.h>
#include <math.h>
#include <esp_memory_utils.h>
namespace sciame {
#line 1 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
// =====================================================================
//  Prova Sciame
//  Uno sciame di particelle (boids) che fluttua intorno a un punto.
//
//  Bersaglio: un punto invisibile che rotola con l'inclinazione del
//             CoreS3 (pan = sinistra/destra, tilt = avanti/indietro),
//             con inerzia e attrito come il blob.
//  Sciame:    ogni particella segue le regole dei boids: separazione
//             (non toccare i vicini), allineamento (vai dove vanno i
//             vicini), coesione (stai vicino ai vicini), più un'orbita
//             intorno al bersaglio. Un po' di rumore le tiene vive.
//  Suono:     a riposo le particelle girano allineate su un anello
//             intorno al bersaglio. Quando sentono suono l'anello si
//             allarga, fuggono verso l'esterno, perdono l'allineamento e
//             si agitano; tornato il silenzio rientrano piano in orbita.
//             Ogni volta che il suono supera una soglia il verso del giro
//             si inverte.
//             I valori ai due estremi sono in cima al file.
//  Debug:     tocco sullo schermo per mostrare/nascondere fps, livello,
//             pesi, tempi e il bersaglio.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
// =====================================================================


// ---------------------------------------------------------------- parametri
// orientamento: se il bersaglio va dalla parte sbagliata, inverti il segno
const float SEGNO_X = -1.0f, SEGNO_Y = 1.0f;     // come la livella dell'Esploratore

const int   N_PARTICELLE = 180;

// bersaglio
const float GRAVITA     = 900.0f;   // px/s² per 1 g di inclinazione
const float ATTRITO     = 2.2f;     // smorzamento della velocità, 1/s
const float MARGINE     = 30.0f;    // il bersaglio non si avvicina al bordo più di così

// sciame
const float R_VISTA     = 30.0f;    // raggio in cui una particella vede le altre, px
const float R_SPAZIO    = 9.0f;     // sotto questa distanza si respingono, px
const float SEPARAZIONE = 1800.0f;  // forza di separazione
const float V_MIN       = 25.0f;    // px/s

// orbita intorno al bersaglio
const float R_ORBITA    = 10.0f;    // raggio dell'anello a riposo, px
const float RICHIAMO    = 5.0f;     // molla che tiene le particelle sull'anello, 1/s²
const float V_ORBITA    = 90.0f;    // velocità di giro a riposo, px/s (positiva = orario, negativa = antiorario)
const float GIRO        = 3.0f;     // quanto in fretta prendono la velocità di giro, 1/s

// il suono sposta ogni peso da un estremo all'altro (riposo → suono)
const float FUGA_RAGGIO           = 20.0f;                // di quanto si allarga l'anello col suono pieno, px
const float FUGA_SPINTA           = 20.0f;                // spinta verso l'esterno col suono pieno, px/s²
const float COESIONE_SILENZIO     = 1.0f,   COESIONE_SUONO     = 0.0f;     // 1/s²
const float ALLINEAMENTO_SILENZIO = 3.0f,   ALLINEAMENTO_SUONO = 0.2f;     // 1/s
const float AGITAZIONE_SILENZIO   = 50.0f,  AGITAZIONE_SUONO   = 420.0f;   // rumore casuale, px/s²
const float V_MAX_SILENZIO        = 130.0f, V_MAX_SUONO        = 260.0f;   // px/s

// sensibilità: dB sopra il rumore di fondo per iniziare a reagire e per la reazione piena
const float SOGLIA_DB = 3.0f, PIENO_DB = 18.0f;

// inversione del giro: scatta quando il livello supera INVERTI_DB e si riarma solo quando
// riscende sotto RIARMO_DB, così un suono lungo inverte una volta sola
const float INVERTI_DB = 12.0f, RIARMO_DB = 6.0f;
const uint32_t INVERTI_PAUSA_MS = 250;     // tempo minimo tra due inversioni

// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240;
M5Canvas tela(&CoreS3.Display);
struct Rett { int x0, y0, x1, y1; };        // rettangolo a schermo, estremi inclusi

#line 74 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);
#line 80 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t);
#line 85 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
float casuale();
#line 104 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void bloccoPronto(void*, void* dati, size_t n);
#line 113 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
float aggiornaFondo(float& fondo, float db, float dt);
#line 120 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void liscia(float& v, float bersaglio, float dt);
#line 125 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void analizzaAudio(float dt);
#line 150 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void muoviBersaglio(float gx, float gy, float dt);
#line 167 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
float verso(float silenzio, float suono);
#line 169 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void preparaSciame();
#line 177 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void passoSciame(float dt);
#line 237 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void calcolaRett();
#line 247 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
Rett unione(const Rett& a, const Rett& b);
#line 255 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void puntoVeloce(uint16_t* buf, int px, int py, uint16_t c);
#line 259 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void lineaVeloce(uint16_t* buf, int x0, int y0, int x1, int y1, uint16_t c);
#line 272 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void disegnaSciame(M5Canvas& c);
#line 297 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void misura(float& media, uint32_t t0);
#line 301 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void disegnaDebug(M5Canvas& c, float gx, float gy);
#line 317 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void pulisci(M5Canvas& c, const Rett& r);
#line 323 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void invia(const Rett& r);
#line 330 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void setup();
#line 359 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
void loop();
#line 74 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaSciame\\ProvaSciame.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t SFONDO = rgb(14, 14, 18), BIANCO = rgb(255, 255, 255), GRIGIO = rgb(120, 120, 120);
const uint16_t CORALLO = rgb(255, 90, 78);

uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t) {
  t = constrain(t, 0.0f, 1.0f);
  return rgb(r1 + (r2 - r1) * t, g1 + (g2 - g1) * t, b1 + (b2 - b1) * t);
}

float casuale() { return (float)esp_random() / 4294967295.0f * 2.0f - 1.0f; }   // -1..1

// ---------------------------------------------------------------- audio
static constexpr int FREQ     = 16000;
static constexpr int BLOCCO   = 64;           // pezzo che arriva dal microfono: 4 ms
static constexpr int N_ANELLO = 1024;         // ultimi 64 ms di suono
static constexpr int N_FINESTRA = 512;        // il volume si misura sugli ultimi 32 ms
static_assert((N_ANELLO & (N_ANELLO - 1)) == 0, "l'anello deve essere una potenza di 2");
// Il microfono registra di continuo blocchi piccoli: ogni blocco finito viene copiato
// nell'anello e subito rimesso in coda, dal task del microfono stesso. Il loop non aspetta mai.
int16_t blocchi[2][BLOCCO];
int16_t anello[N_ANELLO];
volatile uint32_t scritti = 0;

float volume = 0, fondoVol = -60, dbVol = -90;
float versoGiro = 1;                          // +1 orario, -1 antiorario
bool  armato = true;
uint32_t tInversione = 0;

void bloccoPronto(void*, void* dati, size_t n) {
  const int16_t* d = (const int16_t*)dati;
  uint32_t w = scritti;
  for (size_t i = 0; i < n; i++) anello[(w + i) & (N_ANELLO - 1)] = d[i];
  scritti = w + n;
  CoreS3.Mic.record((int16_t*)dati, n, FREQ);
}

// il fondo segue in fretta i valori più bassi e risale piano: così si adatta al rumore della stanza
float aggiornaFondo(float& fondo, float db, float dt) {
  if (db < fondo) fondo += (db - fondo) * min(1.0f, dt * 4.0f);
  else            fondo += (db - fondo) * min(1.0f, dt * 0.05f);
  return db - fondo;
}

// attacco veloce, rilascio lento
void liscia(float& v, float bersaglio, float dt) {
  float k = bersaglio > v ? 25.0f : 1.2f;
  v += (bersaglio - v) * min(1.0f, dt * k);
}

void analizzaAudio(float dt) {
  // float e maschera di bit: l'ESP32-S3 fa i double in software, e il modulo costa una divisione
  const uint32_t inizio = scritti - N_FINESTRA, m = N_ANELLO - 1;
  int32_t totale = 0;
  for (int i = 0; i < N_FINESTRA; i++) totale += anello[(inizio + i) & m];
  float media = (float)totale / N_FINESTRA;             // via la componente continua
  float somma = 0;
  for (int i = 0; i < N_FINESTRA; i++) {
    float v = anello[(inizio + i) & m] - media;
    somma += v * v;
  }
  dbVol = 10.0f * log10f(somma / (N_FINESTRA * 32768.0f * 32768.0f) + 1e-12f);
  float sopra = aggiornaFondo(fondoVol, dbVol, dt);
  // l'inversione usa il livello istantaneo, non il volume lisciato che scende troppo piano
  if (armato && sopra > INVERTI_DB && millis() - tInversione > INVERTI_PAUSA_MS) {
    versoGiro = -versoGiro; armato = false; tInversione = millis();
  } else if (!armato && sopra < RIARMO_DB) {
    armato = true;
  }
  liscia(volume, constrain((sopra - SOGLIA_DB) / (PIENO_DB - SOGLIA_DB), 0.0f, 1.0f), dt);
}

// ---------------------------------------------------------------- bersaglio
float tx = W / 2, ty = H / 2, tvx = 0, tvy = 0;

void muoviBersaglio(float gx, float gy, float dt) {
  tvx += gx * GRAVITA * dt;  tvy += gy * GRAVITA * dt;
  float att = expf(-ATTRITO * dt);
  tvx *= att; tvy *= att;
  tx += tvx * dt; ty += tvy * dt;
  if (tx < MARGINE)     { tx = MARGINE;     if (tvx < 0) tvx = 0; }
  if (tx > W - MARGINE) { tx = W - MARGINE; if (tvx > 0) tvx = 0; }
  if (ty < MARGINE)     { ty = MARGINE;     if (tvy < 0) tvy = 0; }
  if (ty > H - MARGINE) { ty = H - MARGINE; if (tvy > 0) tvy = 0; }
}

// ---------------------------------------------------------------- sciame
float x[N_PARTICELLE], y[N_PARTICELLE], vx[N_PARTICELLE], vy[N_PARTICELLE];
float axs[N_PARTICELLE], ays[N_PARTICELLE];
float coesione = COESIONE_SILENZIO, allineamento = ALLINEAMENTO_SILENZIO;
float agitazione = AGITAZIONE_SILENZIO, vMax = V_MAX_SILENZIO;

float verso(float silenzio, float suono) { return silenzio + (suono - silenzio) * volume; }

void preparaSciame() {
  for (int i = 0; i < N_PARTICELLE; i++) {
    float a = casuale() * PI, r = 10 + 30 * fabsf(casuale());
    x[i] = tx + r * cosf(a); y[i] = ty + r * sinf(a);
    vx[i] = casuale() * 40; vy[i] = casuale() * 40;
  }
}

void passoSciame(float dt) {
  coesione     = verso(COESIONE_SILENZIO, COESIONE_SUONO);
  allineamento = verso(ALLINEAMENTO_SILENZIO, ALLINEAMENTO_SUONO);
  agitazione   = verso(AGITAZIONE_SILENZIO, AGITAZIONE_SUONO);
  vMax         = verso(V_MAX_SILENZIO, V_MAX_SUONO);
  float raggio = R_ORBITA + FUGA_RAGGIO * volume;
  float vGiro  = versoGiro * V_ORBITA * (1.0f - volume);
  const float vista2 = R_VISTA * R_VISTA, spazio2 = R_SPAZIO * R_SPAZIO;

  for (int i = 0; i < N_PARTICELLE; i++) {
    float sx = 0, sy = 0, mx = 0, my = 0, mvx = 0, mvy = 0;
    int n = 0;
    for (int j = 0; j < N_PARTICELLE; j++) {
      if (j == i) continue;
      float dx = x[j] - x[i], dy = y[j] - y[i];
      if (fabsf(dx) > R_VISTA || fabsf(dy) > R_VISTA) continue;   // scarto veloce prima della distanza vera
      float d2 = dx * dx + dy * dy;
      if (d2 > vista2) continue;
      mx += x[j]; my += y[j]; mvx += vx[j]; mvy += vy[j]; n++;
      if (d2 < spazio2) {                                          // separazione: più forte da vicino
        float k = 1.0f / (d2 + 1.0f);
        sx -= dx * k; sy -= dy * k;
      }
    }
    float ax = SEPARAZIONE * sx, ay = SEPARAZIONE * sy;
    if (n > 0) {
      ax += coesione * (mx / n - x[i]) + allineamento * (mvx / n - vx[i]);
      ay += coesione * (my / n - y[i]) + allineamento * (mvy / n - vy[i]);
    }
    // orbita: una molla radiale verso l'anello e una spinta tangenziale, uguale per tutte,
    // così a riposo girano tutte nello stesso verso. Col suono l'anello si allarga e scappano.
    float rx = x[i] - tx, ry = y[i] - ty, d = sqrtf(rx * rx + ry * ry) + 0.001f;
    float ux = rx / d, uy = ry / d;                   // verso l'esterno
    float tgx = -uy, tgy = ux;                        // tangente: senso orario sullo schermo, perché la y cresce verso il basso
    float radiale = RICHIAMO * (raggio - d) + FUGA_SPINTA * volume;
    float tang = GIRO * (vGiro - (vx[i] * tgx + vy[i] * tgy));
    ax += ux * radiale + tgx * tang + agitazione * casuale();
    ay += uy * radiale + tgy * tang + agitazione * casuale();
    axs[i] = ax; ays[i] = ay;
  }

  // si muovono tutte insieme dopo aver guardato tutte, così l'ordine non conta
  for (int i = 0; i < N_PARTICELLE; i++) {
    vx[i] += axs[i] * dt; vy[i] += ays[i] * dt;
    float v = sqrtf(vx[i] * vx[i] + vy[i] * vy[i]);
    if (v > vMax)                { vx[i] *= vMax / v; vy[i] *= vMax / v; }
    else if (v < V_MIN && v > 0) { vx[i] *= V_MIN / v; vy[i] *= V_MIN / v; }
    x[i] += vx[i] * dt; y[i] += vy[i] * dt;
    if (x[i] < 1)     { x[i] = 1;     vx[i] =  fabsf(vx[i]); }
    if (x[i] > W - 2) { x[i] = W - 2; vx[i] = -fabsf(vx[i]); }
    if (y[i] < 1)     { y[i] = 1;     vy[i] =  fabsf(vy[i]); }
    if (y[i] > H - 2) { y[i] = H - 2; vy[i] = -fabsf(vy[i]); }
  }
}

// ---------------------------------------------------------------- disegno
// ogni particella è un trattino nella direzione in cui va: così l'allineamento si vede
const float CODA = 0.05f;                     // lunghezza del trattino, s di movimento
Rett rettSciame, rettPrima = { 0, 0, W - 1, H - 1 };   // il primo frame pulisce tutto lo schermo

void calcolaRett() {
  float x0 = W, y0 = H, x1 = 0, y1 = 0;
  for (int i = 0; i < N_PARTICELLE; i++) {
    float cx = x[i] - vx[i] * CODA, cy = y[i] - vy[i] * CODA;
    x0 = min(x0, min(x[i], cx)); x1 = max(x1, max(x[i], cx));
    y0 = min(y0, min(y[i], cy)); y1 = max(y1, max(y[i], cy));
  }
  rettSciame = { max(0, (int)x0 - 2), max(0, (int)y0 - 2), min(W - 1, (int)x1 + 2), min(H - 1, (int)y1 + 2) };
}

Rett unione(const Rett& a, const Rett& b) {
  return { min(a.x0, b.x0), min(a.y0, b.y0), max(a.x1, b.x1), max(a.y1, b.y1) };
}

// Disegno diretto nel buffer dello sprite. Ogni drawLine o drawPixel di M5GFX fa clip,
// finestra e conversione del colore: per trattini di pochi pixel quel lavoro costa molto
// più dei pixel stessi, e con 360 chiamate a frame era la parte più lenta del loop.
// Lo sprite a 16 bit tiene i colori RGB565 con i byte scambiati.
inline void puntoVeloce(uint16_t* buf, int px, int py, uint16_t c) {
  if ((unsigned)px < (unsigned)W && (unsigned)py < (unsigned)H) buf[py * W + px] = c;
}

void lineaVeloce(uint16_t* buf, int x0, int y0, int x1, int y1, uint16_t c) {   // Bresenham
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  while (true) {
    puntoVeloce(buf, x0, y0, c);
    if (x0 == x1 && y0 == y1) break;
    int e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}

void disegnaSciame(M5Canvas& c) {
  // dal blu freddo (nuvola) al corallo (corrente) con il volume
  uint16_t col = mescola(90, 160, 255, 255, 120, 100, volume);
  uint16_t* buf = (uint16_t*)c.getBuffer();
  bool veloce = buf && c.getColorDepth() == 16 && c.width() == W;
  uint16_t colBuf = __builtin_bswap16(col);
  for (int i = 0; i < N_PARTICELLE; i++) {
    int x0 = (int)x[i], y0 = (int)y[i];
    int x1 = (int)(x[i] - vx[i] * CODA), y1 = (int)(y[i] - vy[i] * CODA);
    if (veloce) {
      lineaVeloce(buf, x0, y0, x1, y1, colBuf);
      puntoVeloce(buf, x0 + 1, y0, colBuf);     // testa un po' più spessa
    } else {                                    // sprite a 8 bit: si passa da M5GFX
      c.drawLine(x0, y0, x1, y1, col);
      c.drawPixel(x0 + 1, y0, col);
    }
  }
}

// ---------------------------------------------------------------- debug
bool debug = false, puliziaDebug = false;
float fps = 0;
float msAudio = 0, msFisica = 0, msDisegno = 0, msInvio = 0;   // tempi lisciati di ogni fase
const Rett RETT_TESTO = { 0, 0, W - 1, 52 };

void misura(float& media, uint32_t t0) {
  media += ((micros() - t0) / 1000.0f - media) * 0.1f;
}

void disegnaDebug(M5Canvas& c, float gx, float gy) {
  c.setFont(&fonts::DejaVu12);
  c.setTextColor(GRIGIO);
  c.setTextDatum(textdatum_t::top_left);
  c.drawString(String(fps, 0) + " fps   " + String(dbVol, 1) + " dB   fondo " + String(fondoVol, 0)
               + "   vol " + String(volume, 2), 6, 4);
  c.drawString(String(versoGiro > 0 ? "orario" : "antiorario") + "   coesione " + String(coesione, 1) + "   allineamento " + String(allineamento, 1)
               + "   incl " + String(gx, 1) + " " + String(gy, 1), 6, 20);
  c.drawString("ms aud " + String(msAudio, 1) + " fis " + String(msFisica, 1)
               + " dis " + String(msDisegno, 1) + " inv " + String(msInvio, 1)
               + (esp_ptr_external_ram(tela.getBuffer()) ? "  sprite PSRAM" : "  sprite RAM"), 6, 36);
  c.drawFastHLine((int)tx - 5, (int)ty, 11, CORALLO);
  c.drawFastVLine((int)tx, (int)ty - 5, 11, CORALLO);
}

// ---------------------------------------------------------------- schermo
void pulisci(M5Canvas& c, const Rett& r) {
  c.fillRect(r.x0, r.y0, r.x1 - r.x0 + 1, r.y1 - r.y0 + 1, SFONDO);
}

// manda allo schermo solo il rettangolo r dello sprite: il clip dello schermo
// fa sì che pushSprite trasferisca solo quei pixel
void invia(const Rett& r) {
  CoreS3.Display.setClipRect(r.x0, r.y0, r.x1 - r.x0 + 1, r.y1 - r.y0 + 1);
  tela.pushSprite(0, 0);
  CoreS3.Display.clearClipRect();
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);

  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(SFONDO);

  // lo sprite (150 KB) sta nella RAM interna: si disegna e si trasferisce più in fretta che dalla PSRAM
  tela.setColorDepth(16);
  tela.setPsram(false);
  if (!tela.createSprite(W, H)) {
    tela.setPsram(true);
    if (!tela.createSprite(W, H)) { tela.setColorDepth(8); tela.createSprite(W, H); }
  }

  // microfono e altoparlante condividono l'I2S
  CoreS3.Speaker.end();
  auto mc = CoreS3.Mic.config();
  mc.dma_buf_len = BLOCCO;                    // il DMA consegna pezzi grandi quanto il blocco
  CoreS3.Mic.config(mc);
  CoreS3.Mic.setBufferReleaseCallback(nullptr, bloccoPronto);
  CoreS3.Mic.begin();
  CoreS3.Mic.record(blocchi[0], BLOCCO, FREQ);
  CoreS3.Mic.record(blocchi[1], BLOCCO, FREQ);

  preparaSciame();
}

void loop() {
  static uint32_t tPrima = micros();
  uint32_t ora = micros();
  float dt = constrain((ora - tPrima) / 1e6f, 0.001f, 0.05f);
  tPrima = ora;
  fps += (1.0f / dt - fps) * 0.05f;

  CoreS3.update();
  if (CoreS3.Touch.getDetail().wasClicked()) { debug = !debug; if (!debug) puliziaDebug = true; }

  uint32_t t0 = micros();
  analizzaAudio(dt);
  misura(msAudio, t0);

  // inclinazione: proiezione della gravità sul piano dello schermo
  t0 = micros();
  float ax = 0, ay = 0, az = 1;
  if (CoreS3.Imu.isEnabled()) CoreS3.Imu.getAccel(&ax, &ay, &az);
  float gx = constrain(SEGNO_X * ax, -1.0f, 1.0f), gy = constrain(SEGNO_Y * ay, -1.0f, 1.0f);
  muoviBersaglio(gx, gy, dt);
  passoSciame(dt);
  misura(msFisica, t0);

  // si ridisegna solo dove lo sciame era e dove è adesso
  t0 = micros();
  calcolaRett();
  bool testi = debug || puliziaDebug;
  if (debug) rettSciame = unione(rettSciame, { (int)tx - 6, (int)ty - 6, (int)tx + 6, (int)ty + 6 });   // la croce del bersaglio
  Rett sporco = unione(rettPrima, rettSciame);
  rettPrima = rettSciame;
  pulisci(tela, sporco);
  if (testi) pulisci(tela, RETT_TESTO);
  disegnaSciame(tela);
  misura(msDisegno, t0);                      // le scritte del debug non contano nel tempo di disegno
  if (debug) disegnaDebug(tela, gx, gy);

  t0 = micros();
  invia(sporco);
  if (testi) invia(RETT_TESTO);
  puliziaDebug = false;
  misura(msInvio, t0);
}

}  // namespace sciame
