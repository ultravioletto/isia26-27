// =====================================================================
//  Esploratore CoreS3
//  Una pagina per ogni sensore e uscita del M5Stack CoreS3.
//
//  Navigazione: flick orizzontale sullo schermo, oppure tocco sui
//  tasti della barra in alto ("<" a sinistra, ">" a destra).
//
//  Pagine: Info, Accelerometro, Giroscopio, Bussola, Luce e prossimità,
//          Camera, Microfono, Touch, Altoparlante, Alimentazione e orologio,
//          Wi-Fi, Bluetooth LE, ESP-NOW, I2C, Scheda SD
//
//  Laboratorio dei sensi (lezione 3): dalla misura all'evento, un senso
//  alla volta, con soglia, isteresi, tempo minimo, finestra, pausa e
//  filtro regolabili. Si comanda dal Monitor seriale (115200 baud,
//  fine riga "Nuova riga"): scrivi ? per l'aiuto. Con "grafico" i numeri
//  vanno al Plotter seriale. Vedi la sezione "Laboratorio dei sensi".
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: M5CoreS3 (installa anche M5Unified e M5GFX)
//            La camera e il sensore di luce usano le classi del pacchetto
//            M5CoreS3; se un nome non compila, confronta con
//            File > Esempi > M5CoreS3 > camera e ltr553.
//            Wi-Fi, BLE, ESP-NOW e SD sono librerie del pacchetto schede.
// =====================================================================

#include <SD.h>          // prima di M5CoreS3.h, come chiede M5GFX
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <M5CoreS3.h>
#include <math.h>

// ---------------------------------------------------------------- base
static constexpr int W = 320, H = 240, TOP = 48;   // barra di navigazione in alto
static constexpr int TASTO = 80;                   // larghezza dei tasti < e > nella barra
M5Canvas tela(&CoreS3.Display);                      // disegno fuori schermo, 320 x 192

constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), BIANCO = rgb(255, 255, 255);
const uint16_t GRIGIO = rgb(120, 120, 120), SCURO = rgb(40, 40, 40);
const uint16_t CORALLO = rgb(255, 90, 78), VERDE = rgb(46, 194, 126);
const uint16_t AZZURRO = rgb(63, 167, 255), GIALLO = rgb(255, 210, 63);
const uint16_t VIOLA = rgb(123, 77, 255), ACQUA = rgb(32, 201, 192);
const uint16_t ARANCIO = rgb(255, 159, 28);

enum Pagina { P_INFO, P_ACCEL, P_GYRO, P_BUSSOLA, P_LUCE, P_CAMERA, P_MIC, P_TOUCH, P_SPEAKER, P_POWER,
              P_WIFI, P_BLE, P_ESPNOW, P_I2C, P_SD, N_PAGINE };
const char* NOMI[N_PAGINE] = { "Info", "Accelerometro", "Giroscopio", "Bussola", "Luce e prossimita",
                               "Camera", "Microfono", "Touch", "Altoparlante", "Alimentazione",
                               "Wi-Fi", "Bluetooth LE", "ESP-NOW", "I2C", "Scheda SD" };
int pagina = P_INFO;
int labCorrente = -1;          // laboratorio dei sensi in corso, -1 = si vedono le pagine
bool cameraOk = false, luceOk = false;

// ---------------------------------------------------------------- utilità
const char* nomeBoard() {
  switch (M5.getBoard()) {
    case m5::board_t::board_M5StackCoreS3: return "M5Stack CoreS3";
    case m5::board_t::board_M5StackCore2:  return "M5Stack Core2";
    default:                               return "altra scheda";
  }
}
const char* nomeImu() {
  switch (CoreS3.Imu.getType()) {
    case m5::imu_t::imu_bmi270:  return "BMI270 + BMM150";
    case m5::imu_t::imu_mpu6886: return "MPU6886";
    default:                     return "assente o sconosciuta";
  }
}
const char* nomePmic() {
  switch (CoreS3.Power.getType()) {
    case m5::Power_Class::pmic_axp2101: return "AXP2101";
    case m5::Power_Class::pmic_axp192:  return "AXP192";
    default:                            return "altro";
  }
}

void testo(M5Canvas& c, int x, int y, const String& s, uint16_t col = BIANCO) {
  c.setTextDatum(textdatum_t::top_left);
  c.setTextColor(col, NERO);
  c.drawString(s, x, y);
}

void messaggio(M5Canvas& c, const char* s) {
  c.setFont(&fonts::DejaVu18);
  c.setTextDatum(textdatum_t::middle_center);
  c.setTextColor(GRIGIO, NERO);
  c.drawString(s, W / 2, (H - TOP) / 2);
}

// barra orizzontale con lo zero al centro, per valori in [-vmax, vmax]
void barraBipolare(M5Canvas& c, int x, int y, int w, int h, float v, float vmax,
                   uint16_t col, const char* nome, const char* unita) {
  c.drawRect(x, y, w, h, GRIGIO);
  int cx = x + w / 2;
  c.drawFastVLine(cx, y, h, GRIGIO);
  float f = constrain(v / vmax, -1.0f, 1.0f);
  int len = (int)(f * (w / 2 - 2));
  if (len >= 0) c.fillRect(cx + 1, y + 2, len, h - 4, col);
  else          c.fillRect(cx + len, y + 2, -len, h - 4, col);
  c.setFont(&fonts::DejaVu12);
  c.setTextColor(BIANCO, NERO);
  c.setTextDatum(textdatum_t::bottom_left);
  c.drawString(nome, x, y - 2);
  c.setTextDatum(textdatum_t::bottom_right);
  c.drawString(String(v, 2) + " " + unita, x + w, y - 2);
}

// barra orizzontale da zero a vmax
void barra(M5Canvas& c, int x, int y, int w, int h, float f, uint16_t col) {
  c.drawRect(x, y, w, h, GRIGIO);
  int len = (int)(constrain(f, 0.0f, 1.0f) * (w - 4));
  if (len > 0) c.fillRect(x + 2, y + 2, len, h - 4, col);
}

void disegnaBarraNavigazione() {
  auto& d = CoreS3.Display;
  d.fillRect(0, 0, W, TOP, SCURO);
  d.fillRoundRect(4, 4, TASTO - 8, TOP - 8, 6, GRIGIO);
  d.fillRoundRect(W - TASTO + 4, 4, TASTO - 8, TOP - 8, 6, GRIGIO);
  d.setFont(&fonts::DejaVu24);
  d.setTextColor(BIANCO, GRIGIO);
  d.setTextDatum(textdatum_t::middle_center);
  d.drawString("<", TASTO / 2, TOP / 2);
  d.drawString(">", W - TASTO / 2, TOP / 2);
  d.setFont(&fonts::DejaVu12);
  d.setTextColor(BIANCO, SCURO);
  if (labCorrente >= 0) {
    d.drawString(String("Lab ") + nomeLabCorrente(), W / 2, TOP / 2 - 8);
    d.setTextColor(GRIGIO, SCURO);
    d.drawString("< o > per uscire", W / 2, TOP / 2 + 9);
    return;
  }
  d.drawString(NOMI[pagina], W / 2, TOP / 2 - 8);
  d.setTextColor(GRIGIO, SCURO);
  d.drawString(String(pagina + 1) + "/" + String(N_PAGINE), W / 2, TOP / 2 + 9);
}

// microfono e altoparlante condividono l'I2S: si accende solo quello che serve
void entraPagina(int p) {
  if (p == P_MIC)          { CoreS3.Speaker.end(); CoreS3.Mic.begin(); }
  else if (p == P_SPEAKER) { CoreS3.Mic.end();     CoreS3.Speaker.begin(); }
  else                     { CoreS3.Mic.end(); }
  if (p == P_ESPNOW) avviaEspNow();
}

void cambiaPagina(int delta) {
  pagina = (pagina + delta + N_PAGINE) % N_PAGINE;
  entraPagina(pagina);
  CoreS3.Display.fillScreen(NERO);
  disegnaBarraNavigazione();
}

// ---------------------------------------------------------------- pagine
void paginaInfo(M5Canvas& c) {
  c.setFont(&fonts::DejaVu12);
  int y = 4, passo = 14;
  uint64_t mac = ESP.getEfuseMac();
  char smac[24];
  snprintf(smac, sizeof(smac), "%02X:%02X:%02X:%02X:%02X:%02X",
           (uint8_t)(mac), (uint8_t)(mac >> 8), (uint8_t)(mac >> 16),
           (uint8_t)(mac >> 24), (uint8_t)(mac >> 32), (uint8_t)(mac >> 40));
  testo(c, 10, y, String("scheda   ") + nomeBoard(), GIALLO);       y += passo;
  testo(c, 10, y, String("chip     ") + ESP.getChipModel() + " rev " + ESP.getChipRevision()
                  + ", " + ESP.getChipCores() + " core, " + ESP.getCpuFreqMHz() + " MHz"); y += passo;
  testo(c, 10, y, String("flash    ") + (ESP.getFlashChipSize() / (1024 * 1024)) + " MB"); y += passo;
  testo(c, 10, y, String("psram    ") + (ESP.getPsramSize() / (1024 * 1024)) + " MB, liberi "
                  + (ESP.getFreePsram() / 1024) + " kB");                                 y += passo;
  testo(c, 10, y, String("heap     ") + (ESP.getFreeHeap() / 1024) + " kB liberi");       y += passo;
  testo(c, 10, y, String("imu      ") + nomeImu());                                        y += passo;
  testo(c, 10, y, String("pmic     ") + nomePmic());                                       y += passo;
  testo(c, 10, y, String("camera   ") + (cameraOk ? "ok" : "non inizializzata"));          y += passo;
  testo(c, 10, y, String("luce     ") + (luceOk ? "LTR553 ok" : "non inizializzato"));     y += passo;
  testo(c, 10, y, String("mac      ") + smac, ACQUA);                                      y += passo;
  testo(c, 10, y, String("sdk      ") + ESP.getSdkVersion());                              y += passo;
  testo(c, 10, y, String("temp     ") + String(temperatureRead(), 1) + " C (interna al chip)"); y += passo;
  testo(c, 10, y, String("acceso da ") + (millis() / 1000) + " s", GRIGIO);
}

