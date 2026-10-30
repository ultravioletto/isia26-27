# Lezione 6 · La piattaforma

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

Architettura a livelli, ciclo senza attese, struttura dei file, cosa la specie può fare e cosa no, gli strumenti di lavoro. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, sul dispositivo: è la lezione in cui ogni gruppo riceve la piattaforma con la specie neutra e la trasforma nella propria.

> **Attenzione.** Alla data di questa bozza la piattaforma non esiste nel repository: `code/arduino` contiene solo le prove (Esploratore, ProvaSonora, TestSincronia, le visualizzazioni e la Raccolta). Il piano descrive la lezione con la piattaforma pronta e dà un piano B nei Punti da decidere.

## Obiettivi

Alla fine della giornata ogni studente sa:

- disegnare i **livelli** del sistema e dire dove passa la linea tra piattaforma e specie;
- spiegare perché il ciclo principale **non aspetta mai** e cosa vuol dire per il codice della specie;
- orientarsi nei file: cosa c'è in `config.h` (valori) e in `creatura.h` (reazioni e disegno), e perché gli altri non si toccano;
- usare gli **strumenti di lavoro**: la riga di seriale, i comandi `set` e `get`, il fattore tempo, l'iniezione di eventi, lo schermo di debug.

Ogni gruppo esce con la **specie v0** che vive su due dispositivi: i parametri della lezione 2, i gesti della lezione 4, la forma della lezione 4, l'identità della lezione 5 e una prima versione dello stato proprio.

## Materiali da preparare

- **La piattaforma con la specie neutra**, compilata e provata su due dispositivi del docente almeno due giorni prima. Una cartella per gruppo, già con il nome della specie, pronta da copiare sui portatili.
- **`flotta.h`** compilato con i MAC del registro dei dispositivi della lezione 3.
- Un dispositivo del docente con la neutra, collegato al proiettore con il monitor seriale aperto.
- Le figure 2 (livelli) e 3 (ciclo di esecuzione) del documento di progetto e la tabella del §3.4 stampate in A3, una per gruppo.
- La tabella dei comandi del §11.2 stampata in A4, una per persona.
- Le consegne delle lezioni 2, 4 e 5 di ogni gruppo: tabella dei parametri, scheda dello stato proprio, tabella dei gesti, `disegnaForma()`, `config.h` d'identità.
- Un foglio di calcolo vuoto su ogni portatile, per incollare la seriale.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: dalla prova alla creatura | 10' |
| 0:10 | Livelli e struttura dei file | 25' |
| 0:35 | Il ciclo senza attese | 20' |
| 0:55 | Cosa la specie può fare e cosa no | 25' |
| 1:20 | Gli strumenti di lavoro | 30' |
| 1:50 | Consegna della piattaforma e regole | 10' |

### 0:00 · Apertura: dalla prova alla creatura (10')

- Fino a oggi ogni pezzo è stato provato da solo: la vita su carta e nel simulatore (lezione 2), i sensi con l'Esploratore (lezione 3), la forma con lo sketch di prova (lezione 4), colore e suono (lezione 5). Oggi i pezzi si montano.
- Il principio 5: una piattaforma, molte specie. Tutto ciò che è comune sta nella piattaforma e non si modifica; tutto ciò che è carattere sta in due file.
- Il principio 6: attrito zero. Il tempo si spende sul comportamento, non sulla compilazione. Per questo quasi tutto si cambia da seriale, senza ricompilare.

### 0:10 · Livelli e struttura dei file (25')

- La Figura 2: quattro livelli, una linea netta. Sotto la linea nessuno modifica niente, sopra la linea ogni specie è libera. La specie vede la piattaforma solo attraverso funzioni leggibili: eventi, bisogni, uscite, vicini, messaggi, unisono. Non tocca mai l'hardware.
- La struttura del progetto (§3.4):

| File | Contenuto | Chi lo modifica |
| --- | --- | --- |
| `Creatura.ino` | avvio, ciclo principale, cablaggio dei moduli | piattaforma |
| `nucleo.h` | bisogni, salute, stati e transizioni, cornice dello stato proprio, tempo spento, memoria | piattaforma |
| `sensi.h` | filtri, eventi e gesti, contesto | piattaforma |
| `espressione.h` | canali, intensità, convenzione del sonno, debug, tabelle d'onda | piattaforma |
| `rete.h` | ESP-NOW, annuncio, vicini, messaggi | piattaforma |
| `unisono.h` | sessione, orologio, strumento, voce autonoma | piattaforma |
| `strumenti.h` | seriale, comandi, parametri, informazioni di flotta | piattaforma |
| `flotta.h` | MAC → id e nome | chi gestisce la flotta |
| `config.h` | identità, carattere, soglie, stato proprio, palette, timbro, unisono | **la specie** |
| `creatura.h` | reazioni a eventi e messaggi, stato proprio, disegno, parte di unisono | **la specie** |

