# Lezione 9 · Convivenza

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

Le regole di convivenza, il limite di 5 messaggi al secondo, il divieto di risposta automatica, l'osservatore. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con la flotta intera nella stessa stanza. È la **prima prova di flotta**: da oggi il lavoro di un gruppo dipende anche dal rispetto delle regole da parte degli altri.

## Obiettivi

Alla fine della giornata ogni studente sa:

- elencare le regole di convivenza del §9.3 e dire per ciascuna chi la garantisce (la piattaforma o la specie) e come si verifica;
- spiegare con un conto alla lavagna perché dieci creature che si rispondono intasano una stanza in pochi secondi, e perché il limite di 5 messaggi al secondo non basta da solo;
- leggere il registro dell'**osservatore** (§11.4): chi parla, quanto, a chi, e se qualcuno si è riavviato;
- applicare la definizione verificabile di "**senza errori**" alla propria specie.

Ogni gruppo esce con il verdetto di due prove di flotta da 30 minuti e con l'elenco delle correzioni fatte. La classe esce con il primo registro completo della vita sociale della flotta.

## Materiali da preparare

- Dispositivi: i 2 CoreS3 di ogni gruppo, carichi, tutti con **la stessa versione della piattaforma** (stessa versione del protocollo: un dispositivo vecchio viene ignorato, §7.4). Le 2 riserve con la specie neutra: una fa da **pungolo**, collegata al portatile del docente; l'altra sostituisce una specie assente.
- L'**osservatore**: un ESP32 qualsiasi con il firmware di solo ascolto, collegato a un portatile che salva su file tutto ciò che stampa. Va messo al centro della stanza, fermo, per tutta la giornata.
- La tabella di flotta (`flotta.h`) aggiornata con i MAC di tutti i dispositivi: nessun id ripetuto.
- Il **registro dei codici di flotta** della lezione 8, stampato e appeso.
- **Scheda del verdetto** stampata, due per gruppo (modello in fondo a questo piano).
- Lo spazio fisso per le prove di flotta, con un tavolo per gruppo e una zona libera al centro.
- Un orologio visibile da tutti: le note a mano si confrontano con il registro dell'osservatore.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: dieci creature in una stanza | 10' |
| 0:10 | Le regole di convivenza | 25' |
| 0:35 | Perché una stanza si intasa | 25' |
| 1:00 | L'osservatore | 30' |
| 1:30 | Cosa vuol dire "senza errori" | 20' |
| 1:50 | Il protocollo della giornata | 10' |

### 0:00 · Apertura: dieci creature in una stanza (10')

- Fino a ieri ogni specie ha vissuto quasi solo con la sua gemella, e per un quarto d'ora con un gruppo accanto. Oggi avrà una decina di vicini, di specie che non conosce.
- Il principio 5: le specie devono convivere anche se non si conoscono. Nessuno ha letto il codice degli altri, e non serve.
- Domanda alla classe: una cena di dieci persone in cui ognuno risponde a ogni frase di tutti gli altri. Quanto dura prima che nessuno capisca più niente? Cosa fa una tavola vera per non arrivarci?

### 0:10 · Le regole di convivenza (25')

Le regole del §9.3 e del §7.3, una per riga:

| Regola | Perché | Chi la garantisce | Come si verifica oggi |
| --- | --- | --- | --- |
| ignorare i messaggi sconosciuti, senza errori e senza rispondere | i codici liberi degli altri non hanno senso per me | la specie | pungolo con un codice di un'altra specie |
| non rispondere mai in automatico, tranne RISPOSTA a CHIAMATA | evita i cicli | la specie (§12: anche la piattaforma) | pungolo; lettura del codice |
| non più di 5 messaggi al secondo per creatura | tetto alla radio | la piattaforma (§12) | conteggio sul registro dell'osservatore |
| rispettare le intensità | una stanza sopportabile | la piattaforma | a orecchio e a vista, non è un errore di rete |
| restare nell'unisono fino alla fine | la sessione comanda | la piattaforma | lezione 10 |
| accettare un SYNC con scala o tonica diverse | idem | la piattaforma | lezione 10 |
| luce e vibrazione solo come bonus | flotta mista | la specie | non serve se la flotta è tutta CoreS3 |