void paginaAccel(M5Canvas& c) {
  float ax, ay, az;
  if (!CoreS3.Imu.isEnabled() || !CoreS3.Imu.getAccel(&ax, &ay, &az)) { messaggio(c, "IMU non disponibile"); return; }

  barraBipolare(c, 16, 22, 180, 16, ax, 2.0f, CORALLO, "X", "g");
  barraBipolare(c, 16, 66, 180, 16, ay, 2.0f, VERDE,   "Y", "g");
  barraBipolare(c, 16, 110, 180, 16, az, 2.0f, AZZURRO, "Z", "g");

  // livella a bolla: proiezione della gravità sul piano dello schermo
  // (se la bolla si muove al contrario, inverti i segni di bx o by)
  int cx = 258, cy = 74, r = 44;
  c.drawCircle(cx, cy, r, GRIGIO);
  c.drawCircle(cx, cy, r / 2, SCURO);
  c.drawFastHLine(cx - r, cy, 2 * r, SCURO);
  c.drawFastVLine(cx, cy - r, 2 * r, SCURO);
  float bx = constrain(-ax, -1.0f, 1.0f), by = constrain(ay, -1.0f, 1.0f);
  c.fillCircle(cx + (int)(bx * (r - 7)), cy + (int)(by * (r - 7)), 7, GIALLO);

  // modulo, inclinazione e picco di scossa (utile per tarare le soglie)
  float mod = sqrtf(ax * ax + ay * ay + az * az);
  static float picco = 0; static uint32_t tPicco = 0;
  float scarto = fabsf(mod - 1.0f);
  if (scarto > picco) { picco = scarto; tPicco = millis(); }
  if (millis() - tPicco > 1500) picco *= 0.95f;
  float becch = degrees(atan2f(ax, az)), rollio = degrees(atan2f(ay, az));

  c.setFont(&fonts::DejaVu12);
  testo(c, 16, 134, "modulo " + String(mod, 2) + " g");
  testo(c, 16, 151, "beccheggio " + String(becch, 0) + "  rollio " + String(rollio, 0) + " gradi");
  testo(c, 16, 168, "picco scossa " + String(picco, 2) + " g", picco > 1.5f ? CORALLO : GRIGIO);
  barra(c, 200, 170, 110, 12, picco / 3.0f, picco > 1.5f ? CORALLO : GRIGIO);
}

void paginaGyro(M5Canvas& c) {
  float gx, gy, gz;
  if (!CoreS3.Imu.isEnabled() || !CoreS3.Imu.getGyro(&gx, &gy, &gz)) { messaggio(c, "IMU non disponibile"); return; }

  barraBipolare(c, 16, 22, 288, 18, gx, 500.0f, CORALLO, "X", "gradi/s");
  barraBipolare(c, 16, 68, 288, 18, gy, 500.0f, VERDE,   "Y", "gradi/s");
  barraBipolare(c, 16, 114, 288, 18, gz, 500.0f, AZZURRO, "Z", "gradi/s");

  // angolo integrato sull'asse Z: deriva lentamente, ed è proprio questo che si vede
  static float angolo = 0; static uint32_t ultimo = millis();
  uint32_t ora = millis();
  angolo += gz * (ora - ultimo) / 1000.0f;
  ultimo = ora;
  c.setFont(&fonts::DejaVu12);
  testo(c, 16, 146, "rotazione integrata su Z " + String(angolo, 1) + " gradi", GRIGIO);
  testo(c, 16, 164, "ferma: i valori restano vicini a zero", GRIGIO);
}

void paginaBussola(M5Canvas& c) {
  float mx, my, mz;
  if (!CoreS3.Imu.isEnabled() || !CoreS3.Imu.getMag(&mx, &my, &mz)) { messaggio(c, "magnetometro non disponibile"); return; }

  // calibrazione continua: ruotando il dispositivo si raccolgono minimo e massimo
  static float mn[3] = {  1e9f,  1e9f,  1e9f }, mxv[3] = { -1e9f, -1e9f, -1e9f };
  float m[3] = { mx, my, mz };
  for (int i = 0; i < 3; i++) { if (m[i] < mn[i]) mn[i] = m[i]; if (m[i] > mxv[i]) mxv[i] = m[i]; }
  float cx_ = (mn[0] + mxv[0]) / 2, cy_ = (mn[1] + mxv[1]) / 2;
  float hx = mx - cx_, hy = my - cy_;
  float rotta = degrees(atan2f(hy, hx));
  if (rotta < 0) rotta += 360;
  bool calibrata = (mxv[0] - mn[0]) > 20 && (mxv[1] - mn[1]) > 20;

  // rosa dei venti
  int cx = 110, cy = 94, r = 70;
  c.drawCircle(cx, cy, r, GRIGIO);
  c.drawCircle(cx, cy, r - 1, GRIGIO);
  c.setFont(&fonts::DejaVu12);
  c.setTextColor(BIANCO, NERO);
  c.setTextDatum(textdatum_t::middle_center);
  c.drawString("N", cx, cy - r - 10);
  c.drawString("S", cx, cy + r + 10);
  c.drawString("E", cx + r + 10, cy);
  c.drawString("O", cx - r - 10, cy);
  float a = radians(rotta);
  int px = cx + (int)(sinf(a) * (r - 8)), py = cy - (int)(cosf(a) * (r - 8));
  c.drawLine(cx, cy, px, py, CORALLO);
  c.drawLine(cx, cy, cx - (px - cx) / 2, cy - (py - cy) / 2, GRIGIO);
  c.fillCircle(cx, cy, 4, BIANCO);

  c.setFont(&fonts::DejaVu24);
  testo(c, 206, 30, String(rotta, 0) + " gradi", calibrata ? GIALLO : GRIGIO);
  c.setFont(&fonts::DejaVu12);
  testo(c, 206, 66,  "x " + String(mx, 1));
  testo(c, 206, 82,  "y " + String(my, 1));
  testo(c, 206, 98,  "z " + String(mz, 1));
  testo(c, 206, 118, "microtesla", GRIGIO);
  // il BMM150 segna il valore minimo (-8192 nei dati grezzi di M5Unified) quando un asse va fuori
  // scala, oltre circa 1300 microtesla: c'è un campo troppo forte, di solito un magnete vicino
  bool saturo = CoreS3.Imu.getRawData(6) == -8192 || CoreS3.Imu.getRawData(7) == -8192;
  if (saturo) {
    testo(c, 206, 146, "asse saturo:", CORALLO);
    testo(c, 206, 162, "magnete vicino?", CORALLO);
  } else {
    testo(c, 206, 146, calibrata ? "calibrata" : "ruota il", calibrata ? VERDE : ARANCIO);
    if (!calibrata) testo(c, 206, 162, "dispositivo", ARANCIO);
    else testo(c, 206, 162, "(grezza, no tilt)", GRIGIO);
  }
}

void paginaLuce(M5Canvas& c) {
  if (!luceOk) { messaggio(c, "LTR553 non inizializzato"); return; }
  uint16_t als = CoreS3.Ltr553.getAlsValue();   // luce ambiente, conteggio grezzo
  uint16_t ps  = CoreS3.Ltr553.getPsValue();    // prossimità, 0..2047, sale avvicinando la mano

  c.setFont(&fonts::DejaVu12);
  testo(c, 16, 6, "luce ambiente (ALS)");
  c.setFont(&fonts::DejaVu24);
  testo(c, 16, 22, String(als), GIALLO);
  float f = log10f(als < 1 ? 1 : als) / 4.8f;   // scala logaritmica, 65535 = pieno
  barra(c, 16, 52, 288, 16, f, GIALLO);

  c.setFont(&fonts::DejaVu12);
  testo(c, 16, 82, "prossimita (PS)");
  c.setFont(&fonts::DejaVu24);
  testo(c, 16, 98, String(ps), ACQUA);
  barra(c, 16, 128, 288, 16, ps / 2047.0f, ACQUA);

  // stati di esempio, le soglie sono da tarare nella stanza vera
  static uint16_t alsPrec = als;
  const char* stato = "";
  uint16_t col = GRIGIO;
  if (ps > 150)                       { stato = "mano vicina"; col = ACQUA; }
  else if (als < 5)                   { stato = "buio";        col = VIOLA; }
  else if (alsPrec > 20 && als < 5)   { stato = "coperto";     col = CORALLO; }
  c.setFont(&fonts::DejaVu18);
  testo(c, 16, 160, stato, col);
  alsPrec = als;
}

void paginaCamera() {
  // disegna direttamente sullo schermo: il fotogramma è già nel formato giusto
  auto& d = CoreS3.Display;
  if (!cameraOk) {
    d.setFont(&fonts::DejaVu18);
    d.setTextDatum(textdatum_t::middle_center);
    d.setTextColor(GRIGIO, NERO);
    d.drawString("camera non disponibile", W / 2, TOP + (H - TOP) / 2);
    delay(50);
    return;
  }
  static uint32_t t0 = millis(); static int frame = 0, fps = 0;
  if (CoreS3.Camera.get()) {
    d.pushImage(0, TOP, CoreS3.Camera.fb->width, CoreS3.Camera.fb->height, (uint16_t*)CoreS3.Camera.fb->buf);
    CoreS3.Camera.free();
    frame++;
    if (millis() - t0 >= 1000) { fps = frame; frame = 0; t0 = millis(); }
    d.setFont(&fonts::DejaVu12);
    d.setTextDatum(textdatum_t::top_left);
    d.setTextColor(GIALLO, NERO);
    d.drawString(String(fps) + " fps  320x240 RGB565", 6, TOP + 4);
  }
}

// due microfoni sul frontale, letti in stereo: i campioni arrivano a coppie sinistra, destra
// (se il lato risulta invertito battendo le mani di fianco, scambia CANALE_S e CANALE_D)
static constexpr size_t N_CAMPIONI = 256;            // per canale
static constexpr int CANALE_S = 0, CANALE_D = 1;
static int16_t bufMic[N_CAMPIONI * 2];

float dbfs(double somma) {                             // da somma dei quadrati a dBFS, -100..0
  float rms = sqrt(somma / N_CAMPIONI) / 32768.0f;
  return 20.0f * log10f(rms < 1e-5f ? 1e-5f : rms);
}

