# Lezione 7 · Corpo fisico e osservazione

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

L'involucro e i metodi per osservare una creatura. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con i dispositivi e con il laboratorio di fabbricazione, disponibile da oggi. È la giornata della **revisione intermedia**, la prima delle due verifiche valutate: ogni gruppo fa provare la propria specie v0 a persone di altri gruppi, senza spiegazioni, e la guarda vivere per un'ora.

## Obiettivi

Alla fine della giornata ogni studente sa:

- leggere la **regola 10** del design system come una lista di vincoli fisici, e dire per ciascuno quale senso o quale gesto si rompe se non lo si rispetta;
- condurre un **playtest** senza aiutare chi prova, e separare quello che si vede da quello che si interpreta;
- registrare un'ora di vita da seriale e trasformarla nel **grafico di vita** della lezione 2: quattro linee e la striscia degli stati;
- confrontare la **vita prevista** (il simulatore) con la **vita registrata** (il dispositivo) e spiegare dove divergono.

Ogni gruppo esce con le griglie del playtest compilate da due gruppi diversi, il grafico di un'ora di vita, i primi schizzi e un primo modello dell'involucro. Il docente esce con una scheda di revisione per ogni specie.

## Materiali da preparare

- Dispositivi: i 2 CoreS3 di ogni gruppo con la specie v0 della lezione 6, carichi. Le 2 riserve con la specie neutra, per sostituire un dispositivo che si blocca durante il playtest.
- Un portatile per gruppo con la seriale funzionante e un modo per salvarla su file (vedi Punti da decidere). Cavi USB-C lunghi almeno 1 m.
- **Griglia di osservazione** stampata: 2 copie per gruppo per ciascuno dei due giri (modello in fondo a questo piano).
- **Scheda di revisione** stampata, una per gruppo, per il docente (modello in fondo).
- Il simulatore di vita ([`simulatore-vita-creatura.html`](simulatore-vita-creatura.html)) aperto su ogni portatile.
- Per l'involucro: sagoma 54 × 54 mm in cartoncino, una per gruppo; cartone vegetale da 2–3 mm, schiuma, cutter, righello, calibro, nastro di carta, colla a caldo. Il regolamento del laboratorio e il calendario delle prenotazioni.
- La tabella delle misure di ogni gruppo (consegna della lezione 3), per ripetere tre misure con il modello d'involucro.
- Un CoreS3 del docente aperto sul tavolo, per mostrare dove stanno schermo, microfoni, altoparlante, USB-C e finestra del sensore di luce.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: la revisione di oggi | 10' |
| 0:10 | L'involucro e la regola 10 | 35' |
| 0:45 | Osservare: il playtest | 30' |
| 1:15 | La vita come grafico | 30' |
| 1:45 | Il protocollo della giornata | 15' |

### 0:00 · Apertura: la revisione di oggi (10')

- Dove siamo: fase Espressione, lezioni 4–7. Ogni gruppo ha una specie v0 che vive su due dispositivi. Da domani conta la flotta: lezione 8 la rete, lezione 9 la prima prova con tutte le specie.
- Cosa si valuta oggi. La valutazione guarda il **comportamento nella stanza**, non il codice. Dei cinque criteri finali oggi se ne guardano quattro: carattere e leggibilità, interazione, comportamento nel tempo, involucro (solo come progetto). La vita sociale e l'unisono arrivano dopo.
- Il principio della giornata: una creatura si giudica da come la tratta **chi non la conosce**. Chi l'ha progettata sa già cosa vuol dire ogni gesto e non è un buon testimone.

### 0:10 · L'involucro e la regola 10 (35')

**La regola, frase per frase** (§10, regola 10): involucro sulla stessa impronta del Core (54 × 54 mm), materiale libero; schermo, microfono, altoparlante, USB-C e finestra del sensore di luce liberi; rigido, perché l'IMU senta la mano e non il guscio; niente metallo intorno all'antenna, sul bordo superiore.

Ogni vincolo protegge un pezzo della piattaforma:

| Vincolo | Cosa protegge | Cosa si rompe se non lo si rispetta |
| --- | --- | --- |
| schermo scoperto | espressione; touch (§12) | carezza, tap, pressione lunga, debug a 3 s, rinascita a 5 s |
| microfono libero | rumore, voce vicina, colpo sonoro | "parlare vicino" non scatta più; lo spavento da colpo sonoro neppure |
| altoparlante libero | suono, voce nell'unisono | un altoparlante da 1 W già debole sotto il do4 (§10.1) sparisce nel coro |
| USB-C accessibile | carica, seriale, caricamento dei binari | ogni prova richiede di smontare il guscio |
| finestra della luce libera | buio, coperto, mano vicina, contesto in tasca | "coprire" e "dormire al buio" diventano impossibili o scattano sempre |
| rigidità | scossa, in mano, fermo, tap | un guscio morbido può smorzare la scossa sotto 1,5 g e aggiungere rimbalzi che sembrano gesti |
| niente metallo sul bordo superiore | radio (§7) | l'RSSI cala, i vicini sembrano lontani, i pacchetti si perdono |

**Dimostrazione (5').** Il CoreS3 del docente aperto sul tavolo: si individuano insieme le finestre sul dispositivo vero e si segnano sulla sagoma 54 × 54. Le posizioni esatte si misurano con il calibro sui dispositivi della flotta, non si copiano da una foto.

**Tre domande di progetto**, che la regola non risolve:

- **Dove si tiene.** La creatura viene presa in mano, capovolta, coperta, portata in tasca. Un involucro che non entra in tasca toglie alla specie il contesto "in tasca" (§5.3): è una scelta di carattere, va fatta apposta.
- **Cosa tocca la mano.** I gesti touch sono sullo schermo. Un bordo alto intorno allo schermo rende difficile la carezza (oltre 40 px, sotto 150 px al secondo).
- **Cosa resta della base.** La base standard ha quattro magneti che saturano la bussola (lezione 3). Un involucro con magneti propri, per esempio per chiudersi, peggiora la stessa cosa.

**Il laboratorio.** Stampa 3D o taglio laser, da oggi. Una stampa richiede ore e una coda: si arriva al laboratorio con un modello in cartone già provato sul dispositivo, non con un'idea. Oggi si lavora in cartone e schiuma; il file per la fabbricazione è il passo successivo.

### 0:45 · Osservare: il playtest (30')

**Cos'è.** Una persona che non conosce la specie riceve la creatura e ci passa del tempo. Chi l'ha progettata guarda e scrive. Non spiega, non suggerisce, non difende.

**Le regole del playtest:**

1. Una sola frase di consegna, uguale per tutti: "Questa è una creatura. Per venti minuti è vostra."
2. Chi prova parla ad alta voce: cosa vuole fare, cosa pensa che la creatura provi.
3. Chi osserva non risponde alle domande. Se chi prova chiede "cosa vuole?", la risposta è "voi cosa pensate?".
4. Si scrive il **minuto**, il **gesto** e la **reazione**, poi a parte l'**interpretazione** di chi prova.
5. Le domande si fanno dopo, mai durante.

**Vedere e interpretare** (esercizio lampo, 8'). Il docente fa una prova di due minuti con un dispositivo della neutra davanti alla classe. Tutti scrivono tre note. Si leggono ad alta voce e si dividono in due colonne:

| Nota osservata | Nota interpretata |
| --- | --- |
| al minuto 1:20 l'ha capovolta, lo schermo si è scurito | si è addormentata perché era stanca |
| ha accarezzato tre volte, la forma si è aperta | le piace essere accarezzata |

La prima colonna si può verificare con il grafico di vita, la seconda no. Una griglia di playtest serve a riempire soprattutto la prima.

**Cosa misurare.** Il tempo al primo gesto; quali dei nove gesti vengono provati; per ogni gesto se la reazione viene notata; quante volte il significato viene indovinato rispetto alla tabella gesto → significato della lezione 4; se il sonno viene riconosciuto (regola 9: deve esserlo da chiunque); se lo stato proprio, quando capita, viene distinto dal sonno (§4.4). La griglia completa è in fondo al piano.

**Gli errori tipici di chi osserva:** aiutare con lo sguardo o con una risata; scrivere solo quando la creatura fa la cosa prevista; scambiare la cortesia di chi prova per comprensione.

### 1:15 · La vita come grafico (30')

**La seriale** (§11.1). A 115200 baud, una riga al secondo, valori separati da virgola: tempo, stato, i quattro bisogni, salute, contesto, numero di vicini, RSSI medio, sessione di unisono, batteria. Gli eventi (gesti, messaggi, transizioni) arrivano su righe marcate, quando avvengono. Un'ora sono circa 3600 righe più gli eventi.

**Come si registra:**

- fattore tempo a 1 (`tempo 1`), orario impostato (`ora AAAA-MM-GG HH:MM`): la fascia del giorno conta per lo stato proprio di più di una specie;
- il dispositivo resta collegato al portatile per tutta l'ora, il portatile non va in sospensione;
- la seriale si salva su file, non si copia dallo schermo alla fine;
- prima di cominciare si annota l'ora di inizio e, durante, i momenti notevoli (arriva chi prova, la creatura passa di mano, pranzo).

**Dal file al grafico.** Si tolgono le righe marcate degli eventi, si incolla il resto in un foglio di calcolo, si fanno quattro linee dei bisogni e, sotto, una striscia colorata per lo stato. È lo stesso grafico della lezione 2. Gli eventi tolti si rimettono come segni verticali sul grafico: una carezza, uno spavento, una transizione.

**Vita prevista e vita registrata.** Il simulatore gioca la stessa specie con gli stessi coefficienti e lo stesso stato proprio, in turni da 10 minuti: un'ora sono 6 turni. Il confronto è di tendenza, non di valori, perché il simulatore ha due soglie ritoccate e cure da un turno (lezione 2). Le domande:

- i bisogni scendono con la pendenza attesa?
- gli stati compaiono nell'ordine e con la durata attesi?
- c'è un difetto della lezione 2 (oscillazione, assillo, stato pozzo, stato irraggiungibile) che sulla carta non c'era?
- il carattere si vede nel grafico anche senza guardare la creatura?

### 1:45 · Il protocollo della giornata (15')

- I due giri del playtest e chi va dove (rotazione nel blocco B).
- Quale dispositivo va in prova e quale registra.
- La consegna di fine giornata e il colloquio di revisione del blocco E.
- Le regole del laboratorio e come si prenota.

---

## Lavoro di gruppo · 5 ore con il pranzo · la revisione intermedia

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Preparare la prova e prevedere la vita | 30' |
| 0:30 | B · Playtest incrociato, due giri | 90' |
| 2:00 | pranzo | 60' |
| 3:00 | C · La vita registrata | 40' |
| 3:40 | D · Primi modelli dell'involucro | 50' |
| 4:30 | E · Colloquio di revisione | 30' |

### A · Preparare la prova e prevedere la vita (30')

1. Caricare la specie v0 su entrambi i dispositivi, con la stessa versione. Da seriale: `info`, e si copia la riga nella cartella di gruppo.
2. Su entrambi: `tempo 1` e `ora` con l'orario vero. Controllare che i parametri siano quelli della specie e non residui di prove (`get *`; se serve, `fabbrica`).
3. Scegliere i ruoli dei dispositivi. Il **dispositivo A** va in prova, libero, senza cavo. Il **dispositivo B** resta sul tavolo del playtest collegato al portatile e registra.
4. **Vita prevista.** Nel simulatore: i coefficienti e lo stato proprio della specie, mazzo "distratta", un seme scritto sul foglio. Si copia la partita come CSV e si segnano i primi 6 turni, cioè la prima ora.
5. Alle 0:30 si avvia la registrazione su B e si annota l'ora.

### B · Playtest incrociato, due giri (90')

Ogni gruppo si divide in due metà. In ogni giro una metà resta al proprio tavolo e osserva, l'altra va a provare la specie di un altro gruppo. Con n gruppi, il gruppo i manda i suoi al gruppo i + 1 nel primo giro e al gruppo i + 2 nel secondo (contando in cerchio). Così ogni specie viene provata da due gruppi diversi, e ogni studente fa una volta chi prova e una volta chi osserva.

Ogni giro dura 45':

| Minuti | Cosa succede |
| --- | --- |
| 0–5 | arrivo; una sola frase di consegna; il dispositivo A passa di mano |
| 5–30 | prova libera, ad alta voce; chi osserva riempie il registro dei gesti |
| 30–40 | le cinque domande dopo (in fondo al piano), con le risposte scritte parola per parola |
| 40–45 | cambio |

Durante la prova chi osserva annota anche lo stato che **crede** di vedere su A. Se un gruppo non riesce a dire in che stato è la propria creatura guardandola, è già un risultato.

Il dispositivo B resta sul tavolo, collegato: chi prova lo può toccare, ma non lo porta via. A e B sono vicini, quindi la socialità di entrambi sale (§4.1): nel grafico va ricordato.

Il docente gira tra i tavoli con la scheda di revisione e guarda soprattutto le prove, non i portatili.

### C · La vita registrata (40')

La registrazione è partita alle 0:30 e ha coperto i due giri: se ne usa un'ora piena.

1. Pulire il file e fare il grafico come nella parte frontale (15').
2. Segnare sul grafico i due giri del playtest e i momenti notevoli annotati (5').
3. Mettere accanto la vita prevista del blocco A e scrivere **tre differenze**, ciascuna con la causa probabile: un parametro, una soglia, un gesto non previsto, la vicinanza dell'altro dispositivo (15').
4. Dalle griglie dei due giri e dal grafico, scegliere le **tre correzioni** più importanti per la specie (5').

### D · Primi modelli dell'involucro (50')

1. Dagli schizzi a un modello 1:1 in cartone o schiuma, sulla sagoma 54 × 54 mm (25').
2. Montarlo sul dispositivo e controllare le finestre: lo schermo si accarezza tutto? La finestra della luce è libera? L'USB-C entra senza smontare? (10')
3. **Tre misure di controllo**, ripetute con il modello montato e confrontate con la tabella della lezione 3: scossa leggera, tap, coperto dalla mano. I valori grezzi si leggono sulla seconda pagina dello schermo di debug (§6.5), senza ricaricare l'Esploratore (10').
4. Prenotare il laboratorio per il modello successivo, con un'idea del processo: stampa o taglio (5').

### E · Colloquio di revisione (30')

Sei minuti per gruppo, al tavolo, con la creatura accesa: il grafico di un'ora, le griglie dei due giri, gli schizzi e il modello, le tre correzioni. Il docente compila la scheda di revisione e la consegna al gruppo alla lezione 8.

---

## Consegna

Revisione intermedia, a fine giornata, in un'unica cartella di gruppo:

1. **Specie v0**: `config.h` e `creatura.h` della versione provata oggi, con la riga di `info` dei due dispositivi.
2. **Un'ora di vita registrata da seriale**: il file grezzo, il grafico (quattro linee e striscia degli stati, con i giri del playtest segnati) e, accanto, la vita prevista dal simulatore con le tre differenze.
3. **Schizzi dell'involucro**: almeno tre, in scala 1:1 sulla sagoma 54 × 54 mm, con le finestre, la zona dell'antenna, il materiale e il modo in cui si tiene. La foto del modello del blocco D e le tre misure di controllo.

A corredo, per il colloquio: le quattro griglie di osservazione ricevute e le tre correzioni scelte.

### Modello della griglia di osservazione

Specie provata: ______ · Giro: 1 / 2 · Chi prova (gruppo): ______ · Chi osserva: ______ · Inizio: __:__

**Registro dei gesti**

| Minuto | Gesto (dei nove) | Cosa ha fatto la creatura | Stato che crediamo di vedere | Cosa ha detto chi prova |
| --- | --- | --- | --- | --- |
| | | | | |

**Riepilogo**

| Voce | Valore |
| --- | --- |
| tempo al primo gesto | s |
| gesti provati, sui nove | /9 |
| gesti con reazione notata da chi prova | /9 |
| significati indovinati (confronto con la tabella della lezione 4) | sì __ · in parte __ · no __ |
| il sonno, se è capitato, è stato riconosciuto | sì / no / non capitato |
| lo stato proprio, se è capitato, è stato distinto dal sonno | sì / no / non capitato |
| momenti in cui chi prova voleva fare qualcosa e la creatura non lo permetteva | |
| la frase più rivelatrice detta da chi prova, parola per parola | |

**Le cinque domande dopo**

1. Come stava la creatura quando l'avete lasciata?
2. Cosa le piaceva? Cosa la disturbava?
3. C'è stato un momento in cui ha fatto qualcosa da sola, senza che la toccaste?
4. Se doveste darle da mangiare, come fareste?
5. La terreste con voi per una giornata? Perché?

### Modello della scheda di revisione (per il docente)

| Criterio | Cosa si guarda oggi | 0 · 1 · 2 · 3 | Note |
| --- | --- | --- | --- |
| Carattere e leggibilità | chi prova capisce cosa prova la creatura senza volto né testo; il sonno si riconosce; la grammatica dei bisogni è rispettata | | |
| Interazione | i gesti provati hanno reazioni notate; i significati si indovinano o si scoprono con piacere | | |
| Comportamento nel tempo | il grafico di un'ora mostra cambiamenti anche senza cure; la differenza tra previsto e registrato è spiegata | | |
| Involucro (progetto) | gli schizzi rispettano la regola 10; il modello lascia libere le finestre; le misure di controllo sono fatte | | |
| Metodo | il playtest è stato condotto senza spiegazioni; le correzioni scelte nascono dalle osservazioni | | |

## Punti da decidere

- [ ] **Piattaforma**: il repository oggi non contiene il codice della piattaforma (nucleo, strumenti, seriale, schermo di debug). La lezione presuppone che sia stato distribuito alla lezione 6 e che la riga seriale del §11.1 e i comandi del §11.2 funzionino.
- [ ] **Formato della riga seriale**: il §11.1 elenca i campi ma non l'ordine, un'intestazione o il segno delle righe di evento. Va fissato prima di oggi, perché ogni gruppo fa lo stesso grafico.
- [ ] **Registrazione su file**: il repository non ha un registratore generico (`banco.py` serve solo a TestSincronia). Scrivere un piccolo script Python con pyserial, oppure indicare un terminale che salva su file. In alternativa, la microSD facoltativa del programma, se la piattaforma scriverà la riga seriale anche su scheda.
- [ ] **"Un'ora di vita"**: questo piano la intende come un'ora vera a fattore tempo 1. Va confermato, perché a fattore 10 un'ora di vita della creatura dura 6 minuti.
- [ ] **Impronta 54 × 54 mm**: decidere se è l'ingombro massimo in pianta dell'involucro o solo la base su cui il Core si appoggia. La regola 10 si può leggere nei due modi e cambia molto il progetto.
- [ ] **Posizioni delle finestre**: il documento indica solo l'antenna sul bordo superiore. Misurare sui dispositivi della flotta la posizione di microfoni, altoparlante, USB-C e finestra della luce, e preparare una sagoma quotata uguale per tutti.
- [ ] **Base standard**: se l'involucro la tiene (con i magneti) o la sostituisce, e cosa resta dell'alimentazione e del fissaggio.
- [ ] **Peso della revisione** sul voto finale e scala della scheda di revisione: il programma fissa i criteri della consegna finale, non quelli della revisione intermedia.
- [ ] **Ore e regole del laboratorio** di fabbricazione: chi ci accede, quante prenotazioni per gruppo, chi paga il materiale.
