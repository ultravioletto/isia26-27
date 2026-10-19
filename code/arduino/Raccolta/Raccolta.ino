// =====================================================================
//  Raccolta
//  Le prove in un solo firmware, con un menu all'avvio.
//
//  Menu:     due pagine, scelte con le linguette in alto:
//            "Audio e sincronia" (Esploratore, Prova sonora, Test sincronia)
//            "Visualizzazioni"   (Blob, Sciame, Scatola, Cimatica, Interference).
//            Tocca una prova per avviarla. Tornando dal menu si riapre la
//            pagina dell'ultima prova.
//  Uscita:   un clic sul tasto di accensione torna al menu.
//
//  Come funziona: scegliere una prova riavvia la scheda e, al riavvio,
//  parte direttamente quella prova. Così Wi-Fi, BLE, camera, microfono
//  e altoparlante partono sempre da zero, come se la prova fosse l'unico
//  sketch caricato. La scelta sopravvive al riavvio in una variabile
//  nella memoria RTC; un'accensione o un crash riportano al menu.
//  Anche un riavvio chiesto dalla prova stessa (per esempio il comando
//  "riavvia" di Test Sincronia) la fa ripartire.
//
//  Gli sketch originali restano nelle loro cartelle e si modificano lì.
//  I file app_*.h ne sono una copia chiusa in un namespace: dopo una
//  modifica, lancia "python aggiorna.py" in questa cartella e ricompila.
//
//  Scheda:   M5CoreS3 (pacchetto schede M5Stack)
//  Librerie: quelle delle prove (M5CoreS3, M5Unified, M5GFX)
// =====================================================================

#include "app_esploratore.h"    // per primo: include SD.h prima di M5CoreS3.h
#include "app_sonora.h"
#include "app_sincronia.h"
#include "app_blob.h"
#include "app_sciame.h"
#include "app_scatola.h"
#include "app_cimatica.h"
#include "app_interferenze.h"
#include <esp_system.h>

enum Pagina { AUDIO, VISUALIZZAZIONI, N_PAGINE };
const char* NOMI_PAGINE[N_PAGINE] = { "Audio e sincronia", "Visualizzazioni" };

struct Prova {
  Pagina pagina;
  const char* nome;
  const char* descrizione;
  void (*avvia)();
  void (*gira)();
};

const Prova PROVE[] = {
  { AUDIO,           "Esploratore",    "sensori e uscite",            esploratore::setup,  esploratore::loop },
  { AUDIO,           "Prova sonora",   "timbri dall'altoparlante",    sonora::setup,       sonora::loop },
  { AUDIO,           "Test sincronia", "orologio via ESP-NOW",        sincronia::setup,    sincronia::loop },
  { VISUALIZZAZIONI, "Blob",           "rotola, il suono lo deforma", blob::setup,         blob::loop },
  { VISUALIZZAZIONI, "Sciame",         "orbita, il suono lo sparge",  sciame::setup,       sciame::loop },
  { VISUALIZZAZIONI, "Scatola",        "stanza vettoriale, 3 solidi", scatola::setup,      scatola::loop },
  { VISUALIZZAZIONI, "Cimatica",       "modi di una membrana",        cimatica::setup,     cimatica::loop },
  { VISUALIZZAZIONI, "Interference",   "onde, inclina le sorgenti",   interferenze::setup, interferenze::loop },
};
const int N_PROVE = sizeof(PROVE) / sizeof(PROVE[0]);

// sopravvivono a ESP.restart(), non a uno spegnimento
static constexpr uint32_t FIRMA = 0x52414343;   // "RACC"
RTC_NOINIT_ATTR uint32_t firma;
RTC_NOINIT_ATTR uint32_t scelta;
int attiva = -1;                                  // -1 = menu
int pagina = AUDIO;

// ---------------------------------------------------------------- menu
static constexpr int W = 320, H = 240, TITOLO = 30, PIEDE = 20, M = 8;
static constexpr int BW = (W - 3 * M) / 2, BH = (H - TITOLO - PIEDE - 2 * M) / 3;

constexpr uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
const uint16_t NERO = rgb(0, 0, 0), BIANCO = rgb(255, 255, 255), GRIGIO = rgb(120, 120, 120);
const uint16_t SCURO = rgb(40, 40, 40), GIALLO = rgb(255, 210, 63);
const uint16_t COLORI[] = { rgb(255, 159, 28), rgb(123, 77, 255), rgb(32, 201, 192),
                            rgb(63, 167, 255), rgb(46, 194, 126), rgb(255, 90, 78),
                            rgb(225, 205, 170), rgb(255, 120, 90) };