void formaOnda(M5Canvas& c, int canale, int cy, int amp, uint16_t col) {
  c.drawFastHLine(0, cy, W, SCURO);
  int px = 0, py = cy;
  for (size_t i = 0; i < N_CAMPIONI; i++) {
    int x = i * W / N_CAMPIONI;
    int y = cy - (int)((long)bufMic[i * 2 + canale] * amp / 32768);
    c.drawLine(px, py, x, y, col);
    px = x; py = y;
  }
}

void livelloMic(M5Canvas& c, int y, const char* nome, float db, float& tenuta, uint32_t& tTenuta, uint16_t col) {
  float livello = constrain((db + 60.0f) / 60.0f, 0.0f, 1.0f);   // -60 dBFS = 0, 0 dBFS = 1
  if (livello > tenuta) { tenuta = livello; tTenuta = millis(); }
  else if (millis() - tTenuta > 800) tenuta = max(0.0f, tenuta - 0.01f);
  testo(c, 10, y, nome, col);
  barra(c, 26, y, 210, 13, livello, livello > 0.8f ? CORALLO : col);
  c.drawFastVLine(28 + (int)(tenuta * 206), y, 13, GIALLO);
  testo(c, 242, y, String(db, 1) + " dB");
}

void paginaMic(M5Canvas& c) {
  if (!CoreS3.Mic.isEnabled()) { messaggio(c, "microfono non disponibile"); return; }
  if (!CoreS3.Mic.record(bufMic, N_CAMPIONI * 2, 16000, true)) { messaggio(c, "registrazione fallita"); return; }
  while (CoreS3.Mic.isRecording()) delay(1);

  double sommaS = 0, sommaD = 0;
  for (size_t i = 0; i < N_CAMPIONI; i++) {
    double s = bufMic[i * 2 + CANALE_S], d = bufMic[i * 2 + CANALE_D];
    sommaS += s * s; sommaD += d * d;
  }
  float dbS = dbfs(sommaS), dbD = dbfs(sommaD);

  // ritardo tra i due canali per correlazione incrociata, entro ±4 campioni (1 campione = 2,1 cm di suono)
  int ritardo = 0; double migliore = -1e30;
  for (int k = -4; k <= 4; k++) {
    double corr = 0;
    for (int i = 4; i < (int)N_CAMPIONI - 4; i++)
      corr += (double)bufMic[i * 2 + CANALE_S] * bufMic[(i + k) * 2 + CANALE_D];
    if (corr > migliore) { migliore = corr; ritardo = k; }
  }

  // bilancia lisciata: differenza di livello in dB, positiva se il suono è più forte a destra
  static float bilancia = 0;
  bool forte = max(dbS, dbD) > -45.0f;                  // sotto questa soglia è solo rumore di fondo
  bilancia += ((forte ? constrain(dbD - dbS, -6.0f, 6.0f) : 0.0f) - bilancia) * 0.2f;

  formaOnda(c, CANALE_S, 32, 26, VERDE);
  formaOnda(c, CANALE_D, 92, 26, AZZURRO);
  c.setFont(&fonts::DejaVu12);
  testo(c, 4, 2, "sinistra", VERDE);
  testo(c, 4, 62, "destra", AZZURRO);

  static float tenS = 0, tenD = 0; static uint32_t tS = 0, tD = 0;
  livelloMic(c, 124, "S", dbS, tenS, tS, VERDE);
  livelloMic(c, 140, "D", dbD, tenD, tD, AZZURRO);

  // indicatore del lato: da -6 dB (tutto a sinistra) a +6 dB (tutto a destra)
  int bx = 26, bw = 210, by = 158, cx = bx + bw / 2;
  c.drawRect(bx, by, bw, 12, GRIGIO);
  c.drawFastVLine(cx, by - 2, 16, GRIGIO);
  int xm = cx + (int)(bilancia / 6.0f * (bw / 2 - 4));
  c.fillRect(xm - 3, by + 2, 7, 8, forte ? GIALLO : SCURO);
  const char* lato = !forte ? "silenzio" : (bilancia < -1.5f ? "a sinistra" : (bilancia > 1.5f ? "a destra" : "al centro"));
  testo(c, 242, by - 1, lato, forte ? GIALLO : GRIGIO);
  testo(c, 10, 176, "diff " + String(dbD - dbS, 1) + " dB   ritardo " + String(ritardo) + " camp.   16 kHz", GRIGIO);
}

void paginaTouch(M5Canvas& c) {
  static int sx[64], sy[64]; static int n = 0, testa = 0;
  int tocchi = CoreS3.Touch.getCount();
  c.setFont(&fonts::DejaVu12);
  testo(c, 10, 6, "contatti " + String(tocchi));

  for (int i = 0; i < tocchi; i++) {
    auto t = CoreS3.Touch.getDetail(i);
    if (!t.isPressed()) continue;
    int x = t.x, y = t.y - TOP;
    sx[testa] = x; sy[testa] = y; testa = (testa + 1) % 64; if (n < 64) n++;
    c.drawCircle(x, y, 18, i == 0 ? CORALLO : AZZURRO);
    c.drawCircle(x, y, 19, i == 0 ? CORALLO : AZZURRO);
    c.fillCircle(x, y, 4, BIANCO);
    testo(c, 10, 26 + i * 18, String(i) + ": x " + String(t.x) + "  y " + String(t.y)
          + (t.isHolding() ? "  tenuto" : ""), i == 0 ? CORALLO : AZZURRO);
  }
  // scia degli ultimi punti
  for (int i = 0; i < n; i++) {
    int k = (testa - 1 - i + 64) % 64;
    c.fillCircle(sx[k], sy[k], 2, i < 8 ? BIANCO : GRIGIO);
  }
  auto t = CoreS3.Touch.getDetail();
  static String ultimo = ""; static uint32_t tUltimo = 0;
  if (t.wasClicked())  { ultimo = "tap x" + String(t.getClickCount()); tUltimo = millis(); }
  if (t.wasHold())     { ultimo = "pressione lunga"; tUltimo = millis(); }
  if (t.wasDragged())  { ultimo = "trascinato"; tUltimo = millis(); }
  if (millis() - tUltimo < 1500) testo(c, 10, 176, ultimo, GIALLO);
  else testo(c, 10, 176, "tocca, trascina, tieni premuto", GRIGIO);
}

// cinque tasti: pentatonica maggiore di do, da do4 a la4
const float NOTE[5] = { 261.63f, 293.66f, 329.63f, 392.00f, 440.00f };
const char* NOMI_NOTE[5] = { "do", "re", "mi", "sol", "la" };
int volume = 128;

void paginaSpeaker(M5Canvas& c) {
  int w = W / 5;
  static int acceso = -1; static uint32_t tAcceso = 0;
  auto t = CoreS3.Touch.getDetail();
  if (t.wasPressed() && t.y >= TOP) {
    int y = t.y - TOP;
    if (y < 120) {
      int k = constrain(t.x / w, 0, 4);
      CoreS3.Speaker.tone(NOTE[k], 250);
      acceso = k; tAcceso = millis();
    } else if (y > 140) {
      if (t.x < W / 2) volume = max(0, volume - 16); else volume = min(255, volume + 16);
      CoreS3.Speaker.setVolume(volume);
      CoreS3.Speaker.tone(NOTE[4], 60);
    }
  }
  if (millis() - tAcceso > 250) acceso = -1;
  for (int k = 0; k < 5; k++) {
    uint16_t col = (k == acceso) ? GIALLO : SCURO;
    c.fillRoundRect(k * w + 4, 8, w - 8, 110, 8, col);
    c.setFont(&fonts::DejaVu18);
    c.setTextDatum(textdatum_t::middle_center);
    c.setTextColor(k == acceso ? NERO : BIANCO, col);
    c.drawString(NOMI_NOTE[k], k * w + w / 2, 63);
  }
  c.setFont(&fonts::DejaVu12);
  testo(c, 16, 126, "volume " + String(volume) + " / 255");
  c.fillRoundRect(16, 146, 130, 40, 6, SCURO);
  c.fillRoundRect(174, 146, 130, 40, 6, SCURO);
  c.setTextDatum(textdatum_t::middle_center);
  c.setTextColor(BIANCO, SCURO);
  c.drawString("vol -", 81, 166);
  c.drawString("vol +", 239, 166);
}

void paginaPower(M5Canvas& c) {
  int livello = CoreS3.Power.getBatteryLevel();
  int mv = CoreS3.Power.getBatteryVoltage();
  bool carica = CoreS3.Power.isCharging() == m5::Power_Class::is_charging;

  // icona batteria
  c.drawRoundRect(16, 12, 120, 50, 6, BIANCO);
  c.fillRect(136, 27, 6, 20, BIANCO);
  int riemp = (int)(constrain(livello, 0, 100) / 100.0f * 112);
  c.fillRect(20, 16, riemp, 42, livello < 20 ? CORALLO : (carica ? AZZURRO : VERDE));
  c.setFont(&fonts::DejaVu24);
  testo(c, 160, 20, String(livello) + " %", livello < 20 ? CORALLO : BIANCO);
  c.setFont(&fonts::DejaVu12);
  testo(c, 160, 50, String(mv) + " mV");

  testo(c, 16, 72,  String("stato      ") + (carica ? "in carica" : "a batteria"), carica ? AZZURRO : BIANCO);
  testo(c, 16, 88,  String("gestore    ") + nomePmic());

  if (CoreS3.Rtc.isEnabled()) {
    auto dt = CoreS3.Rtc.getDateTime();
    char s[40];
    snprintf(s, sizeof(s), "%04d-%02d-%02d  %02d:%02d:%02d",
             dt.date.year, dt.date.month, dt.date.date, dt.time.hours, dt.time.minutes, dt.time.seconds);
    testo(c, 16, 110, "orologio RTC", GRIGIO);
    c.setFont(&fonts::DejaVu18);
    testo(c, 16, 126, s, GIALLO);
    c.setFont(&fonts::DejaVu12);
    testo(c, 16, 152, "va a batteria tampone: tiene l'ora da spento", GRIGIO);
  } else {
    testo(c, 16, 110, "RTC non disponibile", GRIGIO);
  }
  testo(c, 16, 170, "acceso da " + String(millis() / 1000) + " s", GRIGIO);
}

