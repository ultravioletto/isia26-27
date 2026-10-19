// Generato da aggiorna.py a partire da ProvaBlob/ProvaBlob.ino: non modificare qui.
#pragma once
#include <Arduino.h>
#include <M5CoreS3.h>
#include <math.h>
namespace blob {
#line 1 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
// =====================================================================
//  Prova Blob
//  Un blob 2D che rotola seguendo l'inclinazione del CoreS3 e si
//  deforma con il microfono.
//
//  Movimento: l'inclinazione è gravità. Il blob accelera, ha attrito,
//             rimbalza sui bordi e si schiaccia contro le pareti.
//  Corpo:     il contorno è un anello di punti, ognuno con una molla
//             radiale; i vicini si tengono insieme e una pressione
//             interna conserva l'area, così quando un lato si schiaccia
//             gli altri si gonfiano.
//  Suono:     il volume gonfia il blob; lo spettro (FFT, 8 bande) dà
//             la forma: le bande gravi muovono onde larghe e lente sul
//             contorno, le acute increspature fitte. Voce grave =
//             morbido, fischio = spinoso.
//  Debug:     tocco sullo schermo per mostrare/nascondere fps, livello
//             e bande.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
// =====================================================================


// ---------------------------------------------------------------- parametri
// orientamento: se il blob va dalla parte sbagliata, inverti il segno
const float SEGNO_X = -1.0f, SEGNO_Y = 1.0f;     // come la livella dell'Esploratore

const float R0          = 46.0f;    // raggio a riposo, px
const float GRAVITA     = 1400.0f;  // px/s² per 1 g di inclinazione
const float ATTRITO     = 1.6f;     // smorzamento della velocità, 1/s
const float RIMBALZO    = 0.55f;    // velocità conservata nell'urto
const float AVVICINA    = 0.62f;    // quanto il centro può avvicinarsi al bordo (frazione di R0)

const float MOLLA       = 220.0f;   // richiamo radiale di ogni punto verso la forma bersaglio
const float SMORZA      = 9.0f;     // smorzamento dei punti
const float COESIONE    = 900.0f;   // quanto i punti vicini si tengono allineati
const float PRESSIONE   = 260.0f;   // quanto il blob difende la sua area
const float INERZIA     = 0.45f;    // quanto il corpo "resta indietro" quando il centro accelera

const float GONFIA      = 0.35f;    // aumento massimo del raggio con il volume
const float FORMA       = 0.22f;    // ampiezza massima di ogni onda di spettro (frazione del raggio)

// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240;
M5Canvas tela(&CoreS3.Display);
struct Rett { int x0, y0, x1, y1; };        // rettangolo a schermo, estremi inclusi

#line 50 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);
#line 55 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t);
#line 74 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void bloccoPronto(void*, void* dati, size_t n);
#line 83 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void leggiFinestra();
#line 94 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void fft(float* x, float* y, int n);
#line 117 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
float aggiornaFondo(float& fondo, float db, float dt);
#line 124 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void liscia(float& v, float bersaglio, float dt);
#line 129 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void analizzaAudio(const int16_t* s, float dt);
#line 153 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void preparaAudio();
#line 173 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void preparaBlob();
#line 188 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void preparaArmoniche();
#line 196 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void calcolaBersaglio();
#line 210 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void passoBlob(float gx, float gy, float dt);
#line 259 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void calcolaContorno();
#line 270 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
Rett unione(const Rett& a, const Rett& b);
#line 274 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void disegnaBlob(M5Canvas& c);
#line 298 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void misura(float& media, uint32_t t0);
#line 302 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void disegnaDebug(M5Canvas& c, float gx, float gy);
#line 318 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void pulisci(M5Canvas& c, const Rett& r);
#line 324 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void invia(const Rett& r);
#line 331 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void setup();
#line 362 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
void loop();
#line 50 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\code\\arduino\\ProvaBlob\\ProvaBlob.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t SFONDO = rgb(14, 14, 18), BIANCO = rgb(255, 255, 255), GRIGIO = rgb(120, 120, 120);

uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t) {
  t = constrain(t, 0.0f, 1.0f);
  return rgb(r1 + (r2 - r1) * t, g1 + (g2 - g1) * t, b1 + (b2 - b1) * t);
}