// posto della prova nella griglia della sua pagina (0..5)
int posto(int i) {
  int p = 0;
  for (int j = 0; j < i; j++) if (PROVE[j].pagina == PROVE[i].pagina) p++;
  return p;
}

void rettangoloProva(int i, int& x, int& y) {
  int p = posto(i);
  x = M + (p % 2) * (BW + M);
  y = TITOLO + (p / 2) * (BH + M);
}

// linguette delle pagine, nella riga del titolo
static constexpr int LW = (W - 3 * M) / 2, LY = 4, LH = TITOLO - 8;
int linguettaX(int p) { return M + p * (LW + M); }

void disegnaLinguette() {
  auto& d = CoreS3.Display;
  d.setFont(&fonts::DejaVu12);
  d.setTextDatum(textdatum_t::middle_center);
  for (int p = 0; p < N_PAGINE; p++) {
    bool attivaP = p == pagina;
    d.fillRoundRect(linguettaX(p), LY, LW, LH, 6, attivaP ? GIALLO : SCURO);
    d.setTextColor(attivaP ? NERO : GRIGIO);
    d.drawString(NOMI_PAGINE[p], linguettaX(p) + LW / 2, LY + LH / 2);
  }
}

void disegnaProva(int i, bool premuta) {
  auto& d = CoreS3.Display;
  int x, y;
  rettangoloProva(i, x, y);
  d.fillRoundRect(x, y, BW, BH, 8, premuta ? COLORI[i] : SCURO);
  d.fillRect(x, y + 8, 4, BH - 16, COLORI[i]);
  d.setTextDatum(textdatum_t::top_left);
  d.setFont(&fonts::DejaVu18);
  d.setTextColor(premuta ? NERO : BIANCO);
  d.drawString(PROVE[i].nome, x + 14, y + 10);
  d.setFont(&fonts::DejaVu12);
  d.setTextColor(premuta ? NERO : GRIGIO);
  d.drawString(PROVE[i].descrizione, x + 14, y + 34);
}

void disegnaMenu() {
  auto& d = CoreS3.Display;
  d.fillScreen(NERO);
  disegnaLinguette();
  for (int i = 0; i < N_PROVE; i++) if (PROVE[i].pagina == pagina) disegnaProva(i, false);
  d.setFont(&fonts::DejaVu12);
  d.setTextColor(GRIGIO);
  d.setTextDatum(textdatum_t::bottom_center);
  d.drawString("tasto di accensione: torna qui", W / 2, H - 4);
}

int provaToccata(int tx, int ty) {
  for (int i = 0; i < N_PROVE; i++) {
    if (PROVE[i].pagina != pagina) continue;
    int x, y;
    rettangoloProva(i, x, y);
    if (tx >= x && tx < x + BW && ty >= y && ty < y + BH) return i;
  }
  return -1;
}

void riavviaIn(int i) {
  if (i >= 0) { firma = FIRMA; scelta = i; }
  else        { firma = 0; }
  delay(150);
  ESP.restart();
}

// ---------------------------------------------------------------- setup e loop
void setup() {
  // solo un riavvio software porta dentro una prova: accensione, crash e watchdog tornano al menu
  if (esp_reset_reason() == ESP_RST_SW && firma == FIRMA && scelta < (uint32_t)N_PROVE) {
    attiva = scelta;
    PROVE[attiva].avvia();
    return;
  }
  // tornando da una prova (riavvio software, scelta valida) si riapre la sua pagina
  if (esp_reset_reason() == ESP_RST_SW && scelta < (uint32_t)N_PROVE) pagina = PROVE[scelta].pagina;
  firma = 0;
  auto cfg = M5.config();
  CoreS3.begin(cfg);
  CoreS3.Display.setBrightness(160);
  disegnaMenu();
}

void loop() {
  if (attiva >= 0) {
    PROVE[attiva].gira();
    if (CoreS3.BtnPWR.wasClicked()) riavviaIn(-1);
    return;
  }

  CoreS3.update();
  auto t = CoreS3.Touch.getDetail();
  if (t.wasClicked()) {
    if (t.y < TITOLO) {                                   // linguette
      int p = t.x < linguettaX(1) ? 0 : 1;
      if (p != pagina) { pagina = p; disegnaMenu(); }
      return;
    }
    int i = provaToccata(t.x, t.y);
    if (i >= 0) {
      disegnaProva(i, true);
      riavviaIn(i);
    }
  }
  delay(10);
}