// RSSI in dBm su 0..1: -95 è il limite dell'udibile, -35 è attaccato
float forzaSegnale(int rssi) { return constrain((rssi + 95) / 60.0f, 0.0f, 1.0f); }
uint16_t coloreSegnale(int rssi) { return rssi > -60 ? VERDE : (rssi > -75 ? GIALLO : CORALLO); }

String macBreve(const uint8_t* m) {
  char s[12];
  snprintf(s, sizeof(s), "%02X:%02X:%02X", m[3], m[4], m[5]);
  return s;
}

// ---------------------------------------------------------------- Wi-Fi
// scansione asincrona ogni 4 s; le reti più forti in elenco, l'affollamento per canale sotto
static constexpr int MAX_RETI = 8;
struct Rete { String ssid; int rssi, canale; bool aperta; };
Rete reti[MAX_RETI];
int nReti = 0, nRetiTot = -1, occupazione[15];
uint32_t tScanWifi = 0;

void aggiornaWifi() {
  int n = WiFi.scanComplete();
  if (n >= 0) {
    nReti = 0; nRetiTot = n;
    memset(occupazione, 0, sizeof(occupazione));
    for (int i = 0; i < n; i++) {
      Rete r = { WiFi.SSID(i), WiFi.RSSI(i), (int)WiFi.channel(i), WiFi.encryptionType(i) == WIFI_AUTH_OPEN };
      if (r.canale >= 1 && r.canale <= 14) occupazione[r.canale]++;
      // inserimento ordinato per segnale, si tengono le MAX_RETI più forti
      int pos = nReti;
      while (pos > 0 && reti[pos - 1].rssi < r.rssi) pos--;
      if (pos < MAX_RETI) {
        for (int k = min(nReti, MAX_RETI - 1); k > pos; k--) reti[k] = reti[k - 1];
        reti[pos] = r;
        if (nReti < MAX_RETI) nReti++;
      }
    }
    WiFi.scanDelete();
    tScanWifi = millis();
  } else if (n == WIFI_SCAN_FAILED && (tScanWifi == 0 || millis() - tScanWifi > 4000)) {
    WiFi.scanNetworks(true, true);   // asincrona, anche le reti nascoste
    tScanWifi = millis();
  }
}

void paginaWifi(M5Canvas& c) {
  aggiornaWifi();
  c.setFont(&fonts::DejaVu12);
  if (nRetiTot < 0) { messaggio(c, "scansione in corso..."); return; }
  testo(c, 10, 2, "reti trovate " + String(nRetiTot) + (WiFi.scanComplete() == WIFI_SCAN_RUNNING ? "   (aggiorno)" : ""), GRIGIO);

  for (int i = 0; i < nReti; i++) {
    int y = 18 + i * 15;
    const Rete& r = reti[i];
    String nome = r.ssid.length() ? r.ssid : String("(nascosta)");
    if (nome.length() > 18) nome = nome.substring(0, 17) + ".";
    barra(c, 10, y + 2, 50, 10, forzaSegnale(r.rssi), coloreSegnale(r.rssi));
    testo(c, 66, y, nome, r.ssid.length() ? BIANCO : GRIGIO);
    testo(c, 210, y, String(r.rssi) + "  ch" + String(r.canale), GRIGIO);
    if (r.aperta) testo(c, 286, y, "aper", ARANCIO);
  }

  // affollamento dei canali 1..13: il meno affollato tra 1, 6, 11 è il candidato per ESP-NOW
  int massimo = 1;
  for (int ch = 1; ch <= 13; ch++) massimo = max(massimo, occupazione[ch]);
  int migliore = 1;
  if (occupazione[6] < occupazione[migliore]) migliore = 6;
  if (occupazione[11] < occupazione[migliore]) migliore = 11;
  int base = 170, alto = 26, w = 22;
  testo(c, 16, 140, "reti per canale    per ESP-NOW: ch " + String(migliore), VERDE);
  for (int ch = 1; ch <= 13; ch++) {
    int x = 16 + (ch - 1) * w;
    int h = occupazione[ch] * alto / massimo;
    uint16_t col = (ch == migliore) ? VERDE : ((ch == 1 || ch == 6 || ch == 11) ? AZZURRO : GRIGIO);
    c.drawFastHLine(x, base, w - 4, SCURO);
    if (h > 0) c.fillRect(x, base - h, w - 4, h, col);
    c.setTextDatum(textdatum_t::top_center);
    c.setTextColor(col, NERO);
    c.drawString(String(ch), x + (w - 4) / 2, base + 2);
  }
}

// ---------------------------------------------------------------- Bluetooth LE
// scansione attiva di 3 s, ripetuta finché si resta sulla pagina
static constexpr int MAX_DISP = 8;
struct Dispositivo { String nome, mac; int rssi; };
Dispositivo disp[MAX_DISP];
int nDisp = 0, nDispTot = -1;
BLEScan* scanBle = nullptr;
volatile bool bleFinito = false;

void fineScanBle(BLEScanResults) { bleFinito = true; }

void aggiornaBle() {
  if (!scanBle) {
    BLEDevice::init("");
    scanBle = BLEDevice::getScan();
    scanBle->setActiveScan(true);   // chiede anche il nome
    scanBle->setInterval(100);
    scanBle->setWindow(99);
    scanBle->start(3, fineScanBle, false);
    return;
  }
  if (!bleFinito) return;
  bleFinito = false;
  BLEScanResults* ris = scanBle->getResults();
  nDisp = 0; nDispTot = ris->getCount();
  for (int i = 0; i < nDispTot; i++) {
    BLEAdvertisedDevice d = ris->getDevice(i);
    Dispositivo nuovo = { d.haveName() ? d.getName() : String(""), d.getAddress().toString(), d.getRSSI() };
    int pos = nDisp;
    while (pos > 0 && disp[pos - 1].rssi < nuovo.rssi) pos--;
    if (pos < MAX_DISP) {
      for (int k = min(nDisp, MAX_DISP - 1); k > pos; k--) disp[k] = disp[k - 1];
      disp[pos] = nuovo;
      if (nDisp < MAX_DISP) nDisp++;
    }
  }
  scanBle->clearResults();
  scanBle->start(3, fineScanBle, false);
}

void paginaBle(M5Canvas& c) {
  aggiornaBle();
  c.setFont(&fonts::DejaVu12);
  if (nDispTot < 0) { messaggio(c, "scansione BLE, 3 s..."); return; }
  testo(c, 10, 2, "dispositivi " + String(nDispTot) + "   i piu vicini:", GRIGIO);
  for (int i = 0; i < nDisp; i++) {
    int y = 18 + i * 19;
    const Dispositivo& d = disp[i];
    String nome = d.nome.length() ? d.nome : String("senza nome");
    if (nome.length() > 16) nome = nome.substring(0, 15) + ".";
    barra(c, 10, y + 2, 50, 10, forzaSegnale(d.rssi), coloreSegnale(d.rssi));
    testo(c, 66, y, nome, d.nome.length() ? BIANCO : GRIGIO);
    testo(c, 196, y, d.mac.substring(9), GRIGIO);
    testo(c, 284, y, String(d.rssi), GRIGIO);
  }
  testo(c, 10, 176, "telefoni, cuffie, orologi: la stanza vista via BLE", GRIGIO);
}

// ---------------------------------------------------------------- ESP-NOW
// la radio del progetto: ogni CoreS3 con questo sketch si annuncia una volta al secondo
// in broadcast e ascolta gli altri. Servono almeno due dispositivi.
static constexpr uint8_t CANALE_ESPNOW = 1;
const uint8_t BROADCAST[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

struct __attribute__((packed)) Annuncio { char firma[4]; uint32_t contatore; uint32_t accesoMs; };

static constexpr int MAX_VICINI = 6;
struct Vicino { uint8_t mac[6]; int rssi; uint32_t ricevuti, primo, ultimo, tUltimo; };
Vicino vicini[MAX_VICINI];
int nVicini = 0;
portMUX_TYPE muxVicini = portMUX_INITIALIZER_UNLOCKED;
bool espnowOk = false;
uint32_t inviati = 0, tInvio = 0;

// gira nel task del Wi-Fi: solo copie veloci, sotto lucchetto
void riceviEspNow(const esp_now_recv_info_t* info, const uint8_t* dati, int len) {
  if (len != sizeof(Annuncio) || memcmp(dati, "ESPL", 4) != 0) return;
  Annuncio a;
  memcpy(&a, dati, sizeof(a));
  portENTER_CRITICAL(&muxVicini);
  int i = 0;
  while (i < nVicini && memcmp(vicini[i].mac, info->src_addr, 6) != 0) i++;
  if (i == nVicini && nVicini < MAX_VICINI) nVicini++;
  if (i < nVicini) {
    Vicino& v = vicini[i];
    bool nuovo = memcmp(v.mac, info->src_addr, 6) != 0 || a.contatore < v.ultimo;   // nuovo o riavviato
    if (nuovo) { memcpy(v.mac, info->src_addr, 6); v.primo = a.contatore; v.ricevuti = 0; }
    v.rssi = info->rx_ctrl->rssi;
    v.ricevuti++;
    v.ultimo = a.contatore;
    v.tUltimo = millis();
  }
  portEXIT_CRITICAL(&muxVicini);
}

void avviaEspNow() {
  esp_wifi_scan_stop();   // una scansione Wi-Fi in corso sposterebbe il canale
  esp_wifi_set_channel(CANALE_ESPNOW, WIFI_SECOND_CHAN_NONE);
  if (espnowOk) return;
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(riceviEspNow);
  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, BROADCAST, 6);
  peer.channel = 0;   // il canale corrente
  peer.ifidx = WIFI_IF_STA;
  peer.encrypt = false;
  espnowOk = esp_now_add_peer(&peer) == ESP_OK;
}

