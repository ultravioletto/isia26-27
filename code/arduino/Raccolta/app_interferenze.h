// Generato da aggiorna.py a partire da InterferencePatterns/InterferencePatterns.ino: non modificare qui.
#pragma once
#include <Arduino.h>
#include <M5CoreS3.h>
#include <math.h>
namespace interferenze {
#line 1 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
// =====================================================================
//  Interference Patterns
//  Onde in un piatto circolare: sorgenti puntiformi che oscillano,
//  onde che si riflettono sul bordo e si sommano.
//
//  Fisica:    equazione delle onde su una griglia 120 x 120 (differenze
//             finite), parete riflettente sul cerchio, un po' di
//             smorzamento. Ogni sorgente spinge la superficie su e giù
//             con un seno; dove creste e creste si incontrano l'onda
//             cresce, dove cresta incontra ventre si annulla.
//  Sorgenti:  due palline che rotolano sul fondo del piatto. Inclinare
//             la scheda (pan / tilt, dall'accelerometro) è gravità:
//             accelerano, hanno attrito e rimbalzano sul bordo. Hanno
//             attriti un po' diversi, così non restano appaiate.
//             Muovendosi lasciano onde più fitte davanti e più larghe
//             dietro (effetto Doppler).
//  Viste:     "onda"    = la superficie in questo istante
//             "energia" = media nel tempo di u², mostra la figura ferma
//                         di interferenza: le linee nodali restano scure.
//  Tocco:     dito sul piatto o a destra = velocità della simulazione
//             con l'altezza del dito: in alto 50 passi/s, in basso 5.
//             La barra a destra mostra la velocità attuale.
//             Tocco nella colonna di sinistra = cambia vista.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
// =====================================================================


// ---------------------------------------------------------------- parametri
const float VEL2      = 0.25f;    // (velocità · dt / dx)², stabile sotto 0.5
const float SMORZA    = 0.0015f;  // perdita per passo: senza, le sorgenti farebbero crescere l'onda all'infinito
const float PERIODO   = 18.0f;    // passi per oscillazione: lunghezza d'onda ≈ 9 celle
const float SPINTA    = 1.0f;     // ampiezza delle sorgenti
const float PASSI_MIN = 5.0f, PASSI_MAX = 50.0f;   // velocità della simulazione, scelta con il dito:
                                        // a 5 passi/s un'oscillazione dura 3.6 s, a 50 passi/s 0.36 s.
                                        // Tra un passo e l'altro il disegno sfuma, così il movimento resta fluido.
const float MEDIA     = 0.01f;    // per passo: la vista "energia" fa la media su ~100 passi, circa 5 oscillazioni

// sorgenti che rotolano: se vanno dalla parte sbagliata, inverti il segno
const float SEGNO_X = -1.0f, SEGNO_Y = 1.0f;     // come la livella dell'Esploratore
const float ZONA_MORTA = 0.04f;   // g: sotto questa inclinazione la scheda conta come in piano
const float GRAVITA    = 120.0f;  // celle/s² per 1 g di inclinazione (il piatto è largo 116 celle)
const float ATTRITO[]  = { 1.0f, 1.6f };   // smorzamento della velocità di ogni sorgente, 1/s
const float RIMBALZO   = 0.6f;    // velocità conservata nell'urto con il bordo

// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240;
static constexpr int N = 120, SCALA = 2, L = N * SCALA;   // griglia e lato del piatto a schermo
static constexpr int X0 = (W - L) / 2;
static constexpr float RAGGIO = N / 2 - 2;                // margine: i vicini delle celle interne esistono sempre
static constexpr float RAGGIO_SORGENTI = RAGGIO - 3;      // dove le sorgenti toccano il bordo
M5Canvas tela(&CoreS3.Display);

#line 57 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b);
#line 80 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void prepara();
#line 93 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
int cella(float sx, float sy);
#line 100 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void simula();
#line 117 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
float senzaZonaMorta(float g);
#line 123 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void muoviSorgenti(float dt);
#line 150 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void disegna(float f);
#line 181 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void scriviStato();
#line 189 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void disegnaVelocita();
#line 202 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void setup();
#line 224 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
void loop();
#line 57 "C:\\Users\\admin\\Documents\\LAVORI\\ISIA\\isia 26 - 27\\CODE\\ARDUINO\\InterferencePatterns\\InterferencePatterns.ino"
constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), GRIGIO = rgb(120, 120, 120);
uint16_t tinta[256];                                      // luminosità -> colore, byte già scambiati

// ---------------------------------------------------------------- campo
float* u;           // superficie adesso
float* uPrima;      // superficie al passo precedente (viene sovrascritta con il passo successivo)
float* energia;     // media di u² nel tempo
uint8_t* dentro;    // 1 = acqua, 0 = fuori dal piatto
float fase = 0;