Si chiude con la regola che conta di più oggi: un messaggio nasce da un **gesto**, da un **tempo** o da uno **stato** della creatura, mai da un messaggio ricevuto. L'unica eccezione è una RISPOSTA, una sola, a una CHIAMATA rivolta a sé.

### 0:35 · Perché una stanza si intasa (25')

**Il carico di base** (§7.5): dieci creature, un annuncio ogni 2 s ciascuna, 5 annunci al secondo. A questo si aggiungono gli invii di iniziativa della lezione 8: si sommano alla lavagna i "messaggi al minuto" dichiarati da ogni gruppo.

**Il saluto che risponde al saluto.** Alla lavagna, dieci creature che rispondono a ogni SALUTO con un SALUTO a tutti:

| Giro | Messaggi |
| --- | --- |
| 1 | A saluta: 1 |
| 2 | le altre 9 rispondono: 9 |
| 3 | ciascuna delle 9 risposte riceve risposta dalle altre 9: 81 |
| 4 | 729 |

Ogni giro dura pochi millisecondi. Il limite di 5 messaggi al secondo ferma la crescita a 10 × 5 = **50 messaggi al secondo**, ma la tempesta non si spegne mai: ogni creatura riceve 45 saluti al secondo, guadagna socialità di continuo e non sente più nessun segnale vero. Il limite protegge la radio, non il comportamento. Solo il divieto di risposta automatica spegne il ciclo.

**Il ping-pong tra due.** Basta che due creature si rispondano: 5 messaggi al secondo ciascuna, per sempre.

**Lo spavento che si propaga.** Il §7.3 dice che chi è vicino a una creatura che manda SPAVENTO può spaventarsi a sua volta, attenuato. Se chi si spaventa rimanda SPAVENTO, la paura gira per la stanza senza fine. La propagazione lecita è un solo salto: chi riceve si spaventa, non rimanda. Gli altri vedranno comunque il suo stato SPAVENTATO nell'annuncio, e lì decide ciascuna specie, senza mandare messaggi.

**La versione indiretta.** "Ricevo UMORE, cambio stato, entrando nello stato mando un messaggio" è una risposta automatica travestita. Lo stesso vale per un messaggio mandato perché un vicino è comparso nell'annuncio in un certo stato.

### 1:00 · L'osservatore (30')

**Cos'è** (§11.4). Un ESP32 in solo ascolto che stampa su seriale ogni pacchetto che sente: annunci, messaggi, SYNC, note. Non manda niente. Collegato a un computer, è il registro della vita sociale della flotta.

**Cosa c'è in ogni riga**: l'ora del computer e i campi del pacchetto (appendice B): tipo, specie, id, numero progressivo; per un annuncio lo stato, i bisogni e i flag; per un messaggio codice, destinatario e valore.

**Come si legge**, con un foglio di calcolo o uno script:

- **filtrare per id**: tutto ciò che ha detto una creatura;
- **contare per secondo** i messaggi di un id: il ritmo;
- **il numero progressivo**: se salta, l'osservatore ha perso pacchetti; se riparte da capo, il dispositivo si è riavviato;
- **i buchi negli annunci**: un id che tace per più di 10 s è sparito dalle tabelle dei vicini di tutti;
- **le coppie stimolo e risposta**: un messaggio di B che segue di poco un messaggio ricevuto da B.

