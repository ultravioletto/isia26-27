// Generato da aggiorna.py a partire da ProvaScatola/ProvaScatola.ino: non modificare qui.
#pragma once
#include <Arduino.h>
#include <M5CoreS3.h>
#include <math.h>
namespace scatola {
#line 1 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
// =====================================================================
//  Prova Scatola
//  Una stanza vettoriale, come i primi videogiochi: griglia blu su
//  nero, linee senza superfici. Dentro una sfera, un cubo e una
//  piramide in wireframe.
//
//  Stanza:   lo schermo è l'apertura di una scatola che continua dietro
//            il vetro. Il bordo dello schermo è il bordo della scatola;
//            fondo, pavimento, soffitto e pareti hanno una griglia che
//            sfuma con la profondità.
//  Gravità:  la scatola è "in mano". La gravità è il vettore completo
//            dell'accelerometro: col device in piedi gli oggetti stanno
//            sul pavimento, appoggiato sul tavolo cadono verso il fondo,
//            inclinato scivolano verso il lato basso.
//  Vista:    la telecamera si sposta un po' con l'inclinazione
//            (parallasse), così la scatola si legge in profondità.
//  Fisica:   gli oggetti rimbalzano sulle sei facce (il vetro davanti
//            compreso), non tra loro. Ruotano quando toccano una parete;
//            la rotazione è solo visiva.
//  Suono:    quando il suono supera una soglia ogni oggetto riceve un
//            colpo contro la gravità, più forte se il suono è più forte,
//            con una spinta laterale a caso. Un colpo per ogni suono.
//  Debug:    tocco sullo schermo per mostrare/nascondere fps, livello,
//            gravità e tempi.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
// =====================================================================


// ---------------------------------------------------------------- parametri
// orientamento: se gli oggetti cadono dalla parte sbagliata, inverti il segno
const float SEGNO_X = -1.0f, SEGNO_Y = 1.0f, SEGNO_Z = 1.0f;

// stanza, in unità del mondo (lo schermo è largo 2·BX)
const float BX = 1.6f, BY = 1.2f, PROF = 2.6f;   // mezza larghezza, mezza altezza, profondità
const int   GRIGLIA_X = 8, GRIGLIA_Y = 6, GRIGLIA_Z = 6;
const float FOCALE = 2.2f;          // distanza dell'occhio dal vetro: più piccola = prospettiva più forte
const float PARALLASSE = 1.3f;      // quanto si sposta l'occhio con l'inclinazione (segno opposto per invertire)

// fisica
const float GRAVITA   = 7.0f;       // unità/s² per 1 g
const float RIMBALZO  = 0.55f;      // velocità conservata nell'urto con una parete
const float STRISCIO  = 1.8f;       // attrito quando un oggetto tocca una parete, 1/s
const float ARIA      = 0.15f;      // attrito dell'aria, 1/s
const float ROTOLA    = 0.35f;      // quanto la rotazione segue il moto sulla parete (0..1 per urto)

// salto col suono: scatta sopra SALTO_DB, si riarma sotto RIARMO_DB
const float SALTO_DB = 10.0f, RIARMO_DB = 5.0f;
const uint32_t SALTO_PAUSA_MS = 250;
const float SALTO_MIN = 2.5f, SALTO_MAX = 7.0f;   // velocità del colpo, unità/s (al limite e a 20 dB oltre)
const float SALTO_LATO = 1.6f;                    // spinta laterale casuale, unità/s
const float SALTO_GIRO = 6.0f;                    // rotazione casuale data dal colpo, rad/s

// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240;
M5Canvas tela(&CoreS3.Display);
struct V3 { float x, y, z; };                     // y verso il basso, z dentro lo schermo
// (i tipi stanno qui in cima perché Arduino scrive i prototipi delle funzioni prima del codice)
enum Forma { SFERA, CUBO, PIRAMIDE };

struct Oggetto {
  Forma forma;
  float dim;                 // raggio della sfera, mezzo lato del cubo, mezzo lato di base della piramide
  V3 p, v, w;                // posizione, velocità, velocità angolare
  V3 r[3];                   // orientamento: colonne della matrice di rotazione
  uint16_t col;
};

#line 72 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);
#line 77 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t);
#line 82 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
float casuale();
#line 84 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 operator +(V3 a, V3 b);
#line 85 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 operator -(V3 a, V3 b);
#line 86 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 operator *(V3 a, float k);
#line 87 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
float punto(V3 a, V3 b);
#line 88 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 croce(V3 a, V3 b);
#line 89 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
float lungo(V3 a);
#line 90 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 unita(V3 a);
#line 107 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void bloccoPronto(void*, void* dati, size_t n);
#line 116 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
float aggiornaFondo(float& fondo, float db, float dt);
#line 123 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
float ascolta(float dt);
#line 150 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void proietta(V3 p, int16_t& sx, int16_t& sy);
#line 157 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
uint16_t coloreStanza(float z);
#line 162 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void linea(M5Canvas& c, V3 a, V3 b, uint16_t col);
#line 169 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void lineaProfonda(M5Canvas& c, V3 a, V3 b);
#line 177 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void disegnaStanza(M5Canvas& c);
#line 223 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
V3 ruota(const Oggetto& o, V3 q);
#line 225 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
int vertici(const Oggetto& o, V3* out);
#line 231 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void preparaOggetti();
#line 242 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void integraRotazione(Oggetto& o, float dt);
#line 258 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void tocca(Oggetto& o, V3 n, float dt);
#line 268 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void passoOggetto(Oggetto& o, V3 g, float dt);
#line 295 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void salta(Oggetto& o, V3 g, float forza);
#line 305 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void disegnaSfera(M5Canvas& c, const Oggetto& o);
#line 330 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void disegnaOggetto(M5Canvas& c, const Oggetto& o);
#line 346 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void misura(float& media, uint32_t t0);
#line 350 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void disegnaDebug(M5Canvas& c, V3 g);
#line 362 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void setup();
#line 391 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
void loop();
#line 72 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaScatola\\ProvaScatola.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), GRIGIO = rgb(120, 120, 120);

uint16_t mescola(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, float t) {
  t = constrain(t, 0.0f, 1.0f);
  return rgb(r1 + (r2 - r1) * t, g1 + (g2 - g1) * t, b1 + (b2 - b1) * t);
}

float casuale() { return (float)esp_random() / 4294967295.0f * 2.0f - 1.0f; }   // -1..1

V3 operator+(V3 a, V3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
V3 operator-(V3 a, V3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
V3 operator*(V3 a, float k) { return { a.x * k, a.y * k, a.z * k }; }
float punto(V3 a, V3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
V3 croce(V3 a, V3 b) { return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; }
float lungo(V3 a) { return sqrtf(punto(a, a)); }
V3 unita(V3 a) { float l = lungo(a); return l > 1e-6f ? a * (1.0f / l) : V3{ 0, 1, 0 }; }

// ---------------------------------------------------------------- audio
static constexpr int FREQ     = 16000;
static constexpr int BLOCCO   = 64;           // pezzo che arriva dal microfono: 4 ms
static constexpr int N_ANELLO = 1024;         // ultimi 64 ms di suono
static constexpr int N_FINESTRA = 512;        // il livello si misura sugli ultimi 32 ms
// Il microfono registra di continuo blocchi piccoli: ogni blocco finito viene copiato
// nell'anello e subito rimesso in coda, dal task del microfono stesso. Il loop non aspetta mai.
int16_t blocchi[2][BLOCCO];
int16_t anello[N_ANELLO];
volatile uint32_t scritti = 0;

float fondoVol = -60, dbVol = -90, sopraDb = 0;
bool  armato = true;
uint32_t tSalto = 0;

void bloccoPronto(void*, void* dati, size_t n) {
  const int16_t* d = (const int16_t*)dati;
  uint32_t w = scritti;
  for (size_t i = 0; i < n; i++) anello[(w + i) % N_ANELLO] = d[i];
  scritti = w + n;
  CoreS3.Mic.record((int16_t*)dati, n, FREQ);
}

// il fondo segue in fretta i valori più bassi e risale piano: così si adatta al rumore della stanza
float aggiornaFondo(float& fondo, float db, float dt) {
  if (db < fondo) fondo += (db - fondo) * min(1.0f, dt * 4.0f);
  else            fondo += (db - fondo) * min(1.0f, dt * 0.05f);
  return db - fondo;
}

// restituisce la forza del colpo (0..1) quando scatta un salto, altrimenti -1
float ascolta(float dt) {
  uint32_t w = scritti;
  double somma = 0, media = 0;
  for (int i = 0; i < N_FINESTRA; i++) media += anello[(w + N_ANELLO - N_FINESTRA + i) % N_ANELLO];
  media /= N_FINESTRA;                                  // via la componente continua
  for (int i = 0; i < N_FINESTRA; i++) {
    float v = (anello[(w + N_ANELLO - N_FINESTRA + i) % N_ANELLO] - media) / 32768.0f;
    somma += v * v;
  }
  dbVol = 10.0f * log10f(somma / N_FINESTRA + 1e-12f);
  sopraDb = aggiornaFondo(fondoVol, dbVol, dt);

  if (armato && sopraDb > SALTO_DB && millis() - tSalto > SALTO_PAUSA_MS) {
    armato = false; tSalto = millis();
    return constrain((sopraDb - SALTO_DB) / 20.0f, 0.0f, 1.0f);
  }
  if (!armato && sopraDb < RIARMO_DB) armato = true;
  return -1;
}

// ---------------------------------------------------------------- proiezione
// L'occhio sta davanti al vetro, a distanza FOCALE, spostato di (occhioX, occhioY).
// Il vetro (z = 0) si proietta sempre esattamente sul bordo dello schermo, qualunque
// sia la posizione dell'occhio: è una proiezione "fuori asse", come guardare in una finestra.
float occhioX = 0, occhioY = 0;
const float SCALA = (W / 2) / BX;   // px per unità sul vetro

void proietta(V3 p, int16_t& sx, int16_t& sy) {
  float k = FOCALE / (p.z + FOCALE);
  sx = (int16_t)(W / 2 + (occhioX + (p.x - occhioX) * k) * SCALA);
  sy = (int16_t)(H / 2 + (occhioY + (p.y - occhioY) * k) * SCALA);
}

// segmento 3D con colore che sfuma con la profondità: davanti blu acceso, in fondo quasi nero
uint16_t coloreStanza(float z) {
  float t = z / PROF;
  return mescola(60, 110, 255, 8, 18, 70, t);
}

void linea(M5Canvas& c, V3 a, V3 b, uint16_t col) {
  int16_t x0, y0, x1, y1;
  proietta(a, x0, y0); proietta(b, x1, y1);
  c.drawLine(x0, y0, x1, y1, col);
}

// le linee che vanno in profondità si spezzano in tratti, ognuno col suo colore
void lineaProfonda(M5Canvas& c, V3 a, V3 b) {
  for (int k = 0; k < GRIGLIA_Z; k++) {
    float t0 = (float)k / GRIGLIA_Z, t1 = (float)(k + 1) / GRIGLIA_Z;
    V3 p = a + (b - a) * t0, q = a + (b - a) * t1;
    linea(c, p, q, coloreStanza((p.z + q.z) * 0.5f));
  }
}

void disegnaStanza(M5Canvas& c) {
  // fondo
  uint16_t fondo = coloreStanza(PROF);
  for (int i = 0; i <= GRIGLIA_X; i++) {
    float x = -BX + 2 * BX * i / GRIGLIA_X;
    linea(c, { x, -BY, PROF }, { x, BY, PROF }, fondo);
  }
  for (int j = 0; j <= GRIGLIA_Y; j++) {
    float y = -BY + 2 * BY * j / GRIGLIA_Y;
    linea(c, { -BX, y, PROF }, { BX, y, PROF }, fondo);
  }
  // linee in profondità: pavimento e soffitto, poi le due pareti
  for (int i = 0; i <= GRIGLIA_X; i++) {
    float x = -BX + 2 * BX * i / GRIGLIA_X;
    lineaProfonda(c, { x, BY, 0 }, { x, BY, PROF });
    lineaProfonda(c, { x, -BY, 0 }, { x, -BY, PROF });
  }
  for (int j = 1; j < GRIGLIA_Y; j++) {
    float y = -BY + 2 * BY * j / GRIGLIA_Y;
    lineaProfonda(c, { -BX, y, 0 }, { -BX, y, PROF });
    lineaProfonda(c, { BX, y, 0 }, { BX, y, PROF });
  }
  // anelli a profondità costante (quello sul vetro coincide col bordo dello schermo)
  for (int k = 1; k < GRIGLIA_Z; k++) {
    float z = PROF * k / GRIGLIA_Z;
    uint16_t col = coloreStanza(z);
    linea(c, { -BX, -BY, z }, { BX, -BY, z }, col);
    linea(c, { BX, -BY, z }, { BX, BY, z }, col);
    linea(c, { BX, BY, z }, { -BX, BY, z }, col);
    linea(c, { -BX, BY, z }, { -BX, -BY, z }, col);
  }
}

// ---------------------------------------------------------------- oggetti

// vertici nel riferimento dell'oggetto
const V3 VERT_CUBO[8] = { { -1, -1, -1 }, { 1, -1, -1 }, { 1, 1, -1 }, { -1, 1, -1 },
                          { -1, -1, 1 },  { 1, -1, 1 },  { 1, 1, 1 },  { -1, 1, 1 } };
const uint8_t SPIG_CUBO[12][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 4, 5 }, { 5, 6 },
                                   { 6, 7 }, { 7, 4 }, { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 } };
// piramide: base quadrata e punta; il centro è nel baricentro, quindi la base sta un po' sotto
const V3 VERT_PIR[5] = { { -1, 0.45f, -1 }, { 1, 0.45f, -1 }, { 1, 0.45f, 1 }, { -1, 0.45f, 1 }, { 0, -1.35f, 0 } };
const uint8_t SPIG_PIR[8][2] = { { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 } };

Oggetto ogg[3];

V3 ruota(const Oggetto& o, V3 q) { return o.r[0] * q.x + o.r[1] * q.y + o.r[2] * q.z; }

int vertici(const Oggetto& o, V3* out) {
  if (o.forma == CUBO)     { for (int i = 0; i < 8; i++) out[i] = ruota(o, VERT_CUBO[i] * o.dim); return 8; }
  if (o.forma == PIRAMIDE) { for (int i = 0; i < 5; i++) out[i] = ruota(o, VERT_PIR[i] * o.dim);  return 5; }
  return 0;
}

void preparaOggetti() {
  ogg[0] = { SFERA,    0.36f, { -0.8f, 0.6f, 1.2f }, { 0, 0, 0 }, { 0, 0, 0 }, {}, rgb(46, 220, 110) };
  ogg[1] = { CUBO,     0.28f, {  0.1f, 0.6f, 1.6f }, { 0, 0, 0 }, { 0, 0, 0 }, {}, rgb(235, 240, 255) };
  ogg[2] = { PIRAMIDE, 0.30f, {  0.9f, 0.6f, 1.0f }, { 0, 0, 0 }, { 0, 0, 0 }, {}, rgb(255, 110, 80) };
  for (auto& o : ogg) {
    o.r[0] = { 1, 0, 0 }; o.r[1] = { 0, 1, 0 }; o.r[2] = { 0, 0, 1 };
    o.w = { casuale() * 0.5f, casuale() * 0.5f, casuale() * 0.5f };
  }
}

// gira l'orientamento di w·dt (formula di Rodrigues sulle tre colonne), poi lo raddrizza
void integraRotazione(Oggetto& o, float dt) {
  float ang = lungo(o.w) * dt;
  if (ang > 1e-5f) {
    V3 a = unita(o.w);
    float c = cosf(ang), s = sinf(ang);
    for (int i = 0; i < 3; i++) {
      V3 v = o.r[i];
      o.r[i] = v * c + croce(a, v) * s + a * (punto(a, v) * (1 - c));
    }
  }
  o.r[0] = unita(o.r[0]);
  o.r[1] = unita(o.r[1] - o.r[0] * punto(o.r[0], o.r[1]));
  o.r[2] = croce(o.r[0], o.r[1]);
}

// urto con una parete di normale n (verso l'interno): rimbalzo, attrito e rotolamento
void tocca(Oggetto& o, V3 n, float dt) {
  float vn = punto(o.v, n);
  if (vn < 0) o.v = o.v - n * (vn * (1 + RIMBALZO));
  V3 vt = o.v - n * punto(o.v, n);
  o.v = o.v - vt * (1 - expf(-STRISCIO * dt));
  // velocità angolare di chi rotola senza strisciare: w = (n × v) / raggio
  V3 wRotola = croce(n, vt) * (1.0f / o.dim);
  o.w = o.w + (wRotola - o.w) * ROTOLA;
}

void passoOggetto(Oggetto& o, V3 g, float dt) {
  o.v = o.v + g * (GRAVITA * dt);
  o.v = o.v * expf(-ARIA * dt);
  o.p = o.p + o.v * dt;
  o.w = o.w * expf(-0.3f * dt);
  integraRotazione(o, dt);

  // ingombro sui tre assi: per la sfera è il raggio, per gli altri i vertici ruotati
  V3 lo = { -o.dim, -o.dim, -o.dim }, hi = { o.dim, o.dim, o.dim };
  V3 vs[8];
  int n = vertici(o, vs);
  if (n > 0) {
    lo = vs[0]; hi = vs[0];
    for (int i = 1; i < n; i++) {
      lo = { min(lo.x, vs[i].x), min(lo.y, vs[i].y), min(lo.z, vs[i].z) };
      hi = { max(hi.x, vs[i].x), max(hi.y, vs[i].y), max(hi.z, vs[i].z) };
    }
  }
  if (o.p.x + lo.x < -BX)  { o.p.x = -BX - lo.x;  tocca(o, { 1, 0, 0 }, dt); }
  if (o.p.x + hi.x >  BX)  { o.p.x =  BX - hi.x;  tocca(o, { -1, 0, 0 }, dt); }
  if (o.p.y + lo.y < -BY)  { o.p.y = -BY - lo.y;  tocca(o, { 0, 1, 0 }, dt); }
  if (o.p.y + hi.y >  BY)  { o.p.y =  BY - hi.y;  tocca(o, { 0, -1, 0 }, dt); }
  if (o.p.z + lo.z < 0)    { o.p.z = -lo.z;       tocca(o, { 0, 0, 1 }, dt); }    // il vetro
  if (o.p.z + hi.z > PROF) { o.p.z = PROF - hi.z; tocca(o, { 0, 0, -1 }, dt); }
}

// colpo contro la gravità, con una parte laterale e una rotazione a caso
void salta(Oggetto& o, V3 g, float forza) {
  V3 su = unita(g) * -1.0f;
  V3 lato = { casuale(), casuale(), casuale() };
  lato = lato - su * punto(lato, su);
  float vel = SALTO_MIN + (SALTO_MAX - SALTO_MIN) * forza;
  o.v = o.v + su * (vel * (0.8f + 0.4f * fabsf(casuale()))) + lato * SALTO_LATO;
  o.w = o.w + V3{ casuale(), casuale(), casuale() } * SALTO_GIRO;
}

// la sfera è fatta di 4 meridiani e 3 paralleli
void disegnaSfera(M5Canvas& c, const Oggetto& o) {
  const int SEG = 16;
  int16_t px0 = 0, py0 = 0, px, py;
  for (int m = 0; m < 4; m++) {
    float lon = PI * m / 4;
    for (int s = 0; s <= SEG; s++) {
      float a = 2 * PI * s / SEG;
      V3 q = { cosf(lon) * sinf(a), cosf(a), sinf(lon) * sinf(a) };
      proietta(o.p + ruota(o, q * o.dim), px, py);
      if (s > 0) c.drawLine(px0, py0, px, py, o.col);
      px0 = px; py0 = py;
    }
  }
  for (int l = -1; l <= 1; l++) {
    float lat = l * PI / 4, rr = cosf(lat), y = sinf(lat);
    for (int s = 0; s <= SEG; s++) {
      float a = 2 * PI * s / SEG;
      V3 q = { rr * cosf(a), y, rr * sinf(a) };
      proietta(o.p + ruota(o, q * o.dim), px, py);
      if (s > 0) c.drawLine(px0, py0, px, py, o.col);
      px0 = px; py0 = py;
    }
  }
}

void disegnaOggetto(M5Canvas& c, const Oggetto& o) {
  if (o.forma == SFERA) { disegnaSfera(c, o); return; }
  V3 vs[8];
  int16_t sx[8], sy[8];
  int n = vertici(o, vs);
  for (int i = 0; i < n; i++) proietta(o.p + vs[i], sx[i], sy[i]);
  const uint8_t (*sp)[2] = o.forma == CUBO ? SPIG_CUBO : SPIG_PIR;
  int ns = o.forma == CUBO ? 12 : 8;
  for (int e = 0; e < ns; e++) c.drawLine(sx[sp[e][0]], sy[sp[e][0]], sx[sp[e][1]], sy[sp[e][1]], o.col);
}

// ---------------------------------------------------------------- debug
bool debug = false;
float fps = 0;
float msAudio = 0, msFisica = 0, msDisegno = 0, msInvio = 0;   // tempi lisciati di ogni fase

void misura(float& media, uint32_t t0) {
  media += ((micros() - t0) / 1000.0f - media) * 0.1f;
}

void disegnaDebug(M5Canvas& c, V3 g) {
  c.setFont(&fonts::DejaVu12);
  c.setTextColor(GRIGIO, NERO);
  c.setTextDatum(textdatum_t::top_left);
  c.drawString(String(fps, 0) + " fps   " + String(dbVol, 1) + " dB   sopra " + String(sopraDb, 1)
               + (armato ? "" : "   (scattato)"), 6, 4);
  c.drawString("g " + String(g.x, 2) + " " + String(g.y, 2) + " " + String(g.z, 2), 6, 20);
  c.drawString("ms  audio " + String(msAudio, 1) + "  fisica " + String(msFisica, 1)
               + "  disegno " + String(msDisegno, 1) + "  invio " + String(msInvio, 1), 6, 36);
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);

  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(NERO);

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

  preparaOggetti();
}

void loop() {
  static uint32_t tPrima = micros();
  uint32_t ora = micros();
  float dt = constrain((ora - tPrima) / 1e6f, 0.001f, 0.05f);
  tPrima = ora;
  fps += (1.0f / dt - fps) * 0.05f;

  CoreS3.update();
  if (CoreS3.Touch.getDetail().wasClicked()) debug = !debug;

  uint32_t t0 = micros();
  float colpo = ascolta(dt);
  misura(msAudio, t0);

  // gravità nel riferimento della scatola: x a destra, y in basso, z dentro lo schermo.
  // Il device appoggiato sul tavolo dà z = +1: gli oggetti cadono verso il fondo.
  t0 = micros();
  float ax = 0, ay = 0, az = 1;
  if (CoreS3.Imu.isEnabled()) CoreS3.Imu.getAccel(&ax, &ay, &az);
  static V3 g = { 0, 1, 0 };
  V3 gLetta = { SEGNO_X * ax, SEGNO_Y * ay, SEGNO_Z * az };
  g = g + (gLetta - g) * min(1.0f, dt * 15.0f);          // toglie il tremolio del sensore

  // l'occhio sale quando la gravità tira verso il basso dello schermo: si vede di più il pavimento
  occhioX += (-PARALLASSE * g.x - occhioX) * min(1.0f, dt * 6.0f);
  occhioY += (-PARALLASSE * g.y - occhioY) * min(1.0f, dt * 6.0f);

  if (colpo >= 0) for (auto& o : ogg) salta(o, g, colpo);
  const int SOTTOPASSI = 2;
  for (int k = 0; k < SOTTOPASSI; k++)
    for (auto& o : ogg) passoOggetto(o, g, dt / SOTTOPASSI);
  misura(msFisica, t0);

  // l'occhio si sposta, quindi cambia tutta l'immagine: si ridisegna e si invia lo schermo intero
  t0 = micros();
  tela.fillScreen(NERO);
  disegnaStanza(tela);
  // dal più lontano al più vicino, così dove si sovrappongono vince quello davanti
  int ordine[3] = { 0, 1, 2 };
  for (int i = 0; i < 2; i++)
    for (int j = i + 1; j < 3; j++)
      if (ogg[ordine[j]].p.z > ogg[ordine[i]].p.z) { int t = ordine[i]; ordine[i] = ordine[j]; ordine[j] = t; }
  for (int i = 0; i < 3; i++) disegnaOggetto(tela, ogg[ordine[i]]);
  if (debug) disegnaDebug(tela, g);
  misura(msDisegno, t0);

  t0 = micros();
  tela.pushSprite(0, 0);
  misura(msInvio, t0);
}

}  // namespace scatola