// posizione in celle, rispetto al centro del piatto
struct Sorgente { float x, y, vx, vy; };
static constexpr int N_SORGENTI = 2;
Sorgente sorgenti[N_SORGENTI] = { { -8, 0, 0, 0 }, { 8, 0, 0, 0 } };

float passiAlSecondo = PASSI_MIN;
bool vistaEnergia = false;
float scalaOnda = 1, scalaEnergia = 1;    // guadagno automatico del disegno
float fps = 0;

void prepara() {
  u       = (float*)calloc(N * N, sizeof(float));
  uPrima  = (float*)calloc(N * N, sizeof(float));
  energia = (float*)calloc(N * N, sizeof(float));
  dentro  = (uint8_t*)malloc(N * N);
  for (int y = 0; y < N; y++)
    for (int x = 0; x < N; x++) {
      float dx = x - N / 2 + 0.5f, dy = y - N / 2 + 0.5f;
      dentro[y * N + x] = dx * dx + dy * dy < RAGGIO * RAGGIO;
    }
}

// cella della griglia sotto un punto (sx, sy rispetto al centro del piatto)
int cella(float sx, float sy) {
  int x = constrain((int)lroundf(sx + N / 2 - 0.5f), 1, N - 2);
  int y = constrain((int)lroundf(sy + N / 2 - 0.5f), 1, N - 2);
  return y * N + x;
}

// un passo dell'equazione delle onde: u' = 2u - u_prima + c²·laplaciano(u)
void simula() {
  for (int y = 1; y < N - 1; y++)
    for (int x = 1; x < N - 1; x++) {
      int i = y * N + x;
      if (!dentro[i]) continue;                  // fuori resta 0: il bordo riflette
      float lap = u[i - 1] + u[i + 1] + u[i - N] + u[i + N] - 4 * u[i];
      uPrima[i] = (2 * u[i] - uPrima[i] + VEL2 * lap) * (1 - SMORZA);
    }
  float s = SPINTA * sinf(fase);
  for (int k = 0; k < N_SORGENTI; k++) uPrima[cella(sorgenti[k].x, sorgenti[k].y)] = s;
  float* t = u; u = uPrima; uPrima = t;
  // media a ogni passo, non a ogni frame: così non dipende da quanti passi entrano in un frame
  for (int i = 0; i < N * N; i++) energia[i] += (u[i] * u[i] - energia[i]) * MEDIA;
  fase = fmodf(fase + TWO_PI / PERIODO, TWO_PI);
}

// ---------------------------------------------------------------- sorgenti che rotolano
float senzaZonaMorta(float g) {
  g = constrain(g, -1.0f, 1.0f);
  return fabsf(g) < ZONA_MORTA ? 0 : g - copysignf(ZONA_MORTA, g);
}

// in tempo reale, non al ritmo lento della simulazione: le sorgenti rispondono subito alla mano
void muoviSorgenti(float dt) {
  float ax = 0, ay = 0, az = 1;
  if (CoreS3.Imu.isEnabled()) CoreS3.Imu.getAccel(&ax, &ay, &az);
  float gx = senzaZonaMorta(SEGNO_X * ax), gy = senzaZonaMorta(SEGNO_Y * ay);

  for (int k = 0; k < N_SORGENTI; k++) {
    Sorgente& s = sorgenti[k];
    s.vx += gx * GRAVITA * dt;
    s.vy += gy * GRAVITA * dt;
    float frena = expf(-ATTRITO[k] * dt);
    s.vx *= frena; s.vy *= frena;
    s.x += s.vx * dt; s.y += s.vy * dt;

    // rimbalzo sul cerchio: si riporta dentro e si ribalta la velocità verso il bordo
    float r = sqrtf(s.x * s.x + s.y * s.y);
    if (r > RAGGIO_SORGENTI) {
      float nx = s.x / r, ny = s.y / r;
      s.x = nx * RAGGIO_SORGENTI; s.y = ny * RAGGIO_SORGENTI;
      float vn = s.vx * nx + s.vy * ny;
      if (vn > 0) { s.vx -= (1 + RIMBALZO) * vn * nx; s.vy -= (1 + RIMBALZO) * vn * ny; }
    }
  }
}

