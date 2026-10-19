# Creature in rete

Materiali del corso di **Interaction Design 2026–27** all'ISIA.

Creature in rete è un sistema di piccole creature elettroniche da curare. Ogni creatura vive in un M5Stack Core e ha quattro cose che un Tamagotchi non ha mai avuto:

- un **corpo** che sente come viene tenuta;
- un modo di **esprimersi** senza volto e senza parole;
- una **vita sociale** via radio con le altre creature nella stanza;
- uno **stato di grazia collettivo**, l'unisono, in cui la flotta intera diventa uno strumento musicale che suona da solo e si lascia suonare.

La flotta è di circa dieci dispositivi, divisi in quattro o cinque specie. Tutte le creature condividono la stessa piattaforma: bisogni, stati, sensi, protocollo radio e design system. Ogni specie definisce il proprio carattere, la propria espressione, il proprio stato proprio e la propria parte nell'unisono. Durante il corso ogni gruppo progetta una specie.

## Struttura

```
project/   il progetto: la specifica del sistema
docs/      i materiali del corso: programma, lezioni, approfondimenti, strumenti
code/      il codice Arduino per M5Stack CoreS3
```

### project

| File | Contenuto |
| --- | --- |
| [PROGETTO.md](project/PROGETTO.md) | La specifica completa: architettura, modello della creatura (bisogni, stati, transizioni, stato proprio), sensi, espressione, rete, unisono, specie, design system, strumenti, rischi, parametri di partenza e formato dei pacchetti. |
| [creature-in-rete-progetto.pdf](project/creature-in-rete-progetto.pdf) | Lo stesso documento impaginato, con i dodici schemi in appendice C. Gli schemi sono della bozza dell'11 ottobre 2026: la macchina a stati (figura 4) non mostra ancora lo stato proprio. |

### docs

| File | Contenuto |
| --- | --- |
| [programma-corso-interaction-design-26-27.md](docs/programma-corso-interaction-design-26-27.md) · [pdf](docs/programma-corso-interaction-design-26-27.pdf) | Il programma del corso, lezione per lezione. |
| [lezione-02-la-creatura-nel-tempo.md](docs/lezione-02-la-creatura-nel-tempo.md) | Piano della lezione 2: macchine a stati finiti, bisogni nel tempo, simulazione su carta e giornata tipo della specie. |
| [segni-macchina-a-stati.html](docs/segni-macchina-a-stati.html) | I segni per disegnare una macchina a stati secondo UML, il confronto con le altre notazioni e il modello della creatura ridisegnato, con lo stato proprio e l'unisono. |
| [simulatore-vita-creatura.html](docs/simulatore-vita-creatura.html) | Il simulatore della vita di una creatura in turni da 10 minuti, con i tre mazzi di cura, i parametri della specie e lo stato proprio modificabili. |
| [unisono-stati.html](docs/unisono-stati.html) | L'unisono come macchina a stati gerarchica: ingresso, sessione, uscita, eccezioni e recupero. |

Le pagine HTML si aprono direttamente nel browser, senza installare niente.

### code/arduino

Le prove sul dispositivo, una cartella per sketch.

| Sketch | Cosa fa |
| --- | --- |
| [EsploratoreCoreS3](code/arduino/EsploratoreCoreS3) | Una pagina per ogni sensore e uscita del CoreS3: IMU, bussola, luce, camera, microfono, touch, altoparlante, alimentazione, Wi-Fi, BLE, ESP-NOW, I2C, SD. |
| [ProvaSonora](code/arduino/ProvaSonora) | Cosa sa fare l'altoparlante: timbri, registri, inviluppi, accordi, sweep. Con "Tutti" più dispositivi suonano insieme via ESP-NOW. |
| [TestSincronia](code/arduino/TestSincronia) | Prova dell'orologio condiviso dell'unisono su tre o più dispositivi (PROGETTO.md §8.2–8.3). `banco.py` li pilota via seriale e misura lo scarto. |
| [ProvaBlob](code/arduino/ProvaBlob) | Un blob morbido che rotola con l'inclinazione e si deforma con il suono. |
| [ProvaSciame](code/arduino/ProvaSciame) | Uno sciame di boids che orbita intorno a un punto guidato dall'inclinazione e si disperde con il suono. |
| [ProvaScatola](code/arduino/ProvaScatola) | Una stanza vettoriale dietro lo schermo, con oggetti in wireframe che cadono seguendo la gravità vera. |
| [ProvaCimatica](code/arduino/ProvaCimatica) | I modi di vibrazione di una membrana circolare. |
| [InterferencePatterns](code/arduino/InterferencePatterns) | Onde in un piatto circolare, mosse da due sorgenti che rotolano con l'inclinazione. |
| [Raccolta](code/arduino/Raccolta) | Tutte le prove in un solo firmware, con un menu all'avvio. |

**Per compilare:** Arduino IDE con il pacchetto schede M5Stack, scheda **M5CoreS3**, libreria **M5CoreS3** (installa anche M5Unified e M5GFX). Ogni sketch indica in testa le librerie che usa.

**Raccolta:** i file `app_*.h` sono generati a partire dagli sketch originali e non vanno modificati a mano. Dopo aver cambiato uno sketch, lancia `python aggiorna.py` nella cartella `Raccolta` (serve `arduino-cli`, già incluso nell'Arduino IDE 2) e ricompila.

## Hardware

M5Stack **CoreS3** (anche Core2, con i sensi in comune: IMU, microfono, touch). Le specie progettano su questi sensi; luce, prossimità e vibrazione sono un di più. I dettagli sono in [PROGETTO.md §3](project/PROGETTO.md).

## Licenza

© 2026 ultravioletto. Due licenze, entrambe non commerciali:

| Cosa | Licenza | Testo |
| --- | --- | --- |
| Il codice, nella cartella `code/` | [PolyForm Noncommercial 1.0.0](https://polyformproject.org/licenses/noncommercial/1.0.0) | [code/LICENSE.md](code/LICENSE.md) |
| Tutto il resto: documenti, materiali del corso, pagine HTML | [Creative Commons Attribuzione – Non commerciale – Condividi allo stesso modo 4.0 (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/deed.it) | [LICENSE](LICENSE) |

**Il codice** si può usare, modificare e distribuire per qualsiasi scopo non commerciale: studio, ricerca, uso personale, scuole e enti senza fini di lucro. Chi lo distribuisce deve includere la licenza e la riga `Required Notice` in testa al file.

**I materiali** si possono copiare, distribuire e modificare a tre condizioni:
- **Attribuzione:** si cita la fonte.
- **Non commerciale:** non si usa a scopo di lucro.
- **Condividi allo stesso modo:** le opere derivate escono con la stessa licenza.