// ---------------------------------------------------------------- audio
static constexpr int    N_FFT = 512;          // finestra di analisi: 32 ms a 16 kHz, 31 Hz per bin
static constexpr int    BLOCCO = 64;          // pezzo che arriva dal microfono: 4 ms
static constexpr int    N_ANELLO = 1024;      // ultimi 64 ms di suono
static constexpr int    FREQ  = 16000;
static constexpr int    N_BANDE = 8;
// Il microfono registra di continuo blocchi piccoli: ogni blocco finito viene copiato
// nell'anello e subito rimesso in coda, dal task del microfono stesso. Il loop non aspetta
// mai l'audio e analizza a ogni frame gli ultimi N_FFT campioni, freschi al più di 4 ms.
int16_t blocchi[2][BLOCCO];                   // due blocchi in coda, così la registrazione non si interrompe
int16_t anello[N_ANELLO];
volatile uint32_t scritti = 0;                // campioni scritti nell'anello dall'accensione
int16_t finestraMic[N_FFT];

void bloccoPronto(void*, void* dati, size_t n) {
  const int16_t* d = (const int16_t*)dati;
  uint32_t w = scritti;
  for (size_t i = 0; i < n; i++) anello[(w + i) % N_ANELLO] = d[i];
  scritti = w + n;
  CoreS3.Mic.record((int16_t*)dati, n, FREQ);
}

// copia gli ultimi N_FFT campioni dell'anello
void leggiFinestra() {
  uint32_t w = scritti;
  for (int i = 0; i < N_FFT; i++) finestraMic[i] = anello[(w + N_ANELLO - N_FFT + i) % N_ANELLO];
}
float   re[N_FFT], im[N_FFT], finestra[N_FFT];
int     binBanda[N_BANDE + 1];

float banda[N_BANDE];         // livello lisciato di ogni banda, 0..1
float fondoBanda[N_BANDE];    // rumore di fondo di ogni banda, dB, si adatta alla stanza
float volume = 0, fondoVol = -60, dbVol = -90;

void fft(float* x, float* y, int n) {
  for (int i = 1, j = 0; i < n; i++) {                 // permutazione bit-reverse
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) { float t = x[i]; x[i] = x[j]; x[j] = t; t = y[i]; y[i] = y[j]; y[j] = t; }
  }
  for (int len = 2; len <= n; len <<= 1) {
    float ang = -2.0f * PI / len, wr = cosf(ang), wi = sinf(ang);
    for (int i = 0; i < n; i += len) {
      float cr = 1, ci = 0;
      for (int k = 0; k < len / 2; k++) {
        int a = i + k, b = a + len / 2;
        float tr = x[b] * cr - y[b] * ci, ti = x[b] * ci + y[b] * cr;
        x[b] = x[a] - tr; y[b] = y[a] - ti;
        x[a] += tr;       y[a] += ti;
        float t = cr * wr - ci * wi; ci = cr * wi + ci * wr; cr = t;
      }
    }
  }
}

// il fondo segue in fretta i valori più bassi e risale piano: così si adatta al rumore della stanza
float aggiornaFondo(float& fondo, float db, float dt) {
  if (db < fondo) fondo += (db - fondo) * min(1.0f, dt * 4.0f);
  else            fondo += (db - fondo) * min(1.0f, dt * 0.05f);
  return db - fondo;
}

// attacco veloce, rilascio lento
void liscia(float& v, float bersaglio, float dt) {
  float k = bersaglio > v ? 18.0f : 3.0f;
  v += (bersaglio - v) * min(1.0f, dt * k);
}

void analizzaAudio(const int16_t* s, float dt) {
  double somma = 0, media = 0;
  for (int i = 0; i < N_FFT; i++) media += s[i];
  media /= N_FFT;                                       // via la componente continua
  for (int i = 0; i < N_FFT; i++) {
    float v = (s[i] - media) / 32768.0f;
    somma += v * v;
    re[i] = v * finestra[i];
    im[i] = 0;
  }
  dbVol = 10.0f * log10f(somma / N_FFT + 1e-12f);
  float sopra = aggiornaFondo(fondoVol, dbVol, dt);
  liscia(volume, constrain((sopra - 4.0f) / 30.0f, 0.0f, 1.0f), dt);

  fft(re, im, N_FFT);
  for (int b = 0; b < N_BANDE; b++) {
    float e = 0;
    for (int k = binBanda[b]; k < binBanda[b + 1]; k++) e += re[k] * re[k] + im[k] * im[k];
    float db = 10.0f * log10f(e / (binBanda[b + 1] - binBanda[b]) + 1e-12f);
    float sopraB = aggiornaFondo(fondoBanda[b], db, dt);
    liscia(banda[b], constrain((sopraB - 6.0f) / 28.0f, 0.0f, 1.0f), dt);
  }
}