void paginaEspNow(M5Canvas& c) {
  c.setFont(&fonts::DejaVu12);
  if (!espnowOk) { messaggio(c, "ESP-NOW non avviato"); return; }

  if (millis() - tInvio >= 1000) {
    tInvio = millis();
    Annuncio a = { { 'E', 'S', 'P', 'L' }, inviati, millis() };
    if (esp_now_send(BROADCAST, (const uint8_t*)&a, sizeof(a)) == ESP_OK) inviati++;
  }

  uint8_t mio[6];
  esp_wifi_get_mac(WIFI_IF_STA, mio);
  testo(c, 10, 2, "io " + macBreve(mio) + "   canale " + String(CANALE_ESPNOW) + "   inviati " + String(inviati), ACQUA);

  Vicino copia[MAX_VICINI];
  portENTER_CRITICAL(&muxVicini);
  int n = nVicini;
  memcpy(copia, vicini, sizeof(copia));
  portEXIT_CRITICAL(&muxVicini);

  if (n == 0) {
    messaggio(c, "nessun vicino");
    testo(c, 10, 160, "carica questo sketch su un altro CoreS3", GRIGIO);
    testo(c, 10, 176, "e apri la stessa pagina", GRIGIO);
    return;
  }
  testo(c, 10, 22, "vicino      segnale           ricevuti  persi   ultimo", GRIGIO);
  for (int i = 0; i < n; i++) {
    const Vicino& v = copia[i];
    int y = 42 + i * 26;
    float eta = (millis() - v.tUltimo) / 1000.0f;
    bool vivo = eta < 5.0f;
    uint32_t attesi = v.ultimo - v.primo + 1;
    int persi = attesi > v.ricevuti ? (int)((attesi - v.ricevuti) * 100 / attesi) : 0;
    testo(c, 10, y, macBreve(v.mac), vivo ? BIANCO : GRIGIO);
    barra(c, 84, y + 2, 60, 10, forzaSegnale(v.rssi), vivo ? coloreSegnale(v.rssi) : GRIGIO);
    testo(c, 150, y, String(v.rssi), GRIGIO);
    testo(c, 186, y, String(v.ricevuti), vivo ? BIANCO : GRIGIO);
    testo(c, 230, y, String(persi) + "%", persi > 10 ? CORALLO : GRIGIO);
    testo(c, 272, y, String(eta, 1) + "s", vivo ? GRIGIO : CORALLO);
  }
}

// ---------------------------------------------------------------- I2C
// bus interno (i chip della scheda) e porta A (le unità Grove); tocca per riscandire
bool i2cIn[128], i2cEx[128];
bool i2cFatto = false;

const char* nomeI2C(uint8_t a, bool interno) {
  if (interno) switch (a) {
    case 0x10: return "BMM150 bussola";
    case 0x21: return "GC0308 camera";
    case 0x23: return "LTR553 luce";
    case 0x34: return "AXP2101 alimentaz.";
    case 0x36: return "AW88298 amplific.";
    case 0x38: return "FT6336 touch";
    case 0x40: return "ES7210 microfoni";
    case 0x51: return "BM8563 orologio";
    case 0x58: return "AW9523 espansore";
    case 0x69: return "BMI270 IMU";
  }
  switch (a) {   // unità comuni sulla porta A
    case 0x29: return "forse ToF VL53L0X";
    case 0x3C: return "forse OLED";
    case 0x44: return "forse ENV (SHT30)";
    case 0x5A: return "forse MLX90614";
    case 0x70: return "forse PaHub";
    case 0x76: case 0x77: return "forse ENV (press.)";
  }
  return "";
}

void scansioneI2C() {
  memset(i2cIn, 0, sizeof(i2cIn));
  memset(i2cEx, 0, sizeof(i2cEx));
  M5.In_I2C.scanID(i2cIn);
  if (M5.Ex_I2C.isEnabled()) {
    static bool exAvviato = false;
    if (!exAvviato) exAvviato = M5.Ex_I2C.begin();
    if (exAvviato) M5.Ex_I2C.scanID(i2cEx);
  }
  i2cFatto = true;
}

void colonnaI2C(M5Canvas& c, int x, const char* titolo, bool* trovati, bool interno) {
  testo(c, x, 2, titolo, GIALLO);
  int y = 20, n = 0;
  for (int a = 8; a < 0x78; a++) {
    if (!trovati[a]) continue;
    char s[8];
    snprintf(s, sizeof(s), "0x%02X", a);
    testo(c, x, y, s, ACQUA);
    testo(c, x + 36, y, nomeI2C(a, interno), GRIGIO);
    y += 15; n++;
    if (y > 160) break;
  }
  if (n == 0) testo(c, x, y, interno ? "nessuno" : "niente collegato", GRIGIO);
}

void paginaI2C(M5Canvas& c) {
  auto t = CoreS3.Touch.getDetail();
  if (!i2cFatto || (t.wasClicked() && t.y >= TOP)) scansioneI2C();
  c.setFont(&fonts::DejaVu12);
  colonnaI2C(c, 6, "bus interno", i2cIn, true);
  colonnaI2C(c, 166, "porta A (rossa)", i2cEx, false);
  testo(c, 6, 176, "tocca per riscandire", GRIGIO);
}

// ---------------------------------------------------------------- Scheda SD
// la microSD condivide il bus SPI con lo schermo: si legge solo all'ingresso o al tocco
bool sdFatto = false, sdOk = false;
uint8_t sdTipo = CARD_NONE;
uint64_t sdTotale = 0, sdUsato = 0;
static constexpr int MAX_VOCI = 8;
String voci[MAX_VOCI];
int nVoci = 0;

void leggiSD() {
  static bool spiAvviato = false;
  int cs = M5.getPin(m5::pin_name_t::sd_spi_cs);
  if (!spiAvviato) {
    SPI.begin(M5.getPin(m5::pin_name_t::sd_spi_sclk), M5.getPin(m5::pin_name_t::sd_spi_miso),
              M5.getPin(m5::pin_name_t::sd_spi_mosi), cs);
    spiAvviato = true;
  }
  SD.end();
  sdOk = SD.begin(cs, SPI, 25000000);
  nVoci = 0;
  if (sdOk) {
    sdTipo = SD.cardType();
    sdTotale = SD.totalBytes();
    sdUsato = SD.usedBytes();
    File radice = SD.open("/");
    File f = radice.openNextFile();
    while (f && nVoci < MAX_VOCI) {
      voci[nVoci++] = f.isDirectory() ? String(f.name()) + "/" : String(f.name()) + "  " + String((uint32_t)(f.size() / 1024)) + " kB";
      f = radice.openNextFile();
    }
  }
  sdFatto = true;
}

void paginaSD(M5Canvas& c) {
  auto t = CoreS3.Touch.getDetail();
  if (!sdFatto || (t.wasClicked() && t.y >= TOP)) leggiSD();
  c.setFont(&fonts::DejaVu12);
  if (!sdOk) {
    messaggio(c, "nessuna scheda");
    testo(c, 10, 176, "inserisci una microSD (FAT32) e tocca", GRIGIO);
    return;
  }
  const char* tipo = sdTipo == CARD_SDHC ? "SDHC" : (sdTipo == CARD_SD ? "SD" : (sdTipo == CARD_MMC ? "MMC" : "?"));
  testo(c, 10, 2, String("tipo ") + tipo + "   " + String((uint32_t)(sdTotale / (1024 * 1024))) + " MB", GIALLO);
  testo(c, 10, 18, "usati " + String((uint32_t)(sdUsato / (1024 * 1024))) + " MB");
  barra(c, 10, 36, 300, 10, sdTotale ? (float)sdUsato / sdTotale : 0, AZZURRO);
  testo(c, 10, 54, "radice:", GRIGIO);
  for (int i = 0; i < nVoci; i++) testo(c, 20, 66 + i * 14, voci[i]);
  if (nVoci == 0) testo(c, 20, 66, "vuota", GRIGIO);
  testo(c, 10, 178, "tocca per rileggere", GRIGIO);
}

// ---------------------------------------------------------------- Laboratorio dei sensi
// Dalla misura all'evento, un senso alla volta (lezione 3, PROGETTO.md §5.1).
// Ogni laboratorio legge una misura, la filtra e la passa a un riconoscitore,
// cioè a una piccola macchina a stati:
//
//   QUIETO --misura oltre la soglia--> DENTRO
//   DENTRO --esce prima della finestra, dopo il minimo--> evento --> PAUSA o QUIETO
//   DENTRO --finestra 0 e minimo raggiunto--> evento "inizio" --> ATTIVO
//   ATTIVO --esce (soglia meno isteresi)--> evento "fine" --> PAUSA o QUIETO
//   PAUSA  --passata la pausa--> QUIETO
//
// Comandi dal Monitor seriale (115200 baud, fine riga "Nuova riga"):
//   ?            aiuto e parametri del laboratorio in corso
//   lab          elenco dei laboratori
//   scossa       avvia il laboratorio "scossa" (anche: lab scossa)
//   soglia 1.2   cambia un parametro; si accetta anche la virgola
//   valori       rimette i valori di partenza, quelli del §5.1
//   azzera       azzera i conteggi
//   testo        scrive qui un evento per riga (all'avvio)
//   grafico      manda i numeri al Plotter seriale, circa 20 righe al secondo
//   fine         torna alle pagine

enum TipoLab { GENERICO, COPERTO, CAREZZA };
enum IdLab { L_SCOSSA, L_CAPOVOLTO, L_INCLINATA, L_VOCE, L_COLPO, L_BUIO, L_COPERTO, L_MANO, L_TOCCO, L_CAREZZA, N_LAB };
static constexpr int MAX_PARAM = 7;
struct Param { const char* nome; float val; const char* unita; const char* cosa; float base; };
struct Lab {
  const char* nome;      // si scrive nel Monitor per avviarlo
  const char* misura;    // cosa si misura (va sullo schermo: niente accenti)
  const char* unita;
  const char* evento;
  TipoLab tipo;
  bool sotto;            // l'evento scatta sotto la soglia (il buio) invece che sopra
  uint16_t periodo;      // ms tra due campioni; 0 = appena c'è un blocco di microfono
  uint8_t cifre;         // decimali con cui si mostra la misura
  Param p[MAX_PARAM];
};

