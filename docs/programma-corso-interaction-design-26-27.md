# Interaction Design 2026–27 · Creature in rete

Programma del corso · 11 ottobre 2026 · Bruno

## Introduzione

Quest'anno non progettiamo schermate: progettiamo comportamenti. Il corso ruota intorno a **Creature in rete**, una flotta di piccole creature elettroniche da curare, ognuna dentro un M5Stack.

Le creature hanno un corpo che sente come vengono tenute. Si esprimono senza volto e senza parole, e si parlano via radio con le altre creature nella stanza. In certi momenti smettono di essere individui e diventano, tutte insieme, uno strumento musicale.

L'oggetto è un gioco, il Tamagotchi, ma le domande sono quelle dell'interaction design di oggi. Come si progetta qualcosa che cambia anche quando nessuno lo guarda? Come si fa capire cosa prova un oggetto senza disegnargli una faccia? Cosa succede quando dieci oggetti progettati da persone diverse devono vivere nella stessa stanza?

Si lavora in gruppo e, dalla terza lezione, sul dispositivo vero. Ogni gruppo progetta una specie, con il suo carattere, e alla fine la porta nella stanza insieme a tutte le altre.

### Le tematiche

| Tematica | Nel progetto | Lezioni |
| --- | --- | --- |
| Comportamento nel tempo | bisogni che decadono, stati, memoria: si progetta cosa succede tra un'ora, non cosa succede quando premo | 2, 6 |
| Interazione incarnata | sensori, gesti e contesto: la creatura reagisce a come viene tenuta, non a un pulsante | 1, 3 |
| Espressione non antropomorfa | forma, movimento, colore, luce e suono al posto di occhi, bocche ed emoji | 4 |
| Design system e grammatiche condivise | regole comuni che rendono leggibile una flotta di specie diverse; piattaforma e variazione | 5, 6 |
| Sistemi distribuiti e comportamento collettivo | ESP-NOW, vicini, messaggi, contagio: la vita sociale nasce da regole semplici | 8, 9 |
| Suono e strumenti collaborativi | scala comune, orologio condiviso, voce autonoma e suonata: autonomia e controllo tra persona e macchina | 10, 11 |
| Cura e relazione | attaccamento, trascuratezza, malattia e rinascita: cosa vuol dire progettare qualcosa da curare | 2, 7 |
| L'oggetto fisico | involucro, materiali, vincoli dei sensori e dell'antenna | 7, 11 |
| Prototipazione con hardware reale | microcontrollori, firmware, parametri modificabili al volo, debug su seriale | 3, 6 |
| Osservazione e valutazione | playtest, registrazione della vita della creatura, critica collettiva | 7, 12 |

## Il corso in breve

In 12 lezioni ogni gruppo progetta una **specie** di Creature in rete e la fa vivere su due dispositivi. All'ultima lezione la flotta intera abita la stessa stanza e suona insieme nell'unisono.

Il corso segue il documento di progetto: si parte dal corpo e dai sensi, si passa all'espressione senza volto, poi alla vita sociale via radio e infine all'unisono. Ogni lezione dura 7 ore:

- **2 ore frontali**: le tecniche e le specifiche del progetto che servono quel giorno.
- **5 ore di lavoro di gruppo**: si progetta, si scrive e si prova sui dispositivi, con revisione ai tavoli.

La piattaforma (bisogni, stati, sensi, rete, unisono, strumenti) è data e non si modifica. Gli studenti progettano il **carattere**: lavorano in due file, `config.h` e `creatura.h`, più l'involucro fisico.

**Alla fine ogni gruppo consegna** una specie funzionante su due M5Stack, il suo involucro, una scheda di specie e un video di un minuto.

## Gruppi e specie

Un gruppo, una specie, due dispositivi. Con una flotta di circa dieci M5Stack la classe si divide in **4 o 5 gruppi**. Due dispositivi restano di riserva con la specie neutra.