void preparaAudio() {
  for (int i = 0; i < N_FFT; i++) finestra[i] = 0.5f - 0.5f * cosf(2.0f * PI * i / (N_FFT - 1));   // Hann
  // bande logaritmiche da 120 Hz a 5 kHz
  for (int b = 0; b <= N_BANDE; b++) {
    float f = 120.0f * powf(5000.0f / 120.0f, (float)b / N_BANDE);
    binBanda[b] = max(1, (int)roundf(f * N_FFT / FREQ));
    if (b > 0 && binBanda[b] <= binBanda[b - 1]) binBanda[b] = binBanda[b - 1] + 1;
  }
  for (int b = 0; b < N_BANDE; b++) fondoBanda[b] = -90;
}

// ---------------------------------------------------------------- blob
static constexpr int N_PUNTI = 72;
float cx = W / 2, cy = H / 2, vx = 0, vy = 0;   // centro e velocità
float rp[N_PUNTI], vp[N_PUNTI];                 // raggio di ogni punto e sua velocità radiale
float dirX[N_PUNTI], dirY[N_PUNTI];
float fase[N_BANDE];                            // ogni onda di spettro gira lentamente sul contorno
float ax0 = 0, ay0 = 0;                         // accelerazione del centro nel passo precedente
int16_t px[N_PUNTI], py[N_PUNTI];

void preparaBlob() {
  for (int i = 0; i < N_PUNTI; i++) {
    float a = 2.0f * PI * i / N_PUNTI;
    dirX[i] = cosf(a); dirY[i] = sinf(a);
    rp[i] = R0; vp[i] = 0;
  }
  for (int b = 0; b < N_BANDE; b++) fase[b] = b * 1.7f;
}

// raggio voluto per ogni punto: cerchio gonfiato dal volume e modellato dallo spettro.
// Non cambia dentro il frame, quindi si calcola una volta sola prima dei sottopassi.
// sin(k·a + fase) = sin(k·a)·cos(fase) + cos(k·a)·sin(fase): i seni di k·a sono in tabella.
float rBase = R0, rBersaglio[N_PUNTI];
float senoArm[N_BANDE][N_PUNTI], cosenoArm[N_BANDE][N_PUNTI];

void preparaArmoniche() {
  for (int b = 0; b < N_BANDE; b++)
    for (int i = 0; i < N_PUNTI; i++) {
      float a = 2.0f * PI * i / N_PUNTI * (b + 2);          // banda 0 → 2 lobi, banda 7 → 9 lobi
      senoArm[b][i] = sinf(a); cosenoArm[b][i] = cosf(a);
    }
}

void calcolaBersaglio() {
  rBase = R0 * (1.0f + GONFIA * volume);
  float pc[N_BANDE], ps[N_BANDE];
  for (int b = 0; b < N_BANDE; b++) {
    float amp = banda[b] * FORMA / sqrtf((b + 2) * 0.5f);
    pc[b] = amp * cosf(fase[b]); ps[b] = amp * sinf(fase[b]);
  }
  for (int i = 0; i < N_PUNTI; i++) {
    float m = 1.0f;
    for (int b = 0; b < N_BANDE; b++) m += senoArm[b][i] * pc[b] + cosenoArm[b][i] * ps[b];
    rBersaglio[i] = rBase * m;
  }
}