#define P_ISTERESI(u) { "isteresi", 0, u, "si esce un'isteresi oltre la soglia, dalla parte di prima (0 = spenta)" }
#define P_MINIMO(v)   { "minimo", v, "ms", "la condizione deve durare almeno tanto" }
#define P_FINESTRA(v) { "finestra", v, "ms", "l'evento deve finire entro tanto (0 = nessun limite: evento con inizio e fine)" }
#define P_PAUSA       { "pausa", 0, "ms", "dopo un evento non ascolta per tanto" }
#define P_FILTRO(v)   { "filtro", v, "Hz", "passa-basso: liscia la misura ma la ritarda (0 = spento)" }

Lab LABS[N_LAB] = {
  { "scossa", "scarto del modulo da 1 g (IMU)", "g", "scossa", GENERICO, false, 10, 2,
    { { "soglia", 1.5f, "g", "si entra sopra questo valore; 3 g per la scossa forte" },
      P_ISTERESI("g"), P_MINIMO(0), P_FINESTRA(300), P_PAUSA, P_FILTRO(0) } },
  { "capovolto", "asse z verso il basso (IMU), 1 = schermo giu", "g", "capovolto", GENERICO, false, 10, 2,
    { { "soglia", 0.8f, "g", "si entra sopra questo valore" },
      P_ISTERESI("g"), P_MINIMO(1000), P_FINESTRA(0), P_PAUSA, P_FILTRO(0) } },
  { "inclinata", "angolo dalla posizione piatta (IMU)", "gradi", "inclinata", GENERICO, false, 10, 0,
    { { "soglia", 30, "gradi", "si entra sopra questo angolo" },
      P_ISTERESI("gradi"), P_MINIMO(0), P_FINESTRA(0), P_PAUSA, P_FILTRO(5) } },
  { "voce", "livello del microfono, -60..0 dBFS su 0..1", "", "voce vicina", GENERICO, false, 0, 2,
    { { "soglia", 0.3f, "0-1", "si entra sopra questo livello" },
      { "tetto", 0.7f, "0-1", "sopra questo livello non e' piu' voce vicina (0 = nessun tetto)" },
      P_ISTERESI("0-1"), P_MINIMO(1000), P_FINESTRA(0), P_PAUSA, P_FILTRO(0) } },
  { "colpo", "livello del microfono, -60..0 dBFS su 0..1", "", "colpo sonoro", GENERICO, false, 0, 2,
    { { "soglia", 0.8f, "0-1", "si entra sopra questo livello" },
      P_ISTERESI("0-1"), P_MINIMO(0), P_FINESTRA(300), P_PAUSA, P_FILTRO(0) } },
  { "buio", "luce ambiente (ALS), conteggi grezzi", "", "buio", GENERICO, true, 100, 0,
    { { "soglia", 5, "conteggi", "si entra sotto questo valore (il 5.1 dice 5 lux: qui sono conteggi)" },
      P_ISTERESI("conteggi"), P_MINIMO(2000), P_FINESTRA(0), P_PAUSA, P_FILTRO(0) } },
  { "coperto", "luce ambiente (ALS), conteggi grezzi", "", "coperto", COPERTO, true, 100, 0,
    { { "da", 50, "conteggi", "la luce parte sopra questo valore" },
      { "a", 5, "conteggi", "e scende sotto questo" },
      { "finestra", 500, "ms", "in meno di tanto; se ci mette di piu' e' buio che arriva piano" } } },
  { "mano", "prossimita (PS), 0..2047", "", "mano vicina", GENERICO, false, 50, 0,
    { { "soglia", 150, "conteggi", "si entra sopra questo valore" },
      P_ISTERESI("conteggi"), P_MINIMO(0), P_FINESTRA(0), P_PAUSA, P_FILTRO(0) } },
  { "tocco", "contatto sullo schermo, 1 = dito giu", "", "tocco", GENERICO, false, 10, 0,
    { { "soglia", 0.5f, "", "1 = dito sullo schermo, 0 = niente" },
      P_ISTERESI(""), P_MINIMO(0), P_FINESTRA(200), P_PAUSA, P_FILTRO(0) } },
  { "carezza", "velocita del dito sullo schermo", "px/s", "carezza", CAREZZA, false, 10, 0,
    { { "velocita", 150, "px/s", "piu' veloce di cosi' e' un trascinamento, non una carezza" },
      { "distanza", 40, "px", "strada minima del dito" },
      P_FILTRO(5) } },
};

// stato del riconoscitore e dei conteggi
enum Fase { F_QUIETO, F_DENTRO, F_ATTIVO, F_PAUSA };   // per coperto: alto, caduta, basso; per carezza: libero, contatto, fatta
int fase = F_QUIETO;
uint32_t tInizio = 0, tFinePausa = 0, tUltimoEvento = 0, tCampione = 0, tStampa = 0, tSchermo = 0;
float picco = 0, fVal = 0, ultimaMisura = 0, ultimaFiltrata = 0;
bool fPronto = false, inCorso = false, modoTesto = true;
int nEventi = 0, nScartati = 0;
String ultimo = "";
uint16_t coloreUltimo = GRIGIO;
// carezza
float distanzaDito = 0; int xPrec = 0, yPrec = 0, xPartenza = 0, yPartenza = 0;
// grafico: valori raccolti tra una riga e l'altra
float accMisura = 0, accFiltrata = 0; int nAcc = 0; bool accEvento = false;
// traccia sullo schermo: un pixel per campione
static constexpr int N_TRACCIA = W;
float traccia[N_TRACCIA]; uint8_t segni[N_TRACCIA]; int testaTraccia = 0, nTraccia = 0;
// riga in arrivo dal seriale
String riga = ""; uint32_t tCarattere = 0;

const char* nomeLabCorrente() { return labCorrente >= 0 ? LABS[labCorrente].nome : ""; }

float par(const char* nome) {
  if (labCorrente < 0) return 0;
  for (int k = 0; k < MAX_PARAM && LABS[labCorrente].p[k].nome; k++)
    if (!strcmp(LABS[labCorrente].p[k].nome, nome)) return LABS[labCorrente].p[k].val;
  return 0;
}

const char* nomeFase() {
  static const char* G[] = { "QUIETO", "DENTRO", "ATTIVO", "PAUSA" };
  static const char* C[] = { "ALTO", "CADUTA", "BASSO" };
  static const char* K[] = { "LIBERO", "CONTATTO", "FATTA" };
  TipoLab tipo = LABS[labCorrente].tipo;
  return tipo == COPERTO ? C[fase % 3] : (tipo == CAREZZA ? K[fase % 3] : G[fase % 4]);
}

uint16_t coloreFase() {
  static const uint16_t COL[] = { GRIGIO, GIALLO, VERDE, VIOLA };
  return COL[fase % 4];
}

// ---- le tre righe che scrivono gli esiti
void evento(const char* tipo, uint32_t durata) {
  const Lab& L = LABS[labCorrente];
  bool fine = tipo && !strcmp(tipo, "fine");
  if (!fine) { nEventi++; tUltimoEvento = millis(); }
  ultimo = String(L.evento) + (tipo ? String(" ") + tipo : String("")) + "  " + durata + " ms";
  coloreUltimo = fine ? GRIGIO : VERDE;
  if (!modoTesto) return;
  Serial.printf("%8.2f s  %s%s%s", millis() / 1000.0f, L.evento, tipo ? " " : "", tipo ? tipo : "");
  if (!fine) Serial.printf("  #%d", nEventi);
  Serial.printf("  durata %u ms", (unsigned)durata);
  if (L.tipo == GENERICO) Serial.printf("  picco %.*f %s", L.cifre, picco, L.p[0].unita);
  Serial.println();
}

void scarto(const char* motivo, uint32_t durata) {
  const Lab& L = LABS[labCorrente];
  nScartati++;
  ultimo = String("scartato: ") + motivo + "  " + durata + " ms";
  coloreUltimo = ARANCIO;
  if (!modoTesto) return;
  Serial.printf("%8.2f s  scartato, %s  durata %u ms", millis() / 1000.0f, motivo, (unsigned)durata);
  if (L.tipo == GENERICO) Serial.printf("  picco %.*f %s", L.cifre, picco, L.p[0].unita);
  Serial.println();
}

void dopoEvento(uint32_t t) {
  if (par("pausa") > 0) { fase = F_PAUSA; tFinePausa = t + (uint32_t)par("pausa"); }
  else fase = F_QUIETO;
}

// ---- filtro passa-basso a un polo: ogni campione si avvicina alla misura di una frazione a
float filtra(float x, float dtMs) {
  float fc = par("filtro");
  if (fc <= 0 || !fPronto) { fVal = x; fPronto = true; return x; }
  float dt = dtMs / 1000.0f, rc = 1.0f / (2.0f * PI * fc);
  fVal += dt / (rc + dt) * (x - fVal);
  return fVal;
}

// ---- misure
float livelloMicLab() {   // un blocco di 16 ms, i due microfoni insieme
  if (!CoreS3.Mic.isEnabled() || !CoreS3.Mic.record(bufMic, N_CAMPIONI * 2, 16000, true)) return -1;
  while (CoreS3.Mic.isRecording()) delay(1);
  double somma = 0;
  for (size_t i = 0; i < N_CAMPIONI * 2; i++) somma += (double)bufMic[i] * bufMic[i];
  float rms = sqrt(somma / (N_CAMPIONI * 2)) / 32768.0f;
  float db = 20.0f * log10f(rms < 1e-5f ? 1e-5f : rms);
  return constrain((db + 60.0f) / 60.0f, 0.0f, 1.0f);   // la stessa scala della pagina Microfono
}

bool leggiMisura(float& v) {
  switch (labCorrente) {
    case L_SCOSSA: case L_CAPOVOLTO: case L_INCLINATA: {
      float ax, ay, az;
      if (!CoreS3.Imu.isEnabled() || !CoreS3.Imu.getAccel(&ax, &ay, &az)) return false;
      float mod = sqrtf(ax * ax + ay * ay + az * az);
      if (labCorrente == L_SCOSSA)         v = fabsf(mod - 1.0f);   // via la gravità, come la pagina Accelerometro
      else if (labCorrente == L_CAPOVOLTO) v = -az;
      else v = degrees(acosf(constrain(az / (mod > 0.01f ? mod : 1.0f), -1.0f, 1.0f)));
      return true;
    }
    case L_VOCE: case L_COLPO: v = livelloMicLab(); return v >= 0;
    case L_BUIO: case L_COPERTO: if (!luceOk) return false; v = CoreS3.Ltr553.getAlsValue(); return true;
    case L_MANO: if (!luceOk) return false; v = CoreS3.Ltr553.getPsValue(); return true;
    case L_TOCCO: v = CoreS3.Touch.getCount() > 0 ? 1 : 0; return true;
  }
  return false;
}

