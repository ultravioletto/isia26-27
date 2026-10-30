# Lezione 10 · Unisono: l'orologio e la sessione

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

Lo stato di grazia, l'orologio condiviso, la sessione e lo strumento. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con la flotta intera: è la lezione in cui le specie, che finora hanno solo convissuto, cominciano a suonare insieme.

## Obiettivi

Alla fine della giornata ogni studente sa:

- dire cosa cambia per una creatura quando entra in **UNISONO**: bisogni congelati, spavento sospeso, stato precedente messo da parte e ritrovato all'uscita, un solo modo di uscire;
- spiegare come dieci dispositivi senza un orologio comune trovano lo stesso **passo**: il SYNC, il t0 stimato, la correzione lisciata, il salto;
- seguire una **sessione** dall'innesco alla fine, compresi i casi in cui qualcosa va storto: l'origine che sparisce, due sessioni che si incontrano, un riavvio;
- leggere l'unisono come **macchina a stati gerarchica**, con le tre regioni parallele dentro la grazia;
- scrivere la parte di unisono della propria specie: ruolo, pattern di 16 passi, ottava, innesco, uscita.

Ogni gruppo esce con la **partitura** della propria parte e con i valori di unisono in `config.h`, provati accanto a quelli di almeno un'altra specie.

## Materiali da preparare

- **TestSincronia** caricato su tre dispositivi del docente, con il clic acceso, e `banco.py` pronto su un portatile con pyserial e le tre porte scritte nel dizionario `PORTE`.
- **ProvaSonora** su almeno tre dispositivi, per far sentire "Tutti" e il registro dell'altoparlante.
- La pagina [`unisono-stati.html`](unisono-stati.html) aperta sul proiettore, e le Figure 9, 10 e 11 del documento di progetto (orologio, sessione, strumento) stampate in A3, una per gruppo.
- **Foglio della partitura**, uno per gruppo più qualche copia: una griglia di 16 colonne (i passi di una battuta) e 10 righe (i gradi su due ottave), con una riga per le probabilità. Modello in fondo a questo piano.
- Un metronomo a 90 bpm con suddivisione in sedicesimi, va bene quello di un telefono collegato a una cassa.
- Cartellini numerati da appendere al collo per il gioco del blocco A: numeri da 3 a 60, non in ordine.
- La piattaforma con `unisono.h`, se è pronta (vedi Punti da decidere), già caricata con la specie di ogni gruppo.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: tre schermi che lampeggiano | 10' |
| 0:10 | Lo stato di grazia | 20' |
| 0:30 | L'orologio condiviso | 25' |
| 0:55 | La sessione | 35' |
| 1:30 | Lo strumento | 20' |
| 1:50 | Consegne del pomeriggio | 10' |

### 0:00 · Apertura: tre schermi che lampeggiano (10')

- Tre CoreS3 sul tavolo con il TestSincronia. Si tocca il primo: apre una sessione. Gli altri due entrano da soli entro 2 secondi. Gli schermi lampeggiano a ogni quarto, giallo sul primo della battuta, e i clic suonano insieme.
- Domanda alla classe: chi sta dirigendo? Si raccolgono le risposte. Poi si spegne il primo dispositivo: dopo qualche secondo un altro prende l'orologio e il lampo non si ferma.
- Il ponte con le lezioni 8 e 9: la rete finora portava **versi**, segnali che chi ascolta interpreta. Oggi porta anche il **tempo**, e il tempo è l'unica cosa su cui tutti devono essere d'accordo.

### 0:10 · Lo stato di grazia (20')

- Il quarto elemento del progetto, la grazia (§1, §8.1): per la durata di una sessione le creature smettono di essere individui e fanno una cosa sola.
- **Cosa succede entrando**, dalla tabella del livello 1 di `unisono-stati.html`:
  - i bisogni si congelano, con i contatori di decadimento e la salute;
  - lo stato di vita corrente si salva con i suoi timer residui;
  - il flag "in unisono" si alza nell'annuncio, con il numero di sessione;
  - l'espressione dei bisogni lascia il posto alla parte musicale.