void passoBlob(float gx, float gy, float dt) {
  // centro: gravità, attrito, pareti
  float vx0 = vx, vy0 = vy;
  vx += gx * GRAVITA * dt;  vy += gy * GRAVITA * dt;
  float att = expf(-ATTRITO * dt);
  vx *= att; vy *= att;
  cx += vx * dt; cy += vy * dt;
  float m = R0 * AVVICINA;
  if (cx < m)     { cx = m;     if (vx < 0) vx = -vx * RIMBALZO; }
  if (cx > W - m) { cx = W - m; if (vx > 0) vx = -vx * RIMBALZO; }
  if (cy < m)     { cy = m;     if (vy < 0) vy = -vy * RIMBALZO; }
  if (cy > H - m) { cy = H - m; if (vy > 0) vy = -vy * RIMBALZO; }
  float ax = (vx - vx0) / dt, ay = (vy - vy0) / dt;
  // l'urto è un'accelerazione enorme in un passo solo: la si limita perché il corpo non esploda
  float am = sqrtf(ax * ax + ay * ay), aMax = 60000.0f;
  if (am > aMax) { ax *= aMax / am; ay *= aMax / am; }
  ax0 = ax; ay0 = ay;

  // area attuale e area voluta (la pressione spinge tutti i punti allo stesso modo)
  float area = 0;
  for (int i = 0; i < N_PUNTI; i++) area += rp[i] * rp[i];
  float areaVoluta = rBase * rBase * N_PUNTI;
  float spinta = PRESSIONE * (areaVoluta - area) / areaVoluta * R0;

  for (int i = 0; i < N_PUNTI; i++) {
    int p = (i + N_PUNTI - 1) % N_PUNTI, s = (i + 1) % N_PUNTI;
    float f = MOLLA * (rBersaglio[i] - rp[i])
            - SMORZA * vp[i]
            + COESIONE * (rp[p] + rp[s] - 2.0f * rp[i]) / (R0 * 0.1f)
            + spinta
            - INERZIA * (ax * dirX[i] + ay * dirY[i]);    // forza apparente: il corpo resta indietro
    vp[i] += f * dt;
  }
  for (int i = 0; i < N_PUNTI; i++) {
    rp[i] += vp[i] * dt;
    if (rp[i] < 6) { rp[i] = 6; if (vp[i] < 0) vp[i] = 0; }
    // le pareti appiattiscono i punti che uscirebbero dallo schermo
    float lim = 1e9f;
    if (dirX[i] < -1e-3f) lim = min(lim, (0 - cx) / dirX[i]);
    if (dirX[i] >  1e-3f) lim = min(lim, (W - 1 - cx) / dirX[i]);
    if (dirY[i] < -1e-3f) lim = min(lim, (0 - cy) / dirY[i]);
    if (dirY[i] >  1e-3f) lim = min(lim, (H - 1 - cy) / dirY[i]);
    if (rp[i] > lim) { rp[i] = lim; if (vp[i] > 0) vp[i] = 0; }
  }
}

// posizione a schermo dei punti del contorno e rettangolo che li contiene
Rett rettBlob, rettPrima = { 0, 0, W - 1, H - 1 };   // il primo frame pulisce tutto lo schermo

void calcolaContorno() {
  int x0 = W, y0 = H, x1 = 0, y1 = 0;
  for (int i = 0; i < N_PUNTI; i++) {
    px[i] = (int16_t)roundf(cx + rp[i] * dirX[i]);
    py[i] = (int16_t)roundf(cy + rp[i] * dirY[i]);
    x0 = min(x0, (int)px[i]); x1 = max(x1, (int)px[i]);
    y0 = min(y0, (int)py[i]); y1 = max(y1, (int)py[i]);
  }
  rettBlob = { max(0, x0 - 2), max(0, y0 - 2), min(W - 1, x1 + 2), min(H - 1, y1 + 2) };
}

Rett unione(const Rett& a, const Rett& b) {
  return { min(a.x0, b.x0), min(a.y0, b.y0), max(a.x1, b.x1), max(a.y1, b.y1) };
}

void disegnaBlob(M5Canvas& c) {
  // colore: dal blu profondo al corallo con il volume
  uint16_t corpo = mescola(40, 110, 220, 255, 96, 120, volume);

  int16_t x0 = (int16_t)cx, y0 = (int16_t)cy;
  for (int i = 0; i < N_PUNTI; i++) {
    int s = (i + 1) % N_PUNTI;
    c.fillTriangle(x0, y0, px[i], py[i], px[s], py[s], corpo);
  }
  for (int i = 0; i < N_PUNTI; i++) {          // bordo, chiude anche le fessure tra i triangoli
    int s = (i + 1) % N_PUNTI;
    c.drawLine(px[i], py[i], px[s], py[s], corpo);
  }

}