// ---- la traccia per lo schermo e i valori per il Plotter
void registra(float misura, float filtrata, uint8_t segno) {
  ultimaMisura = misura; ultimaFiltrata = filtrata;
  traccia[testaTraccia] = filtrata; segni[testaTraccia] = segno;
  testaTraccia = (testaTraccia + 1) % N_TRACCIA;
  if (nTraccia < N_TRACCIA) nTraccia++;
  // tra due righe del grafico si tiene il valore più estremo, così un picco breve non si perde
  bool sotto = LABS[labCorrente].sotto;
  if (nAcc == 0) { accMisura = misura; accFiltrata = filtrata; }
  else if (sotto) { accMisura = min(accMisura, misura); accFiltrata = min(accFiltrata, filtrata); }
  else { accMisura = max(accMisura, misura); accFiltrata = max(accFiltrata, filtrata); }
  nAcc++;
  if (millis() - tUltimoEvento < 150 || inCorso) accEvento = true;
}

float altoTraccia() {
  float hi = 0;
  for (int i = 0; i < nTraccia; i++) hi = max(hi, traccia[i]);
  return hi > 0 ? hi : 1;
}

// ---- i riconoscitori
void passoGenerico(float misura, uint32_t t, float dt) {
  const Lab& L = LABS[labCorrente];
  float f = filtra(misura, dt);
  float s = par("soglia"), ist = par("isteresi"), tetto = par("tetto");
  bool dentro = L.sotto ? f < s : (f > s && (tetto <= 0 || f < tetto));
  bool fuori  = L.sotto ? f > s + ist : (f < s - ist || (tetto > 0 && f > tetto + ist));
  uint8_t segno = 0;

  if (fase == F_PAUSA && t >= tFinePausa) fase = F_QUIETO;
  if (fase == F_DENTRO || fase == F_ATTIVO) picco = L.sotto ? min(picco, f) : max(picco, f);

  if (fase == F_QUIETO && dentro) { fase = F_DENTRO; tInizio = t; picco = f; }
  else if (fase == F_DENTRO) {
    uint32_t d = t - tInizio;
    float finestra = par("finestra"), minimo = par("minimo");
    if (finestra <= 0 && d >= minimo) { evento("inizio", d); fase = F_ATTIVO; inCorso = true; segno = 1; }
    else if (fuori) {
      if (finestra > 0 && d >= minimo && d <= finestra) { evento(nullptr, d); dopoEvento(t); segno = 1; }
      else { scarto(d < minimo ? "troppo breve" : "troppo lungo", d); fase = F_QUIETO; segno = 2; }
    }
  } else if (fase == F_ATTIVO && fuori) {
    evento("fine", t - tInizio); inCorso = false; dopoEvento(t);
  }
  registra(misura, f, segno);
}

// coperto: la luce deve passare da "da" ad "a" entro la finestra
void passoCoperto(float v, uint32_t t) {
  float da = par("da"), a = par("a"), finestra = par("finestra");
  uint8_t segno = 0;
  if (fase == 0) {                                   // ALTO
    if (v <= da) { fase = 1; tInizio = t; }
  } else if (fase == 1) {                            // CADUTA
    if (v > da) fase = 0;
    else if (v < a) {
      uint32_t d = t - tInizio;
      if (d <= finestra) { evento(nullptr, d); inCorso = true; segno = 1; }
      else { scarto("troppo lento, e' buio e non coperto", d); segno = 2; }
      fase = 2; tFinePausa = t;
    }
  } else if (v > da) {                               // BASSO, finché non torna la luce
    if (inCorso) evento("fine", t - tFinePausa);
    inCorso = false; fase = 0;
  }
  registra(v, v, segno);
}

// carezza: il dito deve fare abbastanza strada senza andare troppo veloce
void passoCarezza(uint32_t t, float dt) {
  uint8_t segno = 0; float v = 0, vf = 0;
  if (CoreS3.Touch.getCount() == 0) { fase = 0; fPronto = false; distanzaDito = 0; }
  else {
    auto d = CoreS3.Touch.getDetail();
    if (fase == 0) {                                 // LIBERO: il dito è appena arrivato
      fase = 1; tInizio = t; distanzaDito = 0;
      xPartenza = xPrec = d.x; yPartenza = yPrec = d.y;
    } else {
      float passo = hypotf(d.x - xPrec, d.y - yPrec);
      xPrec = d.x; yPrec = d.y;
      v = passo / dt * 1000.0f;
      vf = filtra(v, dt);
      if (fase == 1) {                               // CONTATTO
        if (vf > par("velocita")) { scarto("troppo veloce", t - tInizio); fase = 2; segno = 2; }
        else {
          distanzaDito += passo;
          if (distanzaDito >= par("distanza")) {
            int dx = xPrec - xPartenza, dy = yPrec - yPartenza;
            const char* verso = abs(dx) > abs(dy) ? (dx > 0 ? "verso destra" : "verso sinistra")
                                                  : (dy > 0 ? "verso il basso" : "verso l'alto");
            evento(verso, t - tInizio); fase = 2; segno = 1;
          }
        }
      }                                              // FATTA: si aspetta che il dito si alzi
    }
  }
  registra(v, vf, segno);
}

void stampaGrafico() {
  if (modoTesto || nAcc == 0) return;
  const Lab& L = LABS[labCorrente];
  float ev = accEvento ? altoTraccia() : 0;
  if (L.tipo == COPERTO)
    Serial.printf("misura:%.1f,da:%.1f,a:%.1f,evento:%.1f\n", accMisura, par("da"), par("a"), ev);
  else if (L.tipo == CAREZZA)
    Serial.printf("velocita:%.1f,filtrata:%.1f,limite:%.1f,distanza:%.1f,evento:%.1f\n",
                  accMisura, accFiltrata, par("velocita"), distanzaDito, ev);
  else {
    float s = par("soglia"), ist = par("isteresi");
    Serial.printf("misura:%.3f,filtrata:%.3f,soglia:%.3f,uscita:%.3f,evento:%.3f",
                  accMisura, accFiltrata, s, L.sotto ? s + ist : s - ist, ev);
    if (par("tetto") > 0) Serial.printf(",tetto:%.3f", par("tetto"));
    Serial.println();
  }
  nAcc = 0; accEvento = false;
}

// ---- lo schermo del laboratorio
void paginaLab(M5Canvas& c) {
  const Lab& L = LABS[labCorrente];
  c.setFont(&fonts::DejaVu12);
  testo(c, 6, 2, L.misura, GRIGIO);
  c.setFont(&fonts::DejaVu24);
  String unita = L.unita[0] ? String(" ") + L.unita : String("");
  testo(c, 6, 18, String(ultimaFiltrata, (unsigned int)L.cifre) + unita, GIALLO);
  c.setFont(&fonts::DejaVu18);
  testo(c, 206, 22, nomeFase(), coloreFase());

  // traccia con le linee di soglia
  const int Y0 = 50, HT = 96;
  float lo = 0, hi = altoTraccia();
  for (int i = 0; i < nTraccia; i++) lo = min(lo, traccia[i]);
  float linee[3] = { NAN, NAN, NAN }; uint16_t colLinee[3] = { CORALLO, ARANCIO, VIOLA };
  if (L.tipo == COPERTO)      { linee[0] = par("a"); linee[1] = par("da"); }
  else if (L.tipo == CAREZZA) { linee[0] = par("velocita"); }
  else {
    float s = par("soglia"), ist = par("isteresi");
    linee[0] = s;
    if (ist > 0) linee[1] = L.sotto ? s + ist : s - ist;
    if (par("tetto") > 0) linee[2] = par("tetto");
  }
  for (int k = 0; k < 3; k++) if (!isnan(linee[k])) hi = max(hi, linee[k]);
  hi = lo + (hi - lo) * 1.1f;
  if (hi - lo < 1e-3f) hi = lo + 1;
  auto y = [&](float v) { return Y0 + HT - (int)((v - lo) / (hi - lo) * HT); };

  c.drawRect(0, Y0 - 1, W, HT + 2, SCURO);
  for (int k = 0; k < 3; k++) if (!isnan(linee[k])) c.drawFastHLine(0, y(linee[k]), W, colLinee[k]);
  int primo = (testaTraccia - nTraccia + N_TRACCIA) % N_TRACCIA, yp = 0;
  for (int i = 0; i < nTraccia; i++) {
    int k = (primo + i) % N_TRACCIA, yy = y(traccia[k]);
    if (segni[k]) c.drawFastVLine(i, Y0, HT, segni[k] == 1 ? VERDE : ARANCIO);
    if (i > 0) c.drawLine(i - 1, yp, i, yy, AZZURRO);
    yp = yy;
  }

  c.setFont(&fonts::DejaVu12);
  testo(c, 6, 154, "eventi " + String(nEventi) + "   scartati " + String(nScartati));
  testo(c, 210, 154, modoTesto ? "seriale: testo" : "seriale: grafico", GRIGIO);
  if (ultimo.length()) testo(c, 6, 172, ultimo, coloreUltimo);
  else testo(c, 6, 172, "scrivi ? nel Monitor seriale", GRIGIO);

  if (millis() - tUltimoEvento < 250) { c.drawRect(0, 0, W, H - TOP, VERDE); c.drawRect(1, 1, W - 2, H - TOP - 2, VERDE); }
}

// ---- avvio, giro, uscita
void azzeraLab() {
  fase = LABS[labCorrente].tipo == COPERTO ? 2 : 0;   // coperto: prima deve vedere la luce
  fPronto = false; inCorso = false; picco = 0; distanzaDito = 0;
  nEventi = nScartati = 0; ultimo = ""; tUltimoEvento = 0;
  nTraccia = testaTraccia = 0; nAcc = 0; accEvento = false;
}