- **Da dove si entra** (§4.3): da qualsiasi stato tranne MORTO. Anche la creatura che dorme e quella malata, perché con i bisogni congelati la grazia non le fa peggiorare. UNISONO è secondo nell'ordine di priorità, subito dopo MORTO.
- **Lo spavento è sospeso.** In grazia una scossa forte o un colpo sonoro non spaventano: sono gesti dello strumento.
- **Si esce in un modo solo**: alla fine della durata concordata (§8.1). Niente chiusura anticipata, niente uscita individuale, niente scioglimento. All'uscita si torna allo stato salvato, con i timer dove erano: uno SPAVENTATO con 8 s residui riparte da 8 s, uno stato proprio riprende con la sua durata massima già consumata.
- **Nella notazione del corso** ([`segni-macchina-a-stati.html`](segni-macchina-a-stati.html)): l'unisono è uno stato composto a cui si torna con la **storia** `H*`. È lo stesso schema dello spavento della lezione 2, un livello più in alto.
- Cosa resta della creatura in grazia: solo il **carattere**. Timbro, ruolo, pattern, ascolto, docilità, palette. Le differenze sono tutte caratteriali, nessuna è di stato. Per questo l'unisono è il luogo in cui le specie si confrontano alla pari.

### 0:30 · L'orologio condiviso (25')

**Il problema.** Ogni dispositivo conta il tempo da quando si è acceso. I conteggi di due dispositivi non hanno niente in comune, e i quarzi derivano di pochi millisecondi al minuto.

**La soluzione** (§8.2, Figura 9), in quattro frasi alla lavagna:

1. L'**origine** manda ogni 2 s un SYNC: "sono passati tot ms dall'inizio della sessione", con bpm, griglia, scala, tonica e durata.
2. Chi lo riceve calcola l'inizio nel **proprio** orologio: t0 = adesso − trascorso − qualche ms di latenza radio.
3. Da lì calcola da solo il passo in cui si trova. I pacchetti servono solo a concordare l'inizio e a correggere la deriva, non a battere il tempo.
4. I SYNC successivi spostano t0 di **al massimo 10 ms al secondo**; uno scarto **oltre 150 ms** si corregge di colpo. Un passo può saltare, nessun passo viene ripetuto.

**I numeri di partenza** (Appendice A e regola 6 del design system):

| Grandezza | Valore |
| --- | --- |
| tempo | 90 bpm |
| griglia | quarti divisi in 4 passi |
| un quarto | 666,7 ms |
| un passo | 166,7 ms |
| una battuta, 16 passi | 2,67 s |
| una sessione di 180 s | 67 battute e mezza |