- **Un binario per specie** (§2, §11.3): la stessa piattaforma compilata con i due file del gruppo. Lo stesso binario va su entrambi i dispositivi; `flotta.h` dà a ciascuno id e nome dal MAC, e il seme individuale fa variare i coefficienti di carattere del ±10% (§3.5). Due individui della stessa specie si somigliano ma non sono identici.
- **Valori e logica.** `config.h` contiene numeri che si possono cambiare anche da seriale. `creatura.h` contiene decisioni: se succede questo, fai quello. Domanda alla classe: la soglia della fame è un valore o una logica? E "si addormenta solo di sera"?

### 0:35 · Il ciclo senza attese (20')

- La Figura 3: un solo ciclo, nessuna attesa bloccante. Ogni modulo lavora alla sua frequenza contando il tempo:

| Cosa | Frequenza |
| --- | --- |
| tick del nucleo: eventi e transizioni | 10 Hz |
| decadimento dei bisogni | 1 volta al secondo |
| disegno | 30 fotogrammi al secondo |
| riga di seriale | 1 volta al secondo |
| annuncio radio | ogni 2 s, più fino a 0,5 s casuali |
| salvataggio in memoria | ogni 60 s e a ogni cambio di stato |

- **Cosa vuol dire per la specie.** Nessun `delay()`, nessun ciclo che aspetta un tocco, nessun calcolo lungo dentro una reazione. Una reazione decide e torna subito; ciò che deve durare nel tempo si scrive come uno stato o un timer.
- **Esempio alla lavagna.** "Quando la accarezzi, lampeggia tre volte." Scritto con `delay()` ferma i sensi e la radio per un secondo e mezzo. Scritto bene è un contatore che il disegno legge a ogni fotogramma.
- Il ponte con la lezione 4: la `disegnaForma()` che usa il tempo trascorso si sposta nella piattaforma senza cambiare. Quella che usa i fotogrammi va corretta.

### 0:55 · Cosa la specie può fare e cosa no (25')

**Può** (§9.1):

- **Carattere**: coefficienti di decadimento, soglie di fame, sonno, noia e spavento, sensibilità, socievolezza, contagio, curiosità, resilienza. In `config.h`.
- **Reazioni**: il significato dei nove gesti, il comportamento per contesto, la risposta ai messaggi. In `creatura.h`.
- **Espressione**: la forma di base e come la grammatica la deforma, il timbro, come rende le tre intensità.
- **Stato proprio**: nome e significato (solo nella documentazione e nel debug), condizioni d'ingresso e d'uscita in `creatura.h`, coefficienti di decadimento da 0 a 2, durata massima e intervallo minimo in `config.h`.
- **Memoria**: leggere i contatori di vita (§4.6) per comportamenti a lungo termine.

**Non può**:

- aggiungere bisogni o sensi, o cambiare gli stati comuni: li interpreta;
- leggere i sensori grezzi o toccare l'hardware;
- urlare oltre i limiti: l'urgente dura 10 s, poi 60 s di raffreddamento; il suono non supera il 10% del tempo;
- scrivere testo in modalità normale;
- allungare lo stato proprio oltre 10 minuti o accorciare l'intervallo sotto 20; nasconderci una malattia; farlo sopravvivere allo spegnimento;
- intasare la radio o costringere le altre creature (lezioni 8 e 9).

**La cornice dello stato proprio** (§4.4), ripassata con la Figura 4: si entra solo da SVEGLIO e si torna a SVEGLIO; priorità subito sotto SPAVENTATO; MORTO, UNISONO, MALATO e SPAVENTATO lo interrompono sempre e ci si torna con i timer dove erano. Una specie senza stato proprio lascia vuota la condizione d'ingresso. I limiti di 10 e 20 minuti sono provvisori (§12.1, punto 9): i gruppi li mettono alla prova oggi.

**La specie neutra** è il punto di partenza di tutti: parametri dell'appendice A, gesti del §5.2, nessuno stato proprio. È anche la specie delle riserve.

### 1:20 · Gli strumenti di lavoro (30')

**La seriale** (§11.1). A 115200 baud, una riga al secondo, valori separati da virgola: tempo, stato, i quattro bisogni, salute, contesto, vicini, RSSI medio, sessione di unisono, batteria. Gli eventi (gesti, messaggi, transizioni) su righe marcate. Una sessione di seriale incollata in un foglio di calcolo è il grafico di vita della lezione 2.