**Dimostrazione dal vivo (10').** Il docente accende tre riserve con la neutra. Dal pungolo manda con `msg` una raffica di SALUTO e la classe la vede sul registro proiettato, con il tetto dei 5 al secondo; poi spegne e riaccende una riserva e si guarda il numero progressivo ripartire.

**I limiti dell'osservatore.** Sente quello che sente da dove sta: un pacchetto perso dall'osservatore non è un pacchetto perso da tutti. Se un buco colpisce tutti gli id nello stesso istante, è dell'osservatore.

### 1:30 · Cosa vuol dire "senza errori" (20')

La consegna di oggi è una specie che convive con tutte le altre per 30 minuti senza errori. "Senza errori" vuol dire che, per **entrambi i dispositivi** della specie, valgono tutte le condizioni seguenti, verificate sul registro dell'osservatore e con il pungolo.

| | Condizione | Come si verifica |
| --- | --- | --- |
| E1 | **Nessun riavvio**: il numero progressivo non riparte mai da capo | registro |
| E2 | **Presenza continua**: nessun silenzio di annunci oltre 10 s, salvo i buchi che colpiscono tutti gli id insieme | registro |
| E3 | **Ritmo**: in nessun intervallo di 1 s più di 5 messaggi dallo stesso id | registro, conteggio per secondo |
| E4 | **Codici**: solo codici 1–7 o codici propri dichiarati nella scheda della lezione 8; CHIAMATA e RISPOSTA mai a 255 e sempre a un id esistente; una RISPOSTA solo dopo una CHIAMATA rivolta a sé, al massimo una per CHIAMATA | registro e scheda dei codici |
| E5 | **Nessuna risposta automatica**: nei 5 s dopo ogni stimolo del pungolo nessun messaggio dal dispositivo provato, tranne una RISPOSTA alla CHIAMATA; un invio periodico dichiarato che cade per caso nella finestra non conta, se si ripete con il suo periodo anche senza stimoli | pungolo e registro |
| E6 | **Stato proprio nei limiti**: ogni permanenza al massimo 10 min (o la durata dichiarata), almeno 20 min tra un'uscita e l'ingresso successivo (§4.4) | stato negli annunci |
| E7 | **Identità**: specie e id giusti, come in `flotta.h`; versione del protocollo uguale a quella della flotta | registro |

Il **pungolo**: dalla riserva collegata al docente, verso l'id provato e uno alla volta, ogni 10 s: SALUTO, UMORE 100, UMORE 0, REGALO 25, SPAVENTO 100, CHIAMATA, CAREZZA 100, un codice proprio di un altro gruppo, un codice libero che nessun gruppo ha dichiarato. Poi il pungolo si mette in SPAVENTATO e in ANNOIATO con `stato`, 30 s ciascuno, per provare la risposta agli annunci. Circa 3 minuti per dispositivo.

Non sono errori: una creatura che muore perché nessuno l'ha curata, una stanza rumorosa, una specie che si spaventa spesso. Si annotano come osservazioni e si discutono, ma non entrano nel verdetto.

### 1:50 · Il protocollo della giornata (10')

- L'ordine del pungolo, gruppo per gruppo.
- Le regole delle prove di flotta: nessuno ricarica un dispositivo durante i 30 minuti; nessuno tocca l'osservatore; si annotano a mano, con l'ora, i momenti notevoli.
- Chi non passa oggi: vedi i Punti da decidere.

---

## Lavoro di gruppo · 5 ore con il pranzo · la prima prova di flotta

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Il pungolo, un gruppo alla volta | 50' |
| 0:50 | B · Prima prova di flotta: 30 minuti | 40' |
| 1:30 | C · Leggere il registro | 30' |
| 2:00 | pranzo | 60' |
| 3:00 | D · Correzioni | 60' |
| 4:00 | E · Seconda prova di flotta e verdetto | 60' |

### A · Il pungolo, un gruppo alla volta (50')

Tutta la flotta è accesa: il pungolo manda messaggi con un destinatario specifico, quindi gli altri dispositivi non ne sono toccati. Ogni gruppo ha circa 10 minuti con il docente: due dispositivi da 3 minuti, più la lettura del registro (E5 ed E4).

Chi aspetta:

- controlla con `info` la versione del protocollo, con `vicini` che la tabella si riempia, con `get *` che i parametri siano quelli della specie;
- confronta la propria scheda dei codici con il registro di flotta;
- rilegge in `creatura.h` ogni punto in cui la specie manda un messaggio e si chiede se può partire da qualcosa ricevuto.

### B · Prima prova di flotta: 30 minuti (40')

| Minuti | Cosa succede |
| --- | --- |
| 0–5 | tutti i dispositivi accesi a fattore tempo 1; il docente segna l'ora d'inizio sul registro |
| 5–35 | la prova |
| 35–40 | il docente chiude il file del registro e lo distribuisce |

Durante la prova le persone vivono con la flotta: ognuno cura creature di **altri** gruppi, le prende in mano, le porta in giro, le raggruppa al centro, ne isola una in un angolo, ne mette due in tasca. Ogni gruppo ha una persona che annota con l'ora cosa succede alle proprie creature.

### C · Leggere il registro (30')

Ogni gruppo filtra il registro sui propri due id e compila la scheda del verdetto, condizione per condizione. Per ogni errore: l'ora, cosa dice il registro, cosa dicono le note a mano, la causa probabile. Il docente controlla le schede ai tavoli.

### D · Correzioni (60')

Si corregge solo quello che ha prodotto un errore, poi le osservazioni più importanti. Ogni correzione si prova prima a due, con la seriale aperta, e con `msg` per ripetere lo stimolo che l'ha causata. Si carica la versione corretta su entrambi i dispositivi e si annota con `info`.

### E · Seconda prova di flotta e verdetto (60')

| Minuti | Cosa succede |
| --- | --- |
| 0–5 | accensione |
| 5–35 | seconda prova, come la prima; il docente ripete il pungolo sulle specie che non avevano passato E5 |
| 35–55 | lettura del registro e seconda scheda del verdetto |
| 55–60 | giro dei verdetti, alla lavagna |

---

## Consegna

Per la lezione 10, in un'unica cartella di gruppo:

1. **Specie che convive con tutte le altre per 30 minuti senza errori**: `config.h` e `creatura.h` della versione che ha passato la prova, con la riga di `info`.
2. **Il registro dell'osservatore** della prova passata e le due schede del verdetto.
3. **Elenco delle correzioni**: cosa è cambiato tra la prima e la seconda prova, e perché.
4. **Scheda dei codici** aggiornata, se è cambiata.

### Modello della scheda del verdetto

Specie: ______ · Prova: 1 / 2 · Inizio: __:__ · Id dei dispositivi: ___ e ___

| Condizione | Dispositivo 1 | Dispositivo 2 | Ora ed evidenza dal registro | Causa probabile |
| --- | --- | --- | --- | --- |
| E1 nessun riavvio | sì / no | sì / no | | |
| E2 presenza continua | | | | |
| E3 ritmo, massimo messaggi in 1 s | | | | |
| E4 codici | | | | |
| E5 nessuna risposta automatica | | | | |
| E6 stato proprio nei limiti | | | | |
| E7 identità e versione | | | | |
| **Verdetto** | | | | |

Osservazioni fuori verdetto: ______

## Punti da decidere

- [ ] **Osservatore e piattaforma**: il repository non contiene ancora il firmware dell'osservatore né la piattaforma con la rete. La lezione non si può fare senza entrambi.
- [ ] **Formato delle righe dell'osservatore**: il §11.4 dice cosa stampa, non come. Va fissato insieme a uno script che controlli E1–E4 ed E6 da solo; a mano, con un foglio di calcolo, una prova da 30 minuti richiede quasi tutto il blocco C.
- [ ] **Cosa fa la piattaforma oltre i 5 messaggi al secondo**: scartare o ritardare, e se lo segnala su seriale. Il §12 dice che il limite è imposto, ma non come. Va anche deciso se annunci, SYNC e note contano nel limite.
- [ ] **Risposta automatica agli annunci**: il §7.3 vieta che un messaggio ricevuto generi un messaggio, ma non parla degli annunci. Questo piano estende il divieto agli annunci (pungolo in SPAVENTATO e ANNOIATO) e legge la propagazione dello SPAVENTO come un solo salto. Va confermato nel documento.
- [ ] **Durata della finestra del pungolo**: 5 s sono una scelta. Una risposta automatica ritardata oltre la finestra si trova solo leggendo il codice.
- [ ] **Chi non passa oggi**: ripete la prova all'inizio della lezione 10, oppure la consegna slitta senza prova di recupero.
- [ ] **Undicesimo dispositivo**: il §11.4 parla di un undicesimo dispositivo, ma la flotta con le riserve è di circa dodici. Basta un ESP32 qualsiasi in più; va comprato.