void avviaLab(int i) {
  labCorrente = i;
  azzeraLab();
  tCampione = 0;
  if (i == L_VOCE || i == L_COLPO) { CoreS3.Speaker.end(); CoreS3.Mic.begin(); }
  else CoreS3.Mic.end();
  CoreS3.Display.fillScreen(NERO);
  disegnaBarraNavigazione();
  if (modoTesto) stampaParametri();
}

void esciLab() {
  labCorrente = -1;
  entraPagina(pagina);
  CoreS3.Display.fillScreen(NERO);
  disegnaBarraNavigazione();
  Serial.println("laboratorio chiuso, si torna alle pagine");
}

// campionare conta più che disegnare: lo schermo si ridisegna solo quando
// non c'è un evento breve in corso, così il disegno non copre un picco
bool momentoDelicato() {
  const Lab& L = LABS[labCorrente];
  if (L.tipo == COPERTO) return fase == 1;
  if (L.tipo == CAREZZA) return false;
  return fase == F_DENTRO && par("finestra") > 0;
}

void giraLab() {
  const Lab& L = LABS[labCorrente];
  uint32_t ora = millis();
  if (L.periodo == 0 || ora - tCampione >= L.periodo) {
    float dt = tCampione ? (float)(ora - tCampione) : (L.periodo ? (float)L.periodo : 16.0f);
    if (dt < 1) dt = 1;
    tCampione = ora;
    float v;
    if (L.tipo == CAREZZA) passoCarezza(ora, dt);
    else if (leggiMisura(v)) {
      if (L.tipo == COPERTO) passoCoperto(v, ora); else passoGenerico(v, ora, dt);
    }
  }
  if (ora - tStampa >= 40) { stampaGrafico(); tStampa = ora; }
  if (!momentoDelicato() && ora - tSchermo >= 250) {   // ridisegnare costa circa 40 ms senza campioni
    tela.fillScreen(NERO);
    paginaLab(tela);
    tela.pushSprite(0, TOP);
    tSchermo = ora;
  }
}

// ---- seriale
void elencoLab() {
  Serial.println("laboratori (scrivi il nome per avviarne uno):");
  for (int i = 0; i < N_LAB; i++) Serial.printf("  %-10s %s\n", LABS[i].nome, LABS[i].misura);
}

void stampaParametri() {
  const Lab& L = LABS[labCorrente];
  Serial.printf("\nlab %s: %s", L.nome, L.misura);
  if (L.periodo) Serial.printf(", un campione ogni %u ms\n", L.periodo);
  else Serial.println(", un campione ogni 16 ms");
  for (int k = 0; k < MAX_PARAM && L.p[k].nome; k++)
    Serial.printf("  %-9s %8g %-9s %s\n", L.p[k].nome, L.p[k].val, L.p[k].unita, L.p[k].cosa);
  Serial.printf("eventi %d, scartati %d\n\n", nEventi, nScartati);
}

void stampaAiuto() {
  Serial.println();
  Serial.println("Laboratorio dei sensi: dalla misura all'evento (PROGETTO.md 5.1)");
  Serial.println("  lab           elenca i laboratori");
  Serial.println("  scossa        avvia un laboratorio (anche: lab scossa)");
  Serial.println("  soglia 1.2    cambia un parametro del laboratorio in corso");
  Serial.println("  valori        rimette i valori di partenza");
  Serial.println("  azzera        azzera i conteggi");
  Serial.println("  testo         scrive qui gli eventi, uno per riga");
  Serial.println("  grafico       manda i numeri al Plotter seriale");
  Serial.println("  fine          torna alle pagine dell'Esploratore");
  if (labCorrente >= 0) stampaParametri(); else elencoLab();
}

void comando(String s) {
  s.trim(); s.toLowerCase(); s.replace(',', '.');
  if (!s.length()) return;
  int sp = s.indexOf(' ');
  String cmd = sp < 0 ? s : s.substring(0, sp);
  String arg = sp < 0 ? String("") : s.substring(sp + 1);
  arg.trim();

  if (cmd == "?" || cmd == "aiuto" || cmd == "help") { stampaAiuto(); return; }
  if (cmd == "testo")   { modoTesto = true;  Serial.println("modo testo: un evento per riga"); return; }
  if (cmd == "grafico") { modoTesto = false; return; }   // da qui in poi solo numeri, per il Plotter
  if (cmd == "fine" || cmd == "esci" || (cmd == "lab" && arg == "fine")) { if (labCorrente >= 0) esciLab(); return; }
  if (cmd == "lab" && !arg.length()) { elencoLab(); return; }
  String nome = cmd == "lab" ? arg : cmd;
  for (int i = 0; i < N_LAB; i++) if (nome == LABS[i].nome) { avviaLab(i); return; }

  if (labCorrente < 0) { Serial.println("nessun laboratorio in corso: scrivi lab per l'elenco"); return; }
  Lab& L = LABS[labCorrente];
  if (cmd == "azzera") { azzeraLab(); if (modoTesto) Serial.println("conteggi azzerati"); return; }
  if (cmd == "valori") {
    for (int k = 0; k < MAX_PARAM && L.p[k].nome; k++) L.p[k].val = L.p[k].base;
    azzeraLab();
    if (modoTesto) stampaParametri();
    return;
  }
  for (int k = 0; k < MAX_PARAM && L.p[k].nome; k++) {
    if (cmd != L.p[k].nome) continue;
    if (arg.length()) { L.p[k].val = arg.toFloat(); if (!strcmp(L.p[k].nome, "filtro")) fPronto = false; }
    if (modoTesto) Serial.printf("%s = %g %s\n", L.p[k].nome, L.p[k].val, L.p[k].unita);
    return;
  }
  Serial.println("non capisco \"" + s + "\": scrivi ? per l'aiuto");
}

// una riga finisce con Invio; se il Monitor è su "Nessun fine riga" basta una pausa di 300 ms
void leggiSeriale() {
  while (Serial.available()) {
    char ch = Serial.read();
    if (ch == '\n' || ch == '\r') { if (riga.length()) comando(riga); riga = ""; }
    else if (riga.length() < 60) { riga += ch; tCarattere = millis(); }
  }
  if (riga.length() && millis() - tCarattere > 300) { comando(riga); riga = ""; }
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  Serial.begin(115200);

  CoreS3.Display.setBrightness(160);
  CoreS3.Display.fillScreen(NERO);
  CoreS3.Speaker.setVolume(volume);

  tela.setColorDepth(16);
  if (!tela.createSprite(W, H - TOP)) {   // senza PSRAM si ripiega sugli 8 bit
    tela.setColorDepth(8);
    tela.createSprite(W, H - TOP);
  }

  // camera (GC0308, anteprima 320x240 RGB565 in PSRAM)
  cameraOk = CoreS3.Camera.begin();
  if (cameraOk) CoreS3.Camera.sensor->set_hmirror(CoreS3.Camera.sensor, 0);

  // sensore di luce e prossimità LTR553
  Ltr5xx_Init_Basic_Para p = LTR5XX_BASE_PARA_CONFIG_DEFAULT;
  p.ps_led_pulse_freq   = LTR5XX_LED_PULSE_FREQ_40KHZ;
  p.ps_measurement_rate = LTR5XX_PS_MEASUREMENT_RATE_50MS;
  p.als_gain            = LTR5XX_ALS_GAIN_48X;
  p.als_measurement_rate = LTR5XX_ALS_MEASUREMENT_RATE_100MS;   // di serie 500 ms: troppo lento per "coperto"
  CoreS3.Ltr553.begin(&p);
  CoreS3.Ltr553.setPsMode(LTR5XX_PS_ACTIVE_MODE);
  CoreS3.Ltr553.setAlsMode(LTR5XX_ALS_ACTIVE_MODE);
  luceOk = true;   // il driver non segnala errori: se i valori restano a zero, controlla l'esempio ltr553

  // radio in modalità stazione, senza collegarsi a nessuna rete: serve a scansione ed ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  for (int i = 0; i < N_LAB; i++)
    for (int k = 0; k < MAX_PARAM; k++) LABS[i].p[k].base = LABS[i].p[k].val;

  Serial.printf("scheda %s, imu %s, pmic %s, camera %d, luce %d\n",
                nomeBoard(), nomeImu(), nomePmic(), cameraOk, luceOk);
  Serial.println("Laboratorio dei sensi: scrivi ? e premi Invio");
  entraPagina(pagina);
  disegnaBarraNavigazione();
}

void loop() {
  CoreS3.update();
  leggiSeriale();

  auto t = CoreS3.Touch.getDetail();
  if (labCorrente >= 0) {                         // nel laboratorio lo schermo è il sensore:
    if (t.wasClicked() && t.y < TOP) esciLab();   // si esce solo dalla barra in alto
    else giraLab();
    delay(1);
    return;
  }

  // navigazione: flick orizzontale o tocco sugli angoli della barra
  if (t.wasFlicked()) {
    int dx = t.distanceX();
    if (dx < -40) cambiaPagina(+1);
    else if (dx > 40) cambiaPagina(-1);
  } else if (t.wasClicked() && t.y < TOP) {
    if (t.x < TASTO) cambiaPagina(-1);
    else if (t.x >= W - TASTO) cambiaPagina(+1);
  }

  if (pagina == P_CAMERA) { paginaCamera(); return; }

  tela.fillScreen(NERO);
  switch (pagina) {
    case P_INFO:    paginaInfo(tela);    break;
    case P_ACCEL:   paginaAccel(tela);   break;
    case P_GYRO:    paginaGyro(tela);    break;
    case P_BUSSOLA: paginaBussola(tela); break;
    case P_LUCE:    paginaLuce(tela);    break;
    case P_MIC:     paginaMic(tela);     break;
    case P_TOUCH:   paginaTouch(tela);   break;
    case P_SPEAKER: paginaSpeaker(tela); break;
    case P_POWER:   paginaPower(tela);   break;
    case P_WIFI:    paginaWifi(tela);    break;
    case P_BLE:     paginaBle(tela);     break;
    case P_ESPNOW:  paginaEspNow(tela);  break;
    case P_I2C:     paginaI2C(tela);     break;
    case P_SD:      paginaSD(tela);      break;
    default: break;
  }
  tela.pushSprite(0, TOP);
  delay(10);
}
