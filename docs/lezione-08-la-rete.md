# Lezione 8 · La rete

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

ESP-NOW, l'annuncio, i vicini e i messaggi. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con i dispositivi. È la prima lezione della fase Società: la creatura smette di essere sola e comincia a sentire chi ha intorno. Oggi la vita sociale si progetta e si prova tra i due individui di ogni gruppo; la prova con la flotta intera è la lezione 9.

## Obiettivi

Alla fine della giornata ogni studente sa:

- spiegare cosa offre ESP-NOW e cosa no: niente router, broadcast senza conferma, pacchetti piccoli, un canale solo;
- leggere un **annuncio** e la **tabella dei vicini**, e usare l'RSSI come una stima grossolana della vicinanza, con le sue tre fasce;
- usare i **sette messaggi comuni** e i **codici liberi** da 100 a 199, sapendo che ogni messaggio è un verso e non un comando;
- progettare la vita sociale di una specie con tre verbi: **reagire**, **mandare**, **ignorare**.

Ogni gruppo esce con la mappa sociale della propria specie, i propri codici documentati e due dispositivi che si sentono e si rispondono. La classe esce con il **registro dei codici di flotta**.

## Materiali da preparare

- Dispositivi: i 2 CoreS3 di ogni gruppo, carichi, con la piattaforma completa di rete (vedi Punti da decidere). Le 2 riserve con la specie neutra.
- 3 CoreS3 del docente con l'Esploratore CoreS3 (o la Raccolta), per la prova dal vivo della pagina ESP-NOW. Uno dei tre resta acceso tutto il giorno, per chi vuole rifare le misure.
- Figura 7 (rete e vicini) e Figura 8 (pacchetti) del documento di progetto, la tabella dei messaggi del §7.3 e l'appendice B stampate in A3, una per gruppo.
- **Scheda sociale** stampata, una per gruppo (modello in fondo a questo piano).
- Un metro a nastro da 10 m e nastro di carta per segnare a terra 0,5 m, 2 m, 5 m e 10 m.
- Un foglio grande o la lavagna per il **registro dei codici di flotta** del blocco E.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: un verso, non un cavo | 10' |
| 0:10 | ESP-NOW: cosa offre e cosa no | 20' |
| 0:30 | Annuncio e vicini | 25' |
| 0:55 | Prova dal vivo: la pagina ESP-NOW | 20' |
| 1:15 | I messaggi | 30' |
| 1:45 | Progettare una vita sociale | 15' |

### 0:00 · Apertura: un verso, non un cavo (10')

- Il principio 3 del documento: la radio è un linguaggio, non un cavo. Le creature non si scambiano comandi: emettono segnali che chi ascolta interpreta a modo suo. Nessuna creatura può obbligare un'altra a fare qualcosa.
- La metafora del §7.1: un messaggio è un verso. Lo emetti senza sapere chi sente, e se qualcuno risponde è lì. Non c'è una funzione di scoperta: ogni creatura annuncia sé stessa, chi la sente la ricorda, chi tace sparisce.
- Domanda alla classe: in una stanza buia, come sapete chi c'è e quanto è vicino? Voce, passi, respiro. Nessuno dice "sono qui" una volta sola: lo si ripete. È l'annuncio.

### 0:10 · ESP-NOW: cosa offre e cosa no (20')

ESP-NOW è il protocollo di Espressif che usa la radio Wi-Fi senza una rete Wi-Fi (§7.1). Si accende e funziona.

| Caratteristica | Valore nel documento | Conseguenza di progetto |
| --- | --- | --- |
| infrastruttura | nessuna: niente router, IP, associazione | la flotta funziona in qualsiasi stanza |
| destinatario | un dispositivo o tutti, in broadcast | il verso è per tutti; la chiamata è per uno |
| dimensione | al massimo 250 byte per pacchetto | si mandano stati e numeri, non immagini; i nostri pacchetti stanno sotto 24 byte (§7.4) |
| conferma | nessuna in broadcast | tutto ciò che conta si ripete |
| portata | 10–20 m in interni | basta per una stanza |
| canale | uno solo per tutti: il canale 1; il Wi-Fi vero resta spento | un dispositivo su un altro canale non esiste per la flotta |
| RSSI | intensità del segnale ricevuto | stima grossolana di chi è vicino |

