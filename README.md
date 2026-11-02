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
| [lezione-03-sensi-e-gesti.md](docs/lezione-03-sensi-e-gesti.md) | Piano della lezione 3 (bozza): dalla misura all'evento, i nove gesti e il contesto, consegna dei dispositivi e misure della stanza con l'Esploratore. |
| [lezione-03-laboratorio-dei-sensi.md](docs/lezione-03-laboratorio-dei-sensi.md) | Scheda d'uso del laboratorio dei sensi dell'Esploratore: comandi del Monitor seriale e del Plotter, i parametri, dieci esperimenti, uno per senso, su soglia, isteresi, tempo minimo, finestra, pausa e filtro. |
| [lezione-04-espressione-senza-volto.md](docs/lezione-04-espressione-senza-volto.md) | Piano della lezione 4 (bozza): i cinque canali, la grammatica dei bisogni e il sonno; significato dei gesti e forma di base animata sul dispositivo. |
| [lezione-05-design-system-e-suono.md](docs/lezione-05-design-system-e-suono.md) | Piano della lezione 5 (bozza): le dieci regole del design system, tavolozza e primari, scala e timbri; identità della specie provata accanto alle altre. |
| [lezione-06-la-piattaforma.md](docs/lezione-06-la-piattaforma.md) | Piano della lezione 6 (bozza): architettura, file della specie e strumenti di lavoro; dalla specie neutra alla specie v0 su due dispositivi. |
| [lezione-07-corpo-fisico-e-osservazione.md](docs/lezione-07-corpo-fisico-e-osservazione.md) | Piano della lezione 7 (bozza): l'involucro e la regola 10, i metodi di osservazione; revisione intermedia con playtest incrociato e un'ora di vita registrata. |
| [lezione-08-la-rete.md](docs/lezione-08-la-rete.md) | Piano della lezione 8 (bozza): ESP-NOW, annuncio e vicini, messaggi comuni e codici liberi; la vita sociale della specie. |
| [lezione-09-convivenza.md](docs/lezione-09-convivenza.md) | Piano della lezione 9 (bozza): regole di convivenza e limiti della radio; prima prova di flotta con l'osservatore e controlli verificabili. |
| [lezione-10-unisono-orologio-e-sessione.md](docs/lezione-10-unisono-orologio-e-sessione.md) | Piano della lezione 10 (bozza): lo stato di grazia, l'orologio condiviso e la sessione; la parte di unisono della specie. |
| [lezione-11-unisono-autonoma-e-suonata.md](docs/lezione-11-unisono-autonoma-e-suonata.md) | Piano della lezione 11 (bozza): voce autonoma, ruoli e tratti, visivo del battito; prova generale autonoma e suonata, involucro definitivo. |
| [lezione-12-la-stanza.md](docs/lezione-12-la-stanza.md) | Piano della lezione 12 (bozza): allestimento della stanza, presentazione finale, critica collettiva e retrospettiva. |
| [segni-macchina-a-stati.html](docs/segni-macchina-a-stati.html) | I segni per disegnare una macchina a stati secondo UML, il confronto con le altre notazioni e il modello della creatura ridisegnato, con lo stato proprio e l'unisono. |
| [simulatore-vita-creatura.html](docs/simulatore-vita-creatura.html) | Il simulatore della vita di una creatura in turni da 10 minuti, con i tre mazzi di cura, i parametri della specie e lo stato proprio modificabili. |
| [unisono-stati.html](docs/unisono-stati.html) | L'unisono come macchina a stati gerarchica: ingresso, sessione, uscita, eccezioni e recupero. |

Le pagine HTML si aprono direttamente nel browser, senza installare niente.

### code/arduino

Le prove sul dispositivo, una cartella per sketch.

| Sketch | Cosa fa |
| --- | --- |
| [EsploratoreCoreS3](code/arduino/EsploratoreCoreS3) | Una pagina per ogni sensore e uscita del CoreS3: IMU, bussola, luce, camera, microfono, touch, altoparlante, alimentazione, Wi-Fi, BLE, ESP-NOW, I2C, SD. Con il **laboratorio dei sensi** (lezione 3), comandato dal Monitor seriale: soglia, isteresi, tempo minimo, finestra, pausa e filtro su dieci eventi del §5.1, con i numeri per il Plotter seriale. Scrivi `?` nel Monitor a 115200 baud. |
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