- **I gruppi si formano alla lezione 1**, dopo la presentazione del progetto e dell'hardware, e restano gli stessi fino alla fine.
- **Ogni specie sceglie un primario diverso** nella tavolozza comune: la regola 3 del design system impedisce di condividerlo.
- **I cinque caratteri del documento** (la timida, la contagiosa, la nomade, la coppia, l'anziana) sono esempi, non un menu: un gruppo può partire da uno di essi, ma deve trasformarlo.
- **Le specie devono convivere senza conoscersi.** Dalla lezione 9 in poi ogni prova si fa con la flotta intera, quindi il lavoro di un gruppo dipende anche dal rispetto delle regole da parte degli altri.

## Il percorso

Due verifiche: revisione alla lezione 7, presentazione alla 12. Lo schema completo è nel PDF accanto a questo file.

| Fase | Lezioni | Tappe |
| --- | --- | --- |
| Corpo | 1–3 | lezione 1: gruppi formati · lezione 3: dispositivi consegnati |
| Espressione | 4–7 | lezione 6: piattaforma consegnata · **lezione 7: revisione intermedia** |
| Società | 8–9 | lezione 9: prima prova di flotta |
| Grazia | 10–12 | **lezione 12: presentazione finale** |

Le prime sei lezioni costruiscono una creatura singola; dalla 8 in poi conta la flotta.

## Le lezioni

Ogni lezione ha la stessa forma: la parte frontale introduce cosa serve al lavoro di gruppo del pomeriggio, e il lavoro di gruppo chiude con una consegna piccola e verificabile.

### 1. Il progetto e il corpo

- **Frontale.** Dal Tamagotchi alle Creature in rete: i quattro elementi (corpo, cura, società, grazia) e i sei principi di progetto. L'hardware: M5Stack CoreS3, sensori, radio, batteria. La toolchain Arduino con M5Unified.
- **Gruppo.** Installare l'ambiente sui portatili e verificarlo compilando l'Esploratore CoreS3. Dimostrazione dell'Esploratore sul dispositivo del docente. Formare i gruppi.
- **Consegna.** Mappa dei sensi: per ogni sensore, cosa potrebbe sentire una creatura di come viene tenuta.

### 2. La creatura nel tempo

- **Frontale.** Le macchine a stati finiti: stati, eventi, condizioni, transizioni, azioni. Bisogni come variabili continue e stati come loro lettura. Gestire le transizioni: soglia, isteresi, tempo minimo, priorità, e i difetti tipici (oscillazione, assillo, stato pozzo). Il modello della creatura: quattro bisogni, salute nascosta, nove stati comuni più uno stato proprio per specie; tempo spento, memoria e contatori di vita. "Cosa succede tra un'ora" come domanda di progetto.
- **Gruppo.** Simulare a mano una mattina della specie neutra con tre mazzi di cura (attenta, distratta, assente), poi confrontare i grafici di vita. Disegnare la giornata tipo della propria specie su 24 ore e farla vivere a un altro gruppo. Strumento: il simulatore di vita.
- **Consegna.** Concept di specie in una pagina, giornata tipo, tabella dei parametri modificati rispetto alla neutra, grafici della simulazione. Piano dettagliato: `lezione-02-la-creatura-nel-tempo.md`.

### 3. Sensi e gesti

- **Frontale.** Dalla misura all'evento: filtri, soglie, isteresi, tempo minimo. I nove gesti comuni e il contesto (tavolo, mano, tasca). Perché la piattaforma definisce il gesto e la specie il suo significato.
- **Gruppo.** Consegna dei dispositivi. Caricare l'Esploratore, percorrerne le 15 pagine e misurare scosse, inclinazioni, livelli di voce e luce nella stanza vera.
- **Consegna.** Tabella delle misure: i valori osservati per scossa, inclinazione, voce e luce nella stanza. Piano dettagliato: `lezione-03-sensi-e-gesti.md`.

### 4. Espressione senza volto

- **Frontale.** I cinque canali (schermo, colore, suono, vibrazione, luminosità). La grammatica dei bisogni: nutrimento contrae, energia accelera, umore satura, socialità apre. Le tre intensità e la convenzione del sonno. Grafica generativa con le primitive di M5GFX.
- **Gruppo.** Decidere cosa significano i nove gesti per la propria specie. Disegnare la forma di base e le sue deformazioni, poi programmarla sul dispositivo.
- **Consegna.** Tabella gesto → significato → effetto sui bisogni. Forma di base animata, con le quattro direzioni espressive comandate a mano.

### 5. Design system e suono

- **Frontale.** Le dieci regole del design system. La tavolozza comune su uno schermo IPS da 2 pollici. Il suono: scala pentatonica, quattro famiglie d'onda, inviluppi, i limiti dell'altoparlante da 1 W.
- **Gruppo.** Scegliere palette e timbro, provarli in stanza accanto alle altre specie.
- **Consegna.** Identità della specie in `config.h`: codice, palette, timbro.

### 6. La piattaforma

- **Frontale.** Architettura a livelli, ciclo di esecuzione senza attese, struttura dei file. Cosa la specie può fare e cosa no. Gli strumenti di lavoro: seriale, comandi `set` e `get`, fattore tempo, schermo di debug.
- **Gruppo.** Partire dalla specie neutra e trasformarla nella propria: bisogni, stati, reazioni ai gesti, espressione.
- **Consegna.** Specie v0 che vive su due dispositivi.

### 7. Corpo fisico e osservazione

- **Frontale.** L'involucro: vincoli della regola 10 (impronta 54 × 54 mm, finestre libere, rigidità, antenna). Metodi di osservazione: playtest, registrazione da seriale, la vita della creatura come grafico.
- **Gruppo.** Revisione intermedia: ogni gruppo fa provare la propria specie a un altro, senza spiegazioni. Primi modelli di involucro.
- **Consegna.** Revisione intermedia: specie v0, un'ora di vita registrata da seriale, schizzi dell'involucro.

### 8. La rete

- **Frontale.** ESP-NOW e la metafora del verso. Annuncio, tabella dei vicini, RSSI e vicinanza. I messaggi comuni (1–7) e i codici liberi (100–199). Prova dal vivo con la pagina ESP-NOW dell'Esploratore.
- **Gruppo.** Dare alla specie una vita sociale: come reagisce ai vicini, cosa manda, cosa ignora.
- **Consegna.** Specie sociale tra i due individui del gruppo, codici propri documentati.

### 9. Convivenza

- **Frontale.** Le regole di convivenza, il limite di 5 messaggi al secondo, il divieto di risposta automatica. Perché dieci creature possono intasare una stanza in pochi secondi. L'osservatore come strumento di verifica.
- **Gruppo.** Prima prova con la flotta intera nella stessa stanza, con l'osservatore acceso. Correzioni.
- **Consegna.** Specie che convive con tutte le altre per 30 minuti senza errori.

### 10. Unisono: l'orologio e la sessione

- **Frontale.** Lo stato di grazia e i bisogni congelati. L'orologio condiviso: SYNC, passi, deriva. La sessione: innesco, ingresso, continuità, fine. Lo strumento: beccheggio, rollio, tap quantizzato.
- **Gruppo.** Definire la parte di unisono: ruolo, pattern di 16 passi, ottava, innesco, uscita.
- **Consegna.** Parte di unisono in `config.h`, provata con almeno un'altra specie.

### 11. Unisono: autonoma e suonata

- **Frontale.** La voce autonoma e i quattro ruoli (bordone, ritmo, melodia, eco). Docilità, ascolto, prontezza, quorum. Il visivo del battito. Come si presenta una flotta a un pubblico.
- **Gruppo.** Prova generale con la flotta intera: unisono autonomo, poi suonato da persone esterne. Involucro definitivo.
- **Consegna.** Specie completa e involucro montato.

### 12. La stanza

- **Frontale.** Allestimento della stanza e ultima prova di flotta.
- **Gruppo.** Presentazione pubblica: la flotta vive, le persone la curano e la suonano. Critica collettiva e retrospettiva.
- **Consegna.** Consegna finale (vedi sotto).

## Consegne e valutazione

La valutazione è di gruppo e guarda soprattutto il comportamento della specie nella stanza, non il codice. Ci sono due momenti di verifica: la revisione intermedia alla lezione 7 e la presentazione alla lezione 12. Le consegne delle altre lezioni servono a tenere il passo e non danno voto.

**Consegna finale**

- Specie funzionante su due dispositivi, con `config.h` e `creatura.h`.
- Involucro montato, conforme alla regola 10.
- Scheda di specie, al massimo 2 pagine: carattere, significato dei nove gesti, messaggi propri, parte di unisono.
- Video di un minuto della specie in vita.
- Un'ora di vita registrata da seriale, in forma di grafico.

**Criteri (proposta)**

| Criterio | Cosa si guarda | Peso |
| --- | --- | --- |
| Carattere e leggibilità | si capisce cosa prova la creatura senza volto né testo; la grammatica dei bisogni è rispettata | 30% |
| Interazione | i gesti hanno significati coerenti e scoperti con piacere da chi non conosce la specie | 20% |
| Comportamento nel tempo | la creatura cambia anche quando nessuno la tocca; il grafico di vita lo mostra | 15% |
| Vita sociale e unisono | convive con le altre specie; la parte di unisono è riconoscibile e suonabile | 20% |
| Involucro e documentazione | involucro rigido e funzionale; scheda di specie chiara | 15% |

## Materiali e attrezzatura

| Cosa | Quanti | Note |
| --- | --- | --- |
| M5Stack CoreS3 | 2 per gruppo + 2 di riserva | le riserve restano caricate con la specie neutra |
| ESP32 osservatore | 1 | qualsiasi ESP32, anche senza schermo; serve dalla lezione 9 |
| Cavi USB-C | 1 per dispositivo | per caricare i binari e leggere la seriale |
| Portatili | almeno 1 per gruppo | Arduino IDE, pacchetto schede M5Stack 3.3.9, librerie M5Unified, M5GFX e M5CoreS3 a versione fissa |
| Scheda microSD | facoltativa | per registrare la vita della creatura senza cavo |
| Laboratorio di fabbricazione | dalla lezione 7 | stampa 3D o taglio laser per gli involucri |

Materiali didattici:

- il documento di progetto `project/creature-in-rete-progetto.pdf`, con i dodici schemi;
- lo sketch **Esploratore CoreS3**, che mostra ogni sensore e uscita del dispositivo;
- la piattaforma con la specie neutra, da distribuire alla lezione 6.

## Punti da decidere

- [ ] **Piattaforma pronta per la lezione 6.** Il programma presuppone che nucleo, sensi ed espressione siano scritti prima della lezione 6, e rete e unisono prima della lezione 8.
- [ ] **Modello della flotta.** Il programma assume il CoreS3. Se la flotta è mista con Core2, la lezione 3 deve limitarsi ai sensi comuni (IMU, microfono, touch).
- [ ] **Numero di studenti**, e quindi numero di gruppi (4 o 5) e di dispositivi da acquistare.
- [ ] **Date delle lezioni** e spazio fisso per le prove di flotta dalla lezione 9.
- [ ] **Livello di programmazione della classe.** Se è basso, conviene spostare l'introduzione ad Arduino in un modulo a parte prima della lezione 1.
- [ ] **Presentazione finale**: solo interna al corso o aperta al pubblico.
- [ ] **Pesi della valutazione** e modalità d'esame previste dall'ordinamento ISIA.