**Il carico** (§7.5). Dieci creature che si annunciano ogni 2 secondi fanno 5 annunci al secondo; ESP-NOW ne regge centinaia. Il problema di una stanza affollata non è la banda, è il comportamento: lo si vede alla lezione 9.

**La versione del protocollo.** Ogni pacchetto porta nell'intestazione una versione: un dispositivo con un firmware vecchio viene ignorato, non frainteso (§7.4, §11.3). Una creatura che "non vede nessuno" ha spesso solo una piattaforma vecchia.

### 0:30 · Annuncio e vicini (25')

**L'annuncio** (§7.2, appendice B). Ogni 2 secondi, più un ritardo casuale fino a 0,5 s per non sovrapporsi, ogni creatura manda in broadcast:

- nell'intestazione (8 byte): sistema, versione, tipo, **specie**, **id**, numero progressivo;
- nel corpo (10 byte): **stato**, i quattro bisogni e la salute, i flag (in unisono, in mano, caricando, dorme), la generazione, la sessione di unisono.

Chi lo sente sa quindi chi c'è, di che specie è, come sta e cosa sta facendo, senza che nessuno abbia mandato un messaggio.

**Lo stato proprio in rete** (§4.4). Lo stato proprio si annuncia con un **codice comune**, uguale per tutte le specie; la specie nell'intestazione dice di chi è. Una creatura sa che una vicina è nel suo stato proprio, e di quale specie, senza sapere cosa significhi. Due conseguenze:

- ogni specie può reagire allo stato proprio delle altre senza conoscerlo: abbassare la voce, avvicinarsi, aspettare;
- il proprio stato proprio è visibile a tutta la stanza: fa parte della vita sociale anche se nessuno lo ha progettato per gli altri.

**La tabella dei vicini.** Chi riceve un annuncio aggiorna la propria tabella: RSSI lisciato, orario dell'ultimo annuncio. Valori dell'appendice A:

| Parametro | Valore |
| --- | --- |
| vicini al massimo | 16 |
| un vicino sparisce dopo | 10 s di silenzio, cioè 4 o 5 annunci persi di fila |
| lisciatura dell'RSSI | 0,3 |
| vicino | oltre −55 dBm |
| medio | da −55 a −75 dBm |
| lontano | sotto −75 dBm |

Alla lavagna: una serie di RSSI che oscilla tra −50 e −62 letta grezza e lisciata. Grezza, la vicina passa da "vicino" a "medio" ogni due annunci; lisciata, cambia fascia una volta sola. È di nuovo l'attrezzo del filtro della lezione 3.

**Le domande che la specie può fare** (§7.2): quanti vicini ci sono, quanti vicino, quanti della mia specie, chi è in unisono, chi è annoiato, chi è nel suo stato proprio e di quale specie. Non c'è una domanda "dov'è": l'RSSI dice quanto forte, non da dove.

### 0:55 · Prova dal vivo: la pagina ESP-NOW (20')

Tre dispositivi del docente con l'Esploratore, pagina **ESP-NOW** proiettata. Prima si dice cosa mostra la pagina, che **non è** la piattaforma: ogni dispositivo si annuncia una volta al secondo (non ogni 2), sul canale 1; la pagina elenca fino a 6 vicini per indirizzo MAC, con l'RSSI **grezzo**, gli annunci ricevuti, la percentuale di persi e da quanto tempo non si sentono (in grigio dopo 5 s).

Quattro prove, con la classe che annota i valori:

1. **Distanza.** Un dispositivo fermo, l'altro a 0,5 m, 2 m, 5 m e 10 m sui segni a terra. In quale fascia cade ogni distanza?
2. **Il corpo.** Alla stessa distanza: il dispositivo tenuto tra le mani, in tasca, con una persona in mezzo. Quanto scende l'RSSI? Il corpo di chi tiene la creatura cambia la sua "vicinanza".
3. **Il silenzio.** Si spegne un dispositivo: quanto passa prima che diventi grigio? Nella piattaforma il vicino sparirebbe dopo 10 s.
4. **La stanza piena.** Si accendono tutti i dispositivi dei gruppi con la stessa pagina: la percentuale di persi cambia? La pagina mostra solo 6 vicini: con dodici dispositivi, quali vede?