// ---------------------------------------------------------------- disegno
// f = frazione del passo successivo già trascorsa (0..1): l'onda si disegna a metà strada
// tra il passo precedente (uPrima, dopo lo scambio) e quello attuale
void disegna(float f) {
  auto valore = [f](int i) {
    return vistaEnergia ? energia[i] : fabsf(uPrima[i] + (u[i] - uPrima[i]) * f);
  };
  float massimo = 0;
  for (int i = 0; i < N * N; i++) {
    float v = valore(i);
    if (v > massimo) massimo = v;
  }
  // il guadagno insegue il massimo: sale in fretta, scende piano
  float& scala = vistaEnergia ? scalaEnergia : scalaOnda;
  scala += (massimo - scala) * (massimo > scala ? 0.3f : 0.02f);
  float k = 255.0f / max(scala, 1e-6f);

  uint16_t* buf = (uint16_t*)tela.getBuffer();
  for (int y = 0; y < N; y++) {
    uint16_t* riga = buf + y * SCALA * L;
    for (int x = 0; x < N; x++) {
      int i = y * N + x;
      float v = valore(i);
      uint16_t c = tinta[min((int)(v * k), 255)];
      riga[x * 2] = riga[x * 2 + 1] = c;
    }
    memcpy(riga + L, riga, L * sizeof(uint16_t));   // ogni cella è 2 x 2 pixel
  }
  tela.drawCircle(L / 2, L / 2, RAGGIO * SCALA, rgb(60, 58, 54));
  for (int k = 0; k < N_SORGENTI; k++)
    tela.drawCircle(L / 2 + sorgenti[k].x * SCALA, L / 2 + sorgenti[k].y * SCALA, 4, rgb(255, 120, 90));
  tela.pushSprite(X0, 0);
}

void scriviStato() {
  CoreS3.Display.fillRect(0, 0, X0, H - 14, NERO);
  CoreS3.Display.setTextColor(GRIGIO, NERO);
  CoreS3.Display.drawString(vistaEnergia ? "energia" : "onda", 4, 4);
}

// barra della velocità nella colonna di destra: il segno sta all'altezza del dito
static constexpr int BARRA_X = X0 + L + 16, BARRA_Y0 = 20, BARRA_Y1 = H - 20;
void disegnaVelocita() {
  CoreS3.Display.fillRect(X0 + L, 0, W - X0 - L, H, NERO);
  CoreS3.Display.drawFastVLine(BARRA_X + 3, BARRA_Y0, BARRA_Y1 - BARRA_Y0, rgb(60, 58, 54));
  float t = (passiAlSecondo - PASSI_MIN) / (PASSI_MAX - PASSI_MIN);
  int y = BARRA_Y1 - t * (BARRA_Y1 - BARRA_Y0);
  CoreS3.Display.fillRect(BARRA_X - 2, y - 2, 11, 5, rgb(255, 120, 90));
  char s[8];
  snprintf(s, sizeof(s), "%.0f", passiAlSecondo);
  CoreS3.Display.setTextColor(GRIGIO, NERO);
  CoreS3.Display.drawString(s, X0 + L + 6, y < H / 2 ? y + 8 : y - 16);
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);

  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(NERO);

  for (int i = 0; i < 256; i++) {
    float g = powf(i / 255.0f, 1.2f);
    tinta[i] = __builtin_bswap16(rgb(255 * g, 245 * g, 225 * g));
  }

  tela.setColorDepth(16);
  tela.setPsram(false);
  if (!tela.createSprite(L, L)) { tela.setPsram(true); tela.createSprite(L, L); }

  prepara();
  scriviStato();
  disegnaVelocita();
}

void loop() {
  static uint32_t tPrima = micros();
  uint32_t ora = micros();
  float dt = (ora - tPrima) / 1e6f;
  if (dt > 0) fps += (1.0f / dt - fps) * 0.05f;
  tPrima = ora;
  dt = constrain(dt, 0.0f, 0.1f);

  CoreS3.update();
  auto t = CoreS3.Touch.getDetail();
  if (t.x < X0) {                                         // colonna di sinistra: vista
    if (t.wasClicked()) { vistaEnergia = !vistaEnergia; scriviStato(); }
  } else if (t.isPressed()) {                             // altrove: velocità con l'altezza del dito
    float a = 1 - constrain((float)(t.y - BARRA_Y0) / (BARRA_Y1 - BARRA_Y0), 0.0f, 1.0f);
    float p = PASSI_MIN + a * (PASSI_MAX - PASSI_MIN);
    if (fabsf(p - passiAlSecondo) >= 0.5f) { passiAlSecondo = p; disegnaVelocita(); }
  }

  muoviSorgenti(dt);

  static float daFare = 0;                    // passi accumulati, anche frazioni
  daFare = min(daFare + dt * passiAlSecondo, 8.0f);
  while (daFare >= 1) { simula(); daFare -= 1; }
  disegna(daFare);

  static uint32_t ultimoTesto = 0;
  if (millis() - ultimoTesto > 1000) {
    ultimoTesto = millis();
    char s[16];
    snprintf(s, sizeof(s), "%2.0f fps", fps);
    CoreS3.Display.setTextColor(GRIGIO, NERO);
    CoreS3.Display.drawString(s, 4, H - 12);
  }
}

}  // namespace interferenze
