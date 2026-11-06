# Istruzioni per gli assistenti AI

Questo file dà il contesto del progetto a qualsiasi assistente AI che lavori in questa cartella: Gemini CLI, Codex (ChatGPT), GitHub Copilot, Cursor, Claude Code. Se usi un assistente in una chat web, caricalo insieme ai file su cui lavori.

## Il progetto

**Creature in rete** è il progetto del corso di Interaction Design 2026–27 all'ISIA. È un sistema di piccole creature elettroniche da curare, una per ogni M5Stack CoreS3: hanno un corpo che sente come vengono tenute, un'espressione senza volto e senza parole, una vita sociale via radio (ESP-NOW) e uno stato di grazia collettivo, l'**unisono**, in cui la flotta diventa uno strumento musicale.

La flotta è di circa dieci dispositivi, divisi in quattro o cinque **specie**. Tutte condividono la stessa **piattaforma** (bisogni, stati, sensi, rete, design system). Ogni gruppo di studenti progetta una specie.

Chi lavora qui sono soprattutto studenti di design, non programmatori. Spiega cosa fai e perché, in italiano semplice, e preferisci codice leggibile a codice furbo.

## Dove sta cosa

```
project/   la specifica del sistema
docs/      i materiali del corso: programma, lezioni, pagine HTML di strumenti
code/      il codice Arduino per M5Stack CoreS3, una cartella per sketch
private/   materiale privato del docente: non leggerlo, non citarlo, non pubblicarlo
```

- **[project/PROGETTO.md](project/PROGETTO.md) è la fonte di verità.** Prima di proporre un comportamento, un parametro o un messaggio radio, controlla cosa dice la specifica. I numeri di partenza sono nell'appendice A, il formato dei pacchetti nell'appendice B, le decisioni ancora aperte nel §12.1.
- Il PDF `project/creature-in-rete-progetto.pdf` ha gli schemi, ma è più vecchio del testo: dove non coincidono, vale PROGETTO.md.
- Le lezioni sono in `docs/lezione-NN-*.md`. Quelle dalla 3 in poi sono bozze.
- La piattaforma descritta nel §3.4 (`Creatura.ino`, `nucleo.h`, `sensi.h`, `config.h`, `creatura.h`...) **non esiste ancora**. Gli sketch in `code/arduino/` sono prove sul dispositivo, non la piattaforma.

## Le regole del progetto

Queste vengono dalla specifica e valgono per tutto quello che proponi.

- **Nessun volto.** Niente occhi, bocche, emoji, pittogrammi. L'espressione è forma astratta, movimento, colore, luce, suono.
- **Nessun testo sullo schermo** in modalità normale. Il testo esiste solo nello schermo di debug.
- **Una piattaforma, molte specie.** Una specie cambia solo i suoi due file (`config.h` e `creatura.h`) e non tocca mai l'hardware direttamente. Non aggiunge bisogni, sensi o stati comuni; il suo unico stato è lo **stato proprio** (§4.4).
- **Lo stato proprio** si entra solo da SVEGLIO e si torna a SVEGLIO, dura al massimo 10 minuti, ha almeno 20 minuti tra una volta e l'altra, non tocca la salute e non deve sembrare il sonno. I limiti 10/20 sono provvisori.
- **La radio è un linguaggio, non un cavo.** Un messaggio è un verso, non un comando. Una specie ignora i messaggi che non conosce e non risponde mai in automatico, tranne RISPOSTA a CHIAMATA (§9.3). Al massimo 5 messaggi al secondo.
- **Il sonno è riconoscibile**: luminosità al 10%, respiro a 0,2 Hz, nessun colore saturo (§10, regola 9).
- **Suono comune**: scala pentatonica maggiore da do4, 90 bpm nell'unisono, tre intensità (sussurro, normale, urgente).

## Hardware: cose già scoperte sul CoreS3

- **I due microfoni non danno la direzione del suono**: sono troppo vicini. Trattali come una sola sorgente (per esempio la media dei due canali). Sono analogici, letti dal convertitore ES7210; il §3.1 di PROGETTO.md che dice "2 PDM" è sbagliato.
- **La bussola (BMM150) è inutilizzabile con la base standard attaccata**: i quattro magneti della base la saturano. Non è un errore del codice. Per una specie che ha bisogno della direzione serve una base o un involucro senza magneti.
- Il CoreS3 non ha il motore di vibrazione: la piattaforma lo sostituisce con un suono sordo di 30 ms. Luce, prossimità e vibrazione sono un di più, mai l'unico canale di un significato.

## Come si scrive il codice qui

- **Ambiente**: Arduino IDE 2 con il pacchetto schede M5Stack, scheda **M5CoreS3**, libreria **M5CoreS3** (porta con sé M5Unified e M5GFX). Non proporre PlatformIO, ESP-IDF o MicroPython.
- **Ogni sketch comincia con un commento** che dice cosa fa, come si usa, la scheda e le librerie. Segui il modello di [EsploratoreCoreS3.ino](code/arduino/EsploratoreCoreS3/EsploratoreCoreS3.ino).
- **Nomi e commenti in italiano**, come nel resto del codice (`pagina`, `tela`, `soglia`, `vicini`).
- **Niente attese bloccanti nel ciclo principale**: ogni parte lavora alla propria frequenza contando il tempo con `millis()`, così il disegno non rallenta i sensori e la radio (§3.3).
- **Schermo 320 × 240**. Per disegnare senza sfarfallio si usa un `M5Canvas` fuori schermo, poi lo si spinge sul display.
- **Seriale a 115200 baud.** I parametri si cambiano da seriale senza ricompilare (§11).
- **Raccolta**: i file `code/arduino/Raccolta/app_*.h` sono generati, non modificarli a mano. Dopo aver cambiato uno sketch, si lancia `python aggiorna.py` nella cartella `Raccolta`.
- Non puoi provare il codice sul dispositivo: dillo chiaramente, e di' allo studente cosa controllare quando lo carica (cosa deve vedere sullo schermo, cosa deve stampare la seriale).

## Prima di consegnare

- Il repository è **pubblico** su GitHub (`ultravioletto/isia26-27`). Non mettere mai password, chiavi o dati personali nei file.
- Le licenze sono non commerciali: PolyForm Noncommercial per `code/`, CC BY-NC-SA 4.0 per tutto il resto. Non copiare codice da fonti con licenze incompatibili senza dirlo.
- Se cambi la specifica o aggiungi un materiale, aggiorna anche la tabella in [README.md](README.md).