Conclusione da scrivere alla lavagna: la vicinanza è una stima. Una specie che fa dipendere qualcosa di importante da "vicino" deve tollerare che una vicina oscilli tra due fasce.

Se c'è tempo: la pagina **Wi-Fi** mostra quante reti occupano ogni canale nella stanza. Si guarda quanto è affollato il canale 1.

### 1:15 · I messaggi (30')

**La forma** (§7.3, appendice B). Un messaggio è un codice, un valore (intero con segno su 2 byte), un destinatario (un id oppure 255 per tutti) e 4 byte liberi, definiti dal codice. Il destinatario è un **id** della tabella di flotta (§3.5), non un nome.

**I sette messaggi comuni**, letti insieme. Per ognuno, la domanda di progetto è cosa decide la specie:

| Codice | Nome | Valore | Significato comune | Cosa decide la specie |
| --- | --- | --- | --- | --- |
| 1 | SALUTO | nessuno | sono qui e ti ho sentito; chi lo riceve guadagna socialità (+3) | quando salutare; se un saluto vale per lei più o meno di 3 |
| 2 | UMORE | umore, 0..100 | contagio | quanto lasciarsi influenzare, pesando sulla vicinanza |
| 3 | REGALO | quantità di nutrimento | una creatura ne nutre un'altra | se accettare, da chi |
| 4 | SPAVENTO | intensità, 0..100 | la paura si propaga | se e quanto spaventarsi, attenuato |
| 5 | CHIAMATA | nessuno; destinatario specifico | ti sto cercando | se rispondere o ignorare |
| 6 | RISPOSTA | nessuno; destinatario specifico | ci sono | cosa fare quando la risposta arriva, o non arriva |
| 7 | CAREZZA | intensità, 0..100 | affetto a distanza, tra creature legate | chi è "legata" per lei |

La piattaforma, da sola, fa già una cosa: in ANNOIATO la creatura manda un SALUTO ai vicini ogni minuto (§4.3).

**I codici liberi, da 100 a 199.** Semantica propria della specie, documentata. Un codice sconosciuto viene ignorato senza errori: le specie convivono anche se non si conoscono. Un problema da dire subito: i codici liberi sono gli stessi cento per tutte le specie. Se due gruppi usano entrambi il 120 con due significati, una creatura li confonde. Due attrezzi, da usare insieme: leggere un codice libero solo se la specie nell'intestazione è la propria; dividere i codici tra i gruppi (proposta nei Punti da decidere).

**Esempi dal §9.4**, per vedere un vocabolario sociale in numeri:

- la contagiosa manda UMORE ogni 30 s: 2 messaggi al minuto;
- la coppia manda una CHIAMATA alla gemella ogni minuto e le inoltra le carezze ricevute dalle persone come CAREZZA;
- la timida guarda solo quanti vicini ha e quanto sono vicini, e non manda quasi niente.

**Due regole già in vigore** (§7.3), che oggi si rispettano e alla lezione 9 si verificano con la flotta intera: nessuna creatura invia più di 5 messaggi al secondo; nessun messaggio ricevuto genera automaticamente un altro messaggio, tranne una RISPOSTA a una CHIAMATA.

### 1:45 · Progettare una vita sociale (15')

Tre verbi, una tabella. Per ogni cosa che la creatura può sentire dalla rete, il gruppo decide:

- **reagire**: cosa cambia nei bisogni, nello stato, nell'espressione;
- **mandare**: quale messaggio, a chi, quando, quante volte al minuto al massimo;
- **ignorare**: cosa non la riguarda. Ignorare è una scelta di carattere, non una mancanza.