**Esercizio lampo (8').** A coppie, con la calcolatrice. Un SYNC dice "trascorso 12 000 ms" e arriva quando il mio orologio segna 50 000 ms; la latenza è 3 ms.
- Quanto vale t0 nel mio orologio? (37 997 ms)
- In che passo sono quando il mio orologio segna 52 000 ms? (passo 84, cioè la battuta 6, quinto passo)
- Il SYNC successivo mi dà un t0 di 38 004 ms: in quanto tempo lo raggiungo? (7 ms di scarto, meno di un secondo di correzione)

**Le misure vere.** Nel TestSincronia su tre CoreS3 gli schermi lampeggiano entro 0,3 ms l'uno dall'altro; lo scarto mediano misurato è 0,03 ms. `banco.py` pilota i tre dispositivi via seriale, li isola, li mette in pausa, li riavvia, e misura. Si proietta il riassunto di una corsa di `banco.py`, se è stato salvato un `banco.log` prima della lezione.

### 0:55 · La sessione (35')

Il protocollo di §8.3 e la Figura 10, poi la stessa cosa come macchina a stati, livello 2 di `unisono-stati.html`.

| Fase | Cosa succede | Numeri |
| --- | --- | --- |
| innesco | la specie propone: con un gesto, per una condizione interna, o mai | la neutra: tre scosse in 2 s |
| apertura | chi propone diventa origine, sceglie un numero di sessione e manda il primo SYNC, che vale da invito | niente sessione parallela se ha sentito un SYNC negli ultimi 3 s |
| ingresso | chi accetta alza il flag e inoltra i SYNC con hop + 1 | ritardo 50–300 ms, hop massimo 2 |
| prontezza e quorum | quanto aspettare e quanti partecipanti servono prima di entrare | 0–10 s; 0–4 |
| continuità | chi non sente SYNC diventa orfano e continua sul proprio orologio | orfano a 3 s; elezione a 6 s, vince l'id più basso |
| conflitti | due sessioni o due origini: vince l'origine con l'id più basso | ognuno decide da solo, tutti allo stesso modo |
| recupero | chi si riavvia ascolta: se la sessione c'è, rientra subito | finestra di 6 s |
| fine | solo alla durata; ognuno la calcola dal SYNC | 180 s, mai 0; non si entra se ne mancano meno di 10 |
| uscita | la voce sfuma fino alla fine della battuta | al massimo 2 s |
| riposo | non si propone e non si accetta una sessione nuova | 30 s |

- **Il principio dietro tutte le regole.** Nessuna transizione dipende da un singolo pacchetto (§7.5). Un SYNC perso costa qualche decimo di millisecondo di deriva; una nota persa è una nota persa, come in un coro.
- **Una regola sola per i conflitti**: vince l'id più basso. Nessuno vota, nessuno negozia: ogni dispositivo guarda la propria tabella dei vicini e arriva alla stessa conclusione degli altri.
- **Le tre regioni dentro la grazia**: chi tiene l'orologio (origine, partecipante, orfano), la voce (autonoma, in mano, ritorno), l'aggancio del tempo (agganciato, in correzione, salto). Si leggono insieme sulla pagina, una regione alla volta. La regione della voce è il tema della lezione 11.
- **Attenzione alla pagina.** `unisono-stati.html` è più vecchia della versione attuale di §8. Contiene uscite anticipate che il documento di progetto ha tolto: il gesto di chiusura dell'origine, l'uscita individuale, lo scioglimento per solitudine dopo 10 s, l'uscita per silenzio dopo 15 s. Oggi vale §8: una creatura isolata a lungo diventa origine di sé stessa, continua da sola e si riallinea quando torna in portata. Lo si dice prima di proiettare la tabella delle transizioni, e si saltano le righe G6, G9, U3, U4, U5 e U10.
- Dimostrazione con il TestSincronia, dalle prove scritte in testa allo sketch: ingresso (prova 1), continuità con l'origine spenta (prova 3), due origini avviate insieme (prova 6), durata accorciata a 40 s con il comando `durata 40` (prova 7, tutti escono allo stesso passo).

### 1:30 · Lo strumento (20')

- **La scala comune** (§8.2, regola 5): pentatonica maggiore su do4, due ottave. Nel pacchetto NOTA il grado va da 0 a 4 e l'ottava da 0 a 1: dieci note in tutto, nessuna stona con le altre.
- **Il limite dell'altoparlante** (§10.1): 1 W, rende poco sotto il do4. Per questo la scala parte da lì e il bordone preferisce la quinta alla tonica grave. Con ProvaSonora si fa sentire lo SWEEP e poi la scala: la classe dice dove il suono sparisce.
- **Dieci altoparlanti sono un piccolo coro.** Con ProvaSonora in "Tutti", l'ACCORDO su tre dispositivi: ognuno una nota. La leggera imprecisione tra i dispositivi è parte del suono.
- **I quattro ruoli, in breve**, quanto basta per scegliere nel pomeriggio: bordone (il pavimento), ritmo (il battito), melodia (il cammino), eco (la risposta). Il comportamento di ciascuno, con docilità e ascolto, è il tema della lezione 11.
- **Il pattern di 16 passi** (§8.5): una battuta di quattro quarti, con una probabilità per passo. Un passo a 1 suona sempre, uno a 0 mai, uno a 0,5 una volta su due. Il pattern sta in config, senza scrivere logica.
- **Suonare la creatura in mano.** Il programma promette beccheggio, rollio e tap quantizzato, ma §8.4 dice che quella versione è **sospesa** finché la modalità non viene decisa (decisione aperta 7). Si presenta come proposta: l'inclinazione sceglie grado e ottava, il tap suona sempre sul passo più vicino, subito se il passo è passato da meno di 40 ms, altrimenti al passo successivo, al massimo 83 ms dopo, cioè mezzo passo. Si chiede alla classe cosa la convince e cosa no: la decisione serve prima della lezione 11.

### 1:50 · Consegne del pomeriggio (10')

- I blocchi del pomeriggio e le coppie di gruppi per la prova del blocco E, già scritte alla lavagna.
- Cosa decide la specie (§9.1, riga Unisono): ruolo, pattern, ottava preferita, docilità, ascolto, prontezza, quorum, innesco, se entra quando dorme, come esce. Oggi si scrivono ruolo, pattern, ottava, innesco e uscita; docilità, ascolto, prontezza e quorum si mettono ai valori della neutra e si progettano alla lezione 11.

---

## Lavoro di gruppo · 5 ore con il pranzo · la parte di unisono

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · La sessione giocata con i corpi | 40' |
| 0:40 | B · L'orologio sulla flotta intera | 50' |
| 1:30 | pranzo | 60' |
| 2:30 | C · Ruolo, pattern e ottava | 60' |
| 3:30 | D · Innesco e uscita | 40' |
| 4:10 | E · Prova a coppie di specie | 50' |

### A · La sessione giocata con i corpi (40')

Tutta la classe, in piedi, nella stanza delle prove. Ogni studente è un dispositivo e porta al collo un cartellino con il suo id. Il metronomo è spento: il tempo lo tiene l'origine battendo le mani, e "manda un SYNC" alzando il braccio e dicendo ad alta voce il numero della battuta.

Si gioca **quattro scenari**, 8 minuti ciascuno, poi 8 minuti di discussione:

1. **Ingresso.** Uno studente propone; gli altri entrano alzando la mano e battendo con lui. Chi è dall'altra parte della stanza e non vede l'origine entra solo quando vede un vicino che ha già la mano alzata: è l'hop.
2. **L'origine se ne va.** A metà sessione l'origine esce dalla stanza. Tutti continuano a battere; dopo un tempo concordato chi ha l'id più basso alza il braccio e diventa origine. Si guarda se il battito si interrompe.
3. **Due sessioni.** Due studenti agli angoli opposti propongono nello stesso momento. Quando i due gruppi si incontrano, vince l'id più basso: chi seguiva l'altro cambia battito. Si conta quante persone fanno il salto.
4. **La fine.** Si concorda la durata in battute prima di cominciare. Nessuno la annuncia: ognuno conta da solo e si ferma alla battuta stabilita. Si guarda chi sbaglia e di quanto.

Domande per la discussione: in quale scenario la stanza è stata più fragile? Cosa ha reso possibile non fermarsi? Cosa cambierebbe se ognuno potesse uscire quando vuole?

### B · L'orologio sulla flotta intera (50')

Il TestSincronia su tutti i dispositivi della classe, circa dieci, più l'osservatore acceso dalla lezione 9.

1. **Caricare** il TestSincronia, o la Raccolta che lo contiene, su entrambi i dispositivi del gruppo (10').
2. **Prove di flotta** (30'), dalle sette scritte in testa allo sketch, scegliendo quelle che con dieci dispositivi dicono qualcosa di nuovo:
   - ingresso: un dispositivo avvia, si conta in quanti secondi entrano tutti;
   - hop: due o tre dispositivi portati nel corridoio, finché non sentono più l'origine; a schermo si legge `hop`;
   - continuità: si spegne l'origine e si guarda chi prende l'orologio, e se è davvero l'id più basso;
   - due origini: due gruppi avviano insieme dagli angoli opposti;
   - durata: tutti escono allo stesso passo, anche un dispositivo chiuso in un cassetto.
3. **Ripresa al rallentatore** (10') con un telefono: i dieci schermi nello stesso fotogramma. Lo scarto si legge anche a schermo, come `err`.

Ogni gruppo annota in una riga per prova cosa ha visto e se corrisponde a §8.3. Chi trova un comportamento diverso dal documento lo segnala al docente con l'ora e gli id coinvolti: è materiale per la piattaforma.

### C · Ruolo, pattern e ottava (60')

Il cuore del pomeriggio. Ogni gruppo lavora sul foglio della partitura.

1. **Il ruolo** (10'). Bordone, ritmo, melodia, eco. Quale corrisponde al carattere della specie? La timida del §9.4 fa l'eco, l'anziana il bordone: sono esempi, non regole. Due gruppi possono avere lo stesso ruolo, ma la classe controlla alla fine che la flotta non sia tutta melodia.
2. **Il pattern** (25'). Sedici caselle, una probabilità per casella. Si parte da un pattern a soli 0 e 1, lo si batte sul tavolo con il metronomo a 90 bpm in sedicesimi, poi si introducono le probabilità intermedie. Domande al tavolo: il pattern si riconosce anche quando suona a metà? Riempie la battuta o lascia spazio agli altri? Il ruolo di ritmo vuole i passi in levare: quali sono, nella griglia?
3. **L'ottava** (10'). L'ottava preferita, bassa (do4–la4) o alta (do5–la5). Il bordone sta in basso ma non sotto il do4. Con ProvaSonora si ascolta il proprio timbro nelle due ottave: in quale la specie si sente meglio accanto alle altre?
4. **Densità** (15'). Si somma il numero medio di note per battuta: è la somma delle probabilità del pattern. Con dieci creature, quante note al secondo fa la flotta? §7.5 parla di "poche decine": se la somma della classe va oltre, qualcuno deve fare spazio. Il conto si fa alla lavagna con i valori di tutti i gruppi.

Se la piattaforma è pronta, il pattern si prova sul dispositivo con `set` e `unisono avvia`, senza ricompilare. Se non lo è, si prova a voce e a mani sul metronomo, e si scrive comunque nella forma di `config.h`.

### D · Innesco e uscita (40')

1. **L'innesco** (20'). Come la propria creatura propone un unisono, una delle tre strade di §8.3:
   - un **gesto umano**, scelto tra i nove; la neutra usa tre scosse in due secondi;
   - una **condizione interna**, con bisogni, vicini, contesto, ora del giorno; la neutra suggerisce umore alto e almeno tre vicini;
   - **mai**: la specie si limita ad accettare.
   Il gesto d'innesco non deve rubare un significato già dato nella tabella della lezione 4. Una condizione interna troppo larga apre una sessione ogni 3 minuti e mezzo, cioè ogni volta che finisce il riposo: si controlla con quale frequenza scatterebbe in una giornata tipo.
2. **Entra quando dorme?** (5'). §9.1 lo mette tra le scelte della specie, §4.3 e §6.4 dicono che anche la creatura che dorme entra. Per oggi si scrive la preferenza del gruppo con il motivo; la regola si fissa dopo la lezione (vedi Punti da decidere).
3. **L'uscita** (15'). La fine non si sceglie: arriva alla durata. Si sceglie **come** si esce, con la durata della dissolvenza, da subito fino a 2 s. Tutti insieme a fine battuta, oppure una alla volta con una durata diversa per individuo. Il gruppo descrive in una frase cosa sente il pubblico negli ultimi due secondi.

### E · Prova a coppie di specie (50')

Le coppie di gruppi sono fissate alla lavagna; con un numero dispari di gruppi una coppia diventa un terzetto.

- **Con la piattaforma** (35'): i quattro dispositivi in un angolo della stanza, una sessione di 180 s aperta dall'innesco di una specie, poi una seconda dall'innesco dell'altra. Si registra l'audio con un telefono, sempre dalla stessa distanza, circa 1 m.
- **Senza la piattaforma**: le due partiture suonate insieme, a mani o a voce, sul metronomo, per quattro battute ripetute; poi si cambia chi tiene il tempo.
- **Discussione** (15'): le due parti si distinguono? Si coprono? Una delle due ha bisogno dell'altra? Cosa cambierebbe con dieci creature invece di quattro?

Ogni gruppo corregge la propria parte e la riscrive in `config.h`.

---

## Consegna

Per la lezione 11, in un'unica cartella di gruppo:

1. **Parte di unisono in `config.h`**: ruolo, pattern di 16 passi con le probabilità, ottava preferita, innesco, preferenza sull'ingresso nel sonno, durata della dissolvenza d'uscita. Docilità, ascolto, prontezza e quorum ai valori della neutra.
2. **Partitura**: il foglio del blocco C, pulito, con una riga di motivazione per il ruolo e una per l'innesco.
3. **Prova con almeno un'altra specie**: la registrazione audio del blocco E, oppure, senza piattaforma, la partitura sovrapposta a quella dell'altro gruppo, con le correzioni fatte.
4. Le **note delle prove di flotta** del blocco B, una riga per prova.

### Modello del foglio della partitura

| Passo | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| quarto | 1 | | | | 2 | | | | 3 | | | | 4 | | | |
| probabilità | | | | | | | | | | | | | | | | |
| grado (se fisso) | | | | | | | | | | | | | | | | |

Sotto la griglia: specie, ruolo, ottava, timbro (dalla lezione 5), innesco, dissolvenza d'uscita, note medie per battuta.

## Punti da decidere

- [ ] **Piattaforma con l'unisono.** Nel repository non c'è ancora il codice della piattaforma: niente `unisono.h`, niente `config.h` della neutra. Il programma prevede rete e unisono pronti prima della lezione 8. Se a questa lezione non ci sono, i blocchi C ed E si fanno su carta e a mani, e la consegna "provata con almeno un'altra specie" va intesa come prova di partiture.
- [ ] **Nessuno sketch suona un pattern sull'orologio condiviso.** Il TestSincronia fa solo un clic sul quarto; ProvaSonora in "Tutti" fa suonare insieme più dispositivi, ma non sull'orologio dell'unisono. Valutare se aggiungere al TestSincronia un pattern di 16 passi letto da seriale, per provare le partiture prima della piattaforma.
- [ ] **Come si suona la creatura in mano** (§8.4, decisione 7): adottare la proposta di inclinazione e tap quantizzato, o un'altra. Va deciso prima della lezione 11, che prevede l'unisono suonato da persone esterne.
- [ ] **Ingresso nel sonno**: §4.3 e §6.4 dicono che anche la creatura che dorme entra; §9.1 elenca "se entra quando dorme" tra le scelte della specie; `unisono-stati.html` prevede uno stato SPETTATORE. Scegliere una regola e allineare i tre testi.
- [ ] **Aggiornare `unisono-stati.html`** a §8 attuale: togliere le uscite anticipate (G6, G9, U3, U4, U5, U10), i motivi 2 e 3 del FINE, che l'Appendice B non prevede più, e la nota sull'incoerenza tra MALATO e UNISONO, già risolta in §4.3. Aggiornare anche la sezione "Stato nel TestSincronia", che non riporta durata e riposo, oggi presenti nello sketch.
- [ ] **Innesco comune** (decisione 4): lasciarlo tutto alle specie o fissare un gesto comune per il pubblico della lezione 12. Conviene decidere prima della lezione 11.
- [ ] **Scala e tonica** (decisione 5): fisse per tutto il corso o con sessioni in tonalità diverse.
- [ ] **Densità della flotta**: fissare un tetto di note medie per battuta per creatura, se il conto del blocco C mostra che la stanza si riempie.