**I comandi** (§11.2):

| Comando | A cosa serve oggi |
| --- | --- |
| `set nome valore`, `get nome`, `get *` | regolare un parametro senza ricompilare |
| `save`, `load`, `fabbrica` | tenere una regolazione, tornare ai valori della specie |
| `tempo n` | vivere un'ora in sei minuti con `tempo 10` |
| `evento nome` | provare una reazione senza fare il gesto |
| `stato NOME` | guardare l'espressione di uno stato, compreso PROPRIO |
| `bisogno nome valore` | portare un bisogno dove serve, come alla lezione 4 |
| `ora AAAA-MM-GG HH:MM` | impostare l'RTC, per gli stati che dipendono dall'ora |
| `info` | id, nome, specie, MAC, versioni, generazione, contatori |

- **`set` lavora in RAM.** Un valore trovato con `set` si perde al riavvio se non si fa `save`, e si perde comunque al cambio di binario se non lo si riporta in `config.h`. La regola del corso: ciò che funziona finisce in `config.h` entro fine giornata.
- **Il fattore tempo** scala tutto, cure comprese (§4.5). Si progetta a velocità 1, si prova a velocità 10. Attenzione: se il fattore scala anche i limiti delle cure, a `tempo 10` il limite di un pasto ogni 2 minuti diventa un pasto ogni 12 secondi.
- **Lo schermo di debug** (§6.5): pressione lunga di 3 s nell'angolo in basso a destra. Due pagine: stato, bisogni, salute, generazione, vicini, batteria, id, nome, MAC, versione; poi i valori grezzi dei sensori. Si chiude allo stesso modo o da solo dopo 30 s. È l'unico posto dove compare testo.

