// Generato da aggiorna.py a partire da ProvaCimatica/ProvaCimatica.ino: non modificare qui.
#pragma once
#include <Arduino.h>
#include <M5CoreS3.h>
#include <math.h>
namespace cimatica {
#line 1 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
// =====================================================================
//  Prova Cimatica
//  I modi di vibrazione di una membrana circolare, come l'acqua in un
//  piatto che vibra con il suono.
//
//  Forma:     ogni modo ha m diametri nodali (raggi scuri) e n cerchi
//             nodali (anelli). Il profilo radiale è la funzione di
//             Bessel J_m, l'angolo è cos(m·θ).
//  Tempo:     un'onda stazionaria è forma fissa × cos(ωt): la forma si
//             calcola una volta sola all'avvio (lenta), a ogni frame si
//             moltiplica solo per un numero. Due modi mescolati, ognuno
//             con la sua frequenza, danno le figure asimmetriche.
//  Luce:      chiaro dove la superficie si muove (ventri), scuro sulle
//             linee nodali.
//  Tocco:     passa alla combinazione di modi successiva.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
// =====================================================================


// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240;
static constexpr int L = 240, X0 = (W - L) / 2;     // il piatto: quadrato centrale 240 x 240
M5Canvas tela(&CoreS3.Display);

#line 29 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);
#line 60 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
void calcolaForma(int8_t* f, int m, int n);
#line 72 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
void preparaCombinazione();
#line 95 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
void disegna(float t);
#line 108 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
void setup();
#line 131 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
void loop();
#line 29 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\ProvaCimatica\\ProvaCimatica.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), GRIGIO = rgb(120, 120, 120);

// luminosità 0..255 -> colore già con i byte scambiati, pronto per il buffer dello sprite
uint16_t tinta[256];

// ---------------------------------------------------------------- modi
// zeri della funzione di Bessel J_m: riga = m (0..4), colonna = n (1..3)
const float ZERI[5][3] = {
  { 2.405f,  5.520f,  8.654f }, { 3.832f,  7.016f, 10.173f }, { 5.136f,  8.417f, 11.620f },
  { 6.380f,  9.761f, 13.015f }, { 7.588f, 11.065f, 14.373f }
};

struct Combinazione { int mA, nA, mB, nB; float misto; };
const Combinazione COMBINAZIONI[] = {
  { 4, 1, 0, 1, 0.0f  },   // 8 raggi, puro
  { 4, 1, 1, 3, 0.6f  },   // raggi e anelli che si contendono il piatto
  { 2, 1, 0, 2, 0.5f  },
  { 3, 2, 1, 1, 0.7f  },   // la stella si sposta di lato
  { 1, 2, 0, 3, 0.5f  },
  { 4, 2, 2, 1, 0.6f  },
};
const int N_COMB = sizeof(COMBINAZIONI) / sizeof(COMBINAZIONI[0]);
int comb = 0;

int8_t* formaA;     // forma del modo, -127..127, un byte per pixel
int8_t* formaB;
float frqA, frqB;   // Hz: in una membrana la frequenza cresce con lo zero di Bessel

void calcolaForma(int8_t* f, int m, int n) {
  float k = ZERI[m][n - 1];
  float scala = 127.0f / (m == 0 ? 1.0f : 0.6f);   // massimo di J_0 = 1, degli altri circa 0.58
  for (int y = 0; y < L; y++)
    for (int x = 0; x < L; x++) {
      float dx = (x - L / 2 + 0.5f) / (L / 2), dy = (y - L / 2 + 0.5f) / (L / 2);
      float r = sqrtf(dx * dx + dy * dy);
      float v = r > 1 ? 0 : jnf(m, k * r) * cosf(m * atan2f(dy, dx));
      f[y * L + x] = (int8_t)constrain((int)(v * scala), -127, 127);
    }
}

void preparaCombinazione() {
  const Combinazione& c = COMBINAZIONI[comb];
  CoreS3.Display.fillRect(0, 0, X0, H, NERO);
  CoreS3.Display.setTextColor(GRIGIO, NERO);
  CoreS3.Display.drawString("calcolo...", 4, 4);
  calcolaForma(formaA, c.mA, c.nA);
  calcolaForma(formaB, c.mB, c.nB);
  frqA = 0.12f * ZERI[c.mA][c.nA - 1];
  frqB = 0.12f * ZERI[c.mB][c.nB - 1];

  CoreS3.Display.fillRect(0, 0, X0, H, NERO);
  char s[16];
  snprintf(s, sizeof(s), "m%d n%d", c.mA, c.nA);
  CoreS3.Display.drawString(s, 4, 4);
  if (c.misto > 0) {
    snprintf(s, sizeof(s), "+m%d n%d", c.mB, c.nB);
    CoreS3.Display.drawString(s, 4, 16);
  }
  snprintf(s, sizeof(s), "%d/%d", comb + 1, N_COMB);
  CoreS3.Display.drawString(s, 4, H - 12);
}

// ---------------------------------------------------------------- disegno
void disegna(float t) {
  const Combinazione& c = COMBINAZIONI[comb];
  int a = (int)(128 * cosf(TWO_PI * frqA * t));
  int b = (int)(128 * c.misto * cosf(TWO_PI * frqB * t));
  uint16_t* buf = (uint16_t*)tela.getBuffer();
  for (int i = 0; i < L * L; i++) {
    int s = (formaA[i] * a + formaB[i] * b) >> 7;   // -254..254
    buf[i] = tinta[min(abs(s), 255)];
  }
  tela.pushSprite(X0, 0);
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);

  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(NERO);

  // grigio caldo, come le foto stampate; la curva scurisce i toni medi e fa risaltare i ventri
  for (int i = 0; i < 256; i++) {
    float g = powf(i / 255.0f, 1.4f);
    tinta[i] = __builtin_bswap16(rgb(255 * g, 245 * g, 225 * g));
  }

  tela.setColorDepth(16);
  tela.setPsram(false);
  if (!tela.createSprite(L, L)) { tela.setPsram(true); tela.createSprite(L, L); }

  formaA = (int8_t*)malloc(L * L);
  formaB = (int8_t*)malloc(L * L);
  preparaCombinazione();
}

void loop() {
  CoreS3.update();
  if (CoreS3.Touch.getDetail().wasClicked()) {
    comb = (comb + 1) % N_COMB;
    preparaCombinazione();
  }
  disegna(millis() / 1000.0f);
}

}  // namespace cimatica