// ---------------------------------------------------------------- debug
bool debug = false, puliziaDebug = false;
float fps = 0;
float msAudio = 0, msFisica = 0, msDisegno = 0, msInvio = 0;   // tempi lisciati di ogni fase
const Rett RETT_TESTO = { 0, 0, W - 1, 52 };
const int BW = 12, BX = W - N_BANDE * (BW + 3) - 6, BY = H - 6;
const Rett RETT_BANDE = { BX - 1, BY - 41, W - 1, H - 1 };

void misura(float& media, uint32_t t0) {
  media += ((micros() - t0) / 1000.0f - media) * 0.1f;
}

void disegnaDebug(M5Canvas& c, float gx, float gy) {
  c.setFont(&fonts::DejaVu12);
  c.setTextColor(GRIGIO);
  c.setTextDatum(textdatum_t::top_left);
  c.drawString(String(fps, 0) + " fps   " + String(dbVol, 1) + " dB   fondo " + String(fondoVol, 0), 6, 4);
  c.drawString("incl " + String(gx, 2) + " " + String(gy, 2) + "   vol " + String(volume, 2), 6, 20);
  c.drawString("ms  audio " + String(msAudio, 1) + "  fisica " + String(msFisica, 1)
               + "  disegno " + String(msDisegno, 1) + "  invio " + String(msInvio, 1), 6, 36);
  for (int b = 0; b < N_BANDE; b++) {
    int h = (int)(banda[b] * 40);
    c.drawRect(BX + b * (BW + 3), BY - 40, BW, 40, GRIGIO);
    if (h > 0) c.fillRect(BX + b * (BW + 3), BY - h, BW, h, BIANCO);
  }
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

  preparaAudio();
  preparaArmoniche();
  preparaBlob();
}

void loop() {
  static uint32_t tPrima = micros();
  uint32_t ora = micros();
  float dt = constrain((ora - tPrima) / 1e6f, 0.001f, 0.06f);
  tPrima = ora;
  fps += (1.0f / dt - fps) * 0.05f;

  CoreS3.update();
  if (CoreS3.Touch.getDetail().wasClicked()) { debug = !debug; if (!debug) puliziaDebug = true; }

  // audio: gli ultimi 32 ms registrati, senza aspettare
  uint32_t t0 = micros();
  leggiFinestra();
  analizzaAudio(finestraMic, dt);
  misura(msAudio, t0);
  for (int b = 0; b < N_BANDE; b++) fase[b] += dt * (0.4f + 0.25f * b) * (b % 2 ? -1 : 1);

  // inclinazione: proiezione della gravità sul piano dello schermo
  t0 = micros();
  float ax = 0, ay = 0, az = 1;
  if (CoreS3.Imu.isEnabled()) CoreS3.Imu.getAccel(&ax, &ay, &az);
  float gx = constrain(SEGNO_X * ax, -1.0f, 1.0f), gy = constrain(SEGNO_Y * ay, -1.0f, 1.0f);

  calcolaBersaglio();
  const int SOTTOPASSI = 4;                   // molle rigide: più passi piccoli per restare stabili
  for (int k = 0; k < SOTTOPASSI; k++) passoBlob(gx, gy, dt / SOTTOPASSI);
  misura(msFisica, t0);

  // si ridisegna solo dove il blob era e dove è adesso
  t0 = micros();
  calcolaContorno();
  Rett sporco = unione(rettPrima, rettBlob);
  rettPrima = rettBlob;
  pulisci(tela, sporco);
  bool testi = debug || puliziaDebug;
  if (testi) { pulisci(tela, RETT_TESTO); pulisci(tela, RETT_BANDE); }
  disegnaBlob(tela);
  if (debug) disegnaDebug(tela, gx, gy);
  misura(msDisegno, t0);

  t0 = micros();
  invia(sporco);
  if (testi) { invia(RETT_TESTO); invia(RETT_BANDE); }
  puliziaDebug = false;
  misura(msInvio, t0);
}

}  // namespace blob