**Dimostrazione dal vivo (12').** Sul dispositivo del docente con la neutra: `tempo 10`, `bisogno nutrimento 35`, si guarda la fame arrivare dalla seriale e dallo schermo; `evento carezza`; `stato DORME`; poi una pressione lunga vera per nutrirla. Infine `fabbrica` e `tempo 1`.

### 1:50 · Consegna della piattaforma e regole (10')

- Ogni gruppo copia la propria cartella sul portatile. Regole: si modificano solo `config.h` e `creatura.h`; un errore nella piattaforma si segnala al docente, non si corregge; ogni file della specie si salva nella cartella di gruppo a fine giornata.
- Il protocollo del pomeriggio, blocco per blocco.

---

## Lavoro di gruppo · 5 ore con il pranzo · dalla neutra alla specie

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · La neutra sui due dispositivi | 40' |
| 0:40 | B · `config.h`: identità e carattere | 50' |
| 1:30 | pranzo | 60' |
| 2:30 | C · `creatura.h`: gesti e stato proprio | 60' |
| 3:30 | D · La forma nella piattaforma | 45' |
| 4:15 | E · Un'ora di vita in sei minuti | 45' |

### A · La neutra sui due dispositivi (40')

1. Compilare la piattaforma con la neutra e caricarla sui due dispositivi (15'). Circa un minuto per dispositivo; la prima compilazione è più lunga.
2. Con `info`, controllare che ogni dispositivo abbia l'id e il nome del registro (5').
3. Fare i nove gesti sulla neutra e guardare cosa succede sullo schermo e sulla seriale (10').
4. Aprire lo schermo di debug e chiuderlo (5').
5. Leggere `config.h` e `creatura.h` della neutra, con la tabella dei gesti della lezione 4 accanto (5').

### B · `config.h`: identità e carattere (50')

1. **Identità** (10'): copiare la parte della lezione 5 (codice, nome, palette, timbro) e adattarla ai nomi della piattaforma.
2. **Carattere** (25'): riportare la tabella dei parametri della lezione 2, con le correzioni emerse dallo scambio e dal simulatore. Ogni valore con il commento di una riga.
3. **Stato proprio** (10'): durata massima e intervallo minimo, dentro i limiti di 10 e 20 minuti; coefficienti di decadimento durante lo stato, da 0 a 2.
4. Compilare, caricare, controllare con `get *` che i valori siano quelli scritti (5').

### C · `creatura.h`: gesti e stato proprio (60')

1. **I nove gesti** (30'): partire dalle reazioni della neutra e riscriverle con la tabella della lezione 4. Provare ogni gesto due volte: con la mano e con `evento nome`.
2. **Lo stato proprio** (25'): condizione d'ingresso e d'uscita con quello che la creatura sa (bisogni, eventi, contesto, ora, contatori). Provarlo così:
   - forzarlo con `stato PROPRIO` per vederne l'espressione;
   - portare la creatura nella condizione d'ingresso con `bisogno`, `evento` o `ora` e controllare che ci entri da sola;
   - controllare sulla seriale che ne esca entro la durata massima e che non ci rientri prima dell'intervallo minimo.
3. Se lo stato proprio dipende dai vicini e la rete non è ancora nella piattaforma, lo si prova solo con `stato PROPRIO` e si scrive la condizione per la lezione 8 (5').

### D · La forma nella piattaforma (45')

1. Spostare la `disegnaForma()` della lezione 4 nella funzione di disegno di `creatura.h` (20').
2. Collegarla alla grammatica, all'intensità e allo stato che la piattaforma passa al disegno, invece che ai comandi della prova (15').
3. Controllare con `stato NOME` lo stato DORME, l'urgente e lo stato proprio: il sonno deve restare quello del §6.4 e lo stato proprio non deve imitarlo (10').

### E · Un'ora di vita in sei minuti (45')

1. Sui due dispositivi: `tempo 10`, poi un gruppo cura la creatura come il mazzo "distratta" della lezione 2: risponde a una richiesta su due, a caso (15').
2. Copiare la seriale nel foglio di calcolo e disegnare il grafico di vita: quattro linee e la striscia degli stati (15').
3. Confrontarlo con la giornata tipo della lezione 2 (10'). Dove i due grafici divergono, il carattere non è ancora nei numeri: si corregge `config.h` con `set`, si prova, si riporta nel file.
4. `tempo 1` e `save` su entrambi i dispositivi prima di spegnere (5').

---

## Consegna

Per la lezione 7, in un'unica cartella di gruppo:

1. **Specie v0 che vive su due dispositivi**: `config.h` e `creatura.h`, che compilano con la piattaforma distribuita oggi, caricati su entrambi i dispositivi.
2. Il **grafico di vita** del blocco E, con la seriale da cui è nato.
3. Una nota di mezza pagina: cosa la specie fa diversamente dalla neutra, cosa non funziona ancora, quali parametri sono stati cambiati con `set` e perché.

La lezione 7 è la revisione intermedia: chiede anche un'ora di vita registrata da seriale e gli schizzi dell'involucro.

## Punti da decidere

- [ ] **La piattaforma non esiste ancora nel repository.** Il programma la distribuisce oggi e il §12.1 la indica come prossimo passo, nell'ordine nucleo, sensi, espressione, rete, unisono, strumenti. Per questa lezione servono almeno nucleo, sensi, espressione e strumenti, con la neutra; rete e unisono possono arrivare per la lezione 8. Fissare una data di consegna interna, almeno una settimana prima.
- [ ] **Piano B, piattaforma parziale.** Se mancano sensi o espressione: distribuire nucleo e strumenti con la neutra, sensi limitati a touch e IMU, e il disegno della lezione 4 collegato a mano. La lezione resta uguale nella parte frontale; nel pomeriggio si salta D e si allunga C.
- [ ] **Piano B, nessuna piattaforma.** Frontale uguale, sulle figure; nel pomeriggio i gruppi scrivono `config.h` e `creatura.h` sulla carta, come specifica, e continuano su `ProvaEspressione` e sul simulatore. La consegna "specie v0 su due dispositivi" slitta, e la revisione della lezione 7 va ripensata.
- [ ] **Interfaccia della specie**: i nomi delle funzioni e dei campi di `config.h` e `creatura.h` vanno fissati prima della lezione 4, perché il disegno e l'identità delle lezioni 4 e 5 si spostino oggi senza essere riscritti.
- [ ] **Vicini senza rete**: finché la rete non c'è, la creatura non ha mai vicini e la socialità decade sempre alla velocità "senza vicini". Decidere se oggi ridurre quel decadimento, o simulare un vicino da seriale.
- [ ] **Tempi di compilazione**: la piattaforma intera su ESP32-S3 con M5Unified può richiedere minuti alla prima compilazione su un portatile lento. Misurarli prima, su un portatile degli studenti.
- [ ] **Soglie di flotta v0 nella piattaforma**: i valori decisi alla lezione 3 devono essere già nella piattaforma, non nel `config.h` dei gruppi.
- [ ] **Volume di flotta v0** (lezione 5): stesso discorso, nella piattaforma.
- [ ] **Limiti dello stato proprio**: raccogliere oggi, da ogni gruppo, se 10 minuti di durata e 20 di intervallo bastano (§12.1, punto 9).