Domande guida per il pomeriggio: *con quanti vicini sta bene? cosa fa quando resta sola? come tratta una creatura della sua specie e come una straniera? cosa fa quando una vicina è nel suo stato proprio? cosa manda di sua iniziativa, e perché proprio ogni tanti minuti?*

Il controllo da fare su ogni riga: "mando X **quando ricevo** Y" è vietato, tranne RISPOSTA a CHIAMATA. Un invio nasce da un gesto, da un tempo o da uno stato della creatura.

---

## Lavoro di gruppo · 5 ore con il pranzo · la vita sociale

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Due creature si sentono | 30' |
| 0:30 | B · La mappa sociale della specie | 60' |
| 1:30 | pranzo | 60' |
| 2:30 | C · Programmare la vita sociale | 90' |
| 4:00 | D · Prova a due, poi tra vicini | 35' |
| 4:35 | E · Giro dei tavoli e registro dei codici | 25' |

### A · Due creature si sentono (30')

1. Caricare la piattaforma con la rete e la specie su entrambi i dispositivi. Con `info` controllare che la versione del protocollo sia la stessa del dispositivo del docente.
2. Con il comando `vicini` e con la prima pagina dello schermo di debug (§6.5): ognuno dei due vede l'altro, con specie, stato e vicinanza?
3. **Le fasce nella stanza vera.** RSSI letto con `vicini` a 0,5 m, 2 m e 5 m, sul tavolo, in mano e in tasca, e con il modello d'involucro della lezione 7 montato. Annotare in quale fascia cade ogni situazione.
4. Forzare lo stato proprio su un dispositivo (`stato PROPRIO`) e controllare con `vicini` sull'altro che arrivi il codice comune.

### B · La mappa sociale della specie (60')

Sulla scheda sociale (modello sotto), riga per riga:

1. **Cosa sente** (20'): nessun vicino, uno, molti; vicino o lontano; della propria specie o di un'altra; una vicina annoiata, in unisono, che dorme, nel suo stato proprio. Per ogni situazione: reagire o ignorare, e come.
2. **Cosa riceve** (15'): i sette messaggi comuni. Per ognuno l'effetto sui bisogni e sull'espressione, oppure "ignora".
3. **Cosa manda** (15'): quali messaggi comuni, a chi, per quale gesto, tempo o stato, con la frequenza massima. Sommare i messaggi al minuto della specie nel caso peggiore.
4. **I codici propri** (10'): se servono, da uno a tre, ognuno con un nome, il valore, i 4 byte extra se usati, quando si manda, cosa fa chi lo riceve della stessa specie. Un codice proprio che dice la stessa cosa di un messaggio comune non serve.

Controllo finale, con il docente al tavolo: nessuna riga "mando X quando ricevo Y", tranne RISPOSTA a CHIAMATA.

### C · Programmare la vita sociale (90')

- In `creatura.h`: le reazioni ai messaggi e ai vicini, gli invii, i codici propri. In `config.h`: la socievolezza (vicini preferiti, minimo e massimo) e il contagio (§9.1).
- **Provare senza l'altro dispositivo.** Con `msg codice valore dest` da seriale si inietta un messaggio verso l'altro individuo, un messaggio alla volta, e si guarda la reazione sulla seriale e sullo schermo. Con `bisogno` e `stato` si mette la creatura nella situazione da provare.
- Ogni messaggio inviato compare sulla seriale su una riga marcata (§11.1): si tiene aperta la seriale di entrambi i dispositivi.
- Il docente passa ai tavoli e guarda soprattutto gli invii: quando partono e quanti sono.

### D · Prova a due, poi tra vicini (35')

1. **A due** (20'). I due individui del gruppo accesi insieme, con le persone che li curano normalmente: vicini, poi lontani, poi uno in tasca. Seriale registrata per 10 minuti su entrambi. Si contano i messaggi inviati da ciascuno.
2. **Tra vicini** (15'). Si avvicinano i dispositivi di un gruppo accanto. Le due specie si sentono: i codici propri dell'altra vengono ignorati? I messaggi comuni hanno l'effetto previsto? Qualcuno risponde a qualcosa a cui non dovrebbe?

### E · Giro dei tavoli e registro dei codici (25')

Ogni gruppo legge la propria scheda dei codici in 5 minuti: cosa manda, quanto spesso, cosa vuol dire. Il docente scrive alla lavagna il **registro dei codici di flotta**: per ogni codice da 100 a 199 in uso, la specie, il nome, la frequenza massima. Il registro è il punto di partenza della lezione 9.

---

## Consegna

Per la lezione 9, in un'unica cartella di gruppo:

1. **Specie sociale tra i due individui del gruppo**: `config.h` e `creatura.h`, provati a due.
2. **Codici propri documentati**: la scheda dei codici (modello sotto), uno per riga. Se la specie non ha codici propri, una riga che lo dice e perché.
3. **Mappa sociale**: la scheda del blocco B, aggiornata dopo la prova.
4. **Registro di 10 minuti** dalla seriale dei due dispositivi, con almeno uno scambio per ogni codice proprio e il conteggio dei messaggi al minuto di ciascun dispositivo.

### Modello della scheda sociale

| Cosa sente o riceve | Reagire (bisogni, stato, espressione) | Mandare (cosa, a chi) | Ignorare |
| --- | --- | --- | --- |
| nessun vicino per 5 min | | | |
| un vicino, vicino | | | |
| più di tre vicini | | | |
| una vicina della stessa specie | | | |
| una vicina di un'altra specie nel suo stato proprio | | | |
| una vicina annoiata | | | |
| SALUTO | | vietato | |
| UMORE | | vietato | |
| REGALO | | vietato | |
| SPAVENTO | | vietato | |
| CHIAMATA | | una RISPOSTA, al mittente, se la specie vuole | |
| RISPOSTA | | vietato | |
| CAREZZA | | vietato | |
| codice di un'altra specie | | vietato | sempre |

| Invio di sua iniziativa | Codice | Destinatario | Quando (gesto, tempo, stato) | Al minuto, al massimo |
| --- | --- | --- | --- | --- |
| | | | | |

### Modello della scheda dei codici

| Codice | Nome | Valore | Extra (4 byte) | Destinatario | Quando si manda | Al minuto, al massimo | Cosa fa chi lo riceve (stessa specie) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1__ | | | | | | | |

## Punti da decidere

- [ ] **La rete della piattaforma**: il repository non contiene ancora `rete.h` né il resto della piattaforma. Il programma chiede rete e unisono pronti prima di questa lezione; senza, la lezione si riduce alla pagina ESP-NOW dell'Esploratore e alla progettazione su carta.
- [ ] **Codici degli stati**: l'appendice B dice che l'annuncio porta il codice dello stato e che lo stato proprio ha un codice comune, ma non elenca i numeri. Vanno fissati, insieme all'intervallo dei codici di specie.
- [ ] **Codici liberi condivisi**: proposta di assegnare a ogni gruppo un blocco di 20 codici (gruppo 1: 100–119, gruppo 2: 120–139, e così via) e di leggere un codice libero solo se arriva dalla propria specie. Il §7.3 dice che le altre specie li ignorano, ma non dice come fanno a riconoscerli.
- [ ] **Messaggi con destinatario specifico**: il documento non dice esplicitamente che chi non è il destinatario li ignora. Questo piano lo dà per scontato.
- [ ] **Lisciatura dell'RSSI 0,3**: questo piano la legge come peso della nuova misura in una media mobile esponenziale. Va confermato nel codice.
- [ ] **Canale**: il §7.1 fissa il canale 1, il §12 prevede una scelta del canale in base al luogo e la pagina Wi-Fi dell'Esploratore suggerisce il meno affollato tra 1, 6 e 11. Decidere se il canale si può cambiare e come, per tutta la flotta insieme.
- [ ] **Flag "dorme"**: l'appendice B lo aggiunge ai flag dell'annuncio, il §7.2 no. Allineare i due.
- [ ] **Creature "legate"** per la CAREZZA: chi decide il legame, la specie o la tabella di flotta.
- [ ] **L'osservatore** già da oggi: il programma lo prevede dalla lezione 9, ma se è pronto aiuterebbe nel blocco D.
