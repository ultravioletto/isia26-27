# Lezione 2 · La creatura nel tempo

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 16 ottobre 2026 · Bruno

Macchine a stati finiti, bisogni e variabili nel tempo, gestione delle transizioni. **2 ore frontali + 5 ore di progettazione di gruppo** su carta: i dispositivi arrivano alla lezione 3.

## Obiettivi

Alla fine della giornata ogni studente sa:

- leggere e disegnare una macchina a stati: stati, eventi, condizioni, transizioni, azioni;
- distinguere le **variabili continue** (i bisogni, che scorrono nel tempo) dagli **stati discreti** (la lettura sintetica che se ne fa);
- usare i quattro attrezzi che rendono stabile una transizione: soglia, isteresi, tempo minimo, priorità;
- ragionare per "cosa succede tra un'ora" e non per "cosa succede se premo".

Ogni gruppo esce con la prima idea di carattere della propria specie e con la sua **giornata tipo** disegnata.

## Materiali da preparare

- Slide della parte frontale (scaletta sotto) e Figura 4 del documento di progetto (macchina a stati) stampata in A3, una per gruppo.
- **Kit di simulazione** per gruppo (descritto nel blocco A): scheda della creatura, foglio di vita con 24 righe, mazzo di carte evento, un dado, pennarelli in quattro colori, uno per bisogno.
- Rotolo di carta o fogli A1 per le timeline del pomeriggio.
- Tabella dei parametri di partenza (appendice A) stampata, una per gruppo.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: cosa succede tra un'ora | 15' |
| 0:15 | Macchine a stati finiti | 30' |
| 0:45 | Variabili nel tempo: i bisogni | 25' |
| 1:10 | Gestire le transizioni | 30' |
| 1:40 | Il modello di Creature in rete | 15' |
| 1:55 | Consegne del pomeriggio | 5' |

### 0:00 · Apertura: cosa succede tra un'ora (15')

- La domanda del principio 1: progettare cosa succede **quando nessuno guarda**. Un'interfaccia classica aspetta, una creatura no.
- Riferimenti rapidi: il Tamagotchi (1996), i *motivi* che decadono nei Sims, i fantasmi di Pac-Man che cambiano modo (inseguire, disperdersi, scappare). Sono tutti comportamenti costruiti con stati e variabili nel tempo.
- Domanda alla classe, a voce: "Un oggetto che avete in casa e che cambia anche se non lo toccate?" (frigo, termostato, piante, batterie). Si raccolgono 4 o 5 risposte e ci si torna nel blocco successivo.

### 0:15 · Macchine a stati finiti (30')

- **Gli elementi.** Uno *stato* è una situazione stabile in cui il sistema si comporta in un certo modo. Un *evento* è qualcosa che succede. Una *condizione* è qualcosa che è vero. Una *transizione* porta da uno stato a un altro quando arriva un evento o vale una condizione. Le *azioni* si fanno entrando, restando o uscendo da uno stato.
- **La notazione.** Quella dei diagrammi di stato UML (ISO/IEC 19505-2): rettangoli arrotondati per gli stati, pallino pieno per l'inizio, frecce continue con l'etichetta `evento [guardia] / effetto`, `when(...)` per le soglie e `after(...)` per le durate. Si usa la stessa per tutto il corso. Il cerchio come stato è la convenzione dei manuali di teoria degli automi: la si riconosce, ma non si usa. Approfondimento con i dodici simboli, il modello della creatura ridisegnato e la convenzione del corso: [`segni-macchina-a-stati.html`](segni-macchina-a-stati.html).
- **Tre esempi di difficoltà crescente**, alla lavagna:
  1. *Tornello*: due stati (bloccato, libero) e due eventi (moneta, spinta). È la macchina minima.
  2. *Semaforo*: le transizioni avvengono **per tempo**, non per evento. Il tempo è un ingresso come gli altri.
  3. *Fantasma di Pac-Man*: uno stato temporaneo (spaventato) da cui si **torna allo stato precedente**. Serve memoria dello stato da cui si viene.
- **Esercizio lampo (8').** A coppie: disegnare la macchina a stati di una porta automatica (chiusa, si apre, aperta, si chiude, con persona sulla soglia). Due coppie la mostrano. Di solito qualcuno dimentica cosa succede se una persona arriva mentre la porta si sta chiudendo: è il momento per dire che **ogni stato deve sapere cosa fare con ogni evento**, anche ignorarlo.
- Accenno agli statechart di Harel, da cui UML ha preso stati annidati, regioni parallele e storia (`H*`). Ci servirà per lo spavento e per l'UNISONO, che tornano allo stato precedente.

### 0:45 · Variabili nel tempo: i bisogni (25')

- **Continuo e discreto.** I bisogni sono numeri da 0 a 100 che scorrono. Lo stato è una lettura sintetica di quei numeri: "affamato" non è una variabile, è un'interpretazione del nutrimento sotto 30.
- **Il decadimento.** Velocità al minuto e tempo per raggiungere una soglia. Esempi da fare insieme con i valori di partenza:
  - nutrimento da 100 a 30 a 1,5 al minuto: circa **47 minuti** prima della fame;
  - energia da 100 a 20 a 1,0 al minuto: **80 minuti** prima del sonno, ma solo 27 se gioca (3,0 al minuto);
  - umore da 100 a 50 a 0,5 al minuto: **100 minuti** prima che la noia sia possibile.
  
  Il messaggio è che i ritmi relativi dei bisogni **sono** il carattere: chi ha fame ogni 20 minuti e chi ogni 3 ore sono due creature diverse.
- **Cure ed effetti.** Ogni cura alza un bisogno ma ha un limite: nutrire non più di una volta ogni 2 minuti, la voce vicina al massimo una volta al minuto. Senza limiti una creatura si "ricarica" a forza di tap.
- **La variabile nascosta.** La salute non si vede: riassume quanto a lungo i bisogni sono rimasti bassi. È la memoria della trascuratezza, e governa malattia e morte.
- **Il fattore tempo.** Tutto si moltiplica per 1 nella vita normale, per 5 o 10 in una dimostrazione. Si progetta a velocità 1 e si prova a velocità 10.
- **Disegnare il tempo.** Il grafico di vita: quattro linee nel tempo e, sotto, una striscia colorata con lo stato. È lo strumento di tutto il pomeriggio e della consegna finale (un'ora di vita da seriale).

### 1:10 · Gestire le transizioni (30')

Il cuore della lezione. Una macchina a stati che funziona sulla carta può comportarsi male nel tempo. Quattro attrezzi e quattro difetti tipici.

**Gli attrezzi**

1. **Soglia.** Si entra in uno stato quando una variabile passa un valore. È il caso più semplice e da solo non basta.
2. **Isteresi.** Si entra a un valore e si esce a un altro: fame sotto 30, sazia sopra 50. Esempio del termostato, che con una soglia sola si accenderebbe e spegnerebbe di continuo. Alla lavagna: il nutrimento che oscilla intorno a 30 con e senza isteresi.
3. **Tempo minimo e durata.** Una condizione deve valere per un po' prima di contare (buio *per 5 s* prima di dormire), e certi stati hanno una durata propria (spaventato *per 15 s*, giocoso *finché il gioco continua più 30 s*). Filtra gli incidenti e dà ritmo.
4. **Priorità.** Quando valgono più condizioni insieme vince la più urgente: MORTO, UNISONO, MALATO, SPAVENTATO, PROPRIO, DORME, AFFAMATO, GIOCOSO, ANNOIATO, SVEGLIO. Una creatura affamata e annoiata è affamata.

**Tre tipi di transizione da riconoscere**

- *Verso uno stato e ritorno al perno*: SVEGLIO è il perno, quasi tutto parte da lì e ci torna.
- *Temporanea con ritorno allo stato precedente*: SPAVENTATO, come il fantasma di Pac-Man.
- *Sospensione*: UNISONO congela i bisogni e i timer, poi tutto riparte da dove era. È lo stato annidato degli statechart.

**Le transizioni costano.** Un risveglio forzato toglie 5 di umore, uno spavento 10, tre spaventi in un minuto intaccano la salute. Il costo di una transizione è un modo per dare peso alle azioni delle persone.

**I quattro difetti**

| Difetto | Come si riconosce | Rimedio |
| --- | --- | --- |
| Oscillazione | la striscia degli stati sfarfalla | isteresi, tempo minimo |
| Assillo | la creatura chiede attenzione senza tregua | intensità che sale e poi si acquieta; raffreddamento |
| Stato pozzo | si entra e non si esce più (o solo per caso) | verificare che ogni stato abbia un'uscita raggiungibile |
| Stato irraggiungibile | uno stato progettato che nella vita non capita mai | simulare una giornata e contare quante volte ci si entra |

### 1:40 · Il modello di Creature in rete (15')

- La Figura 4 e la tabella delle transizioni (§4.2–4.3): i nove stati comuni e le loro condizioni di partenza. Si legge insieme una volta, con il dito sullo schema.
- Cosa è **piattaforma** (stati comuni, priorità, struttura delle transizioni) e cosa è **specie** (soglie, tempi, coefficienti di decadimento, quali gesti sono gioco). Le specie non cambiano gli stati comuni: li interpretano.
- **Lo stato proprio** (§4.4): l'unico stato che ogni specie progetta da zero, il momento del suo carattere. Entra e torna da SVEGLIO, dura al massimo 10 minuti, non torna prima di 20, e malattia, spavento e unisono lo interrompono sempre. Mostrare due esempi del §9.4 (l'anziana *assopita* la sera, la timida *sola* senza vicini).
- **Tempo spento** (§4.5): una creatura lasciata in un cassetto torna affamata e sola, mai morta. La morte si guadagna solo con la trascuratezza da accesa, davanti a qualcuno.
- **Memoria e contatori di vita** (§4.6): carezze, pasti, spaventi, minuti in compagnia. Sopravvivono alla morte e permettono comportamenti a lungo termine (l'anziana).

### 1:55 · Consegne del pomeriggio (5')

Presentazione dei blocchi del lavoro di gruppo e della consegna di fine giornata.

---

## Lavoro di gruppo · 5 ore · la vita delle creature

Progettazione "macro": non si progetta ancora cosa fa la creatura quando la tocchi, ma **come scorre la sua vita** nell'arco di ore e di giorni.

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Simulare a mano una mattina | 60' |
| 1:00 | B · Grafici a parete e discussione | 30' |
| 1:30 | pausa | 15' |
| 1:45 | C · Il carattere e la giornata tipo | 90' |
| 3:15 | D · Scambio: un altro gruppo vive la vostra creatura | 60' |
| 4:15 | E · Pin-up e critica | 45' |

### A · Simulare a mano una mattina (60')

Ogni gruppo fa vivere su carta la **specie neutra** dalle 9:00 alle 13:00, in **24 turni da 10 minuti**. Un giocatore fa da creatura (tiene i conti), uno da mondo (pesca gli eventi), gli altri controllano le regole e disegnano.

**Le regole, arrotondate a turni da 10 minuti** (valori di partenza moltiplicati per 10, con due ritocchi spiegati sotto):

| Ogni turno | Sveglia | Nel sonno |
| --- | --- | --- |
| Nutrimento | −15 | −8 |
| Energia | −10 (−30 se gioca) | +50 |
| Umore | −5 (−10 se malata) | −5 |
| Socialità | −10 da sola; +15 con un vicino; +20 in mano | metà |
| Salute | −10 per ogni bisogno sotto 15; +10 se tutti sopra 50 | uguale |

Soglie come nel documento, tranne il sonno: si addormenta sotto 25 (invece di 20) e si sveglia sopra 80.

Si parte con tutti i bisogni a 70 e salute 100. A ogni turno:

1. **La creatura chiede?** Se è affamata, annoiata o malata, o un bisogno è sotto 40, chi cura lancia il dado. L'attenta risponde con 2 o più, la distratta con 3 o più, l'assente solo con 6. Chi risponde cura il bisogno più basso: nutre, mette a dormire, gioca (o accarezza, se l'energia è bassa), prende in mano. Nel sonno la creatura non chiede.
2. **Si pesca una carta**: è il mondo intorno e la cura spontanea. Si applicano gli effetti.
3. Si applica il decadimento, si aggiorna la salute, si controllano le transizioni nell'ordine di priorità, si scrive la riga sul foglio di vita.

> **Perché i ritocchi.** Con i valori del documento moltiplicati per 10 e una sola carta per turno, la creatura muore in un'ora e mezza anche con il mazzo "attenta". In 10 minuti un bisogno passa da 29 a 14 prima che qualcuno possa intervenire, e la salute crolla. Il dado di risposta, le cure da un turno e due ritocchi (benessere sopra 50 invece di 60, sonno sotto 25 invece di 20) rendono la mattina giocabile. Su 300 partite simulate di 24 turni l'attenta perde la creatura l'1% delle volte, la distratta il 33%, l'assente sempre. Lo stesso problema valeva per il dispositivo, e il documento di progetto è stato corretto: la perdita di salute ora è 1,0 al minuto per ogni bisogno sotto 15, e due bisogni trascurati portano la salute da 100 a 0 in circa 50 minuti invece di 25.

**Il mazzo di eventi** (40 carte). Tre mazzi diversi, uno per gruppo o per coppie di gruppi, per confrontare i grafici. Si usa insieme al dado di risposta del mazzo:

| Carta | Effetto | Attenta | Distratta | Assente |
| --- | --- | --- | --- | --- |
| Nessuno | niente | 12 | 22 | 32 |
| Nutrire | due pasti: +50 nutrimento (persa se dorme) | 6 | 3 | 1 |
| Carezza | tre carezze: +15 umore | 6 | 3 | 1 |
| Gioco | +30 umore, −30 energia; se le condizioni valgono, GIOCOSO | 4 | 2 | 0 |
| In mano | +20 socialità | 4 | 2 | 1 |
| Vicino arriva / se ne va | il vicino resta finché non esce la carta opposta | 4 | 4 | 4 |
| Voce vicina | +3 socialità | 2 | 2 | 0 |
| Scossa forte | SPAVENTATO per il turno, −10 umore | 1 | 1 | 1 |
| Buio (coperta) | lancia il dado: con 4–6 dura abbastanza per dormire | 1 | 1 | 0 |

**Il simulatore.** [`simulatore-vita-creatura.html`](simulatore-vita-creatura.html) gioca la stessa partita con gli stessi valori, i tre mazzi e il dado di risposta. Con lo stesso seme, la partita è identica su tutti i portatili. Serve in tre momenti: per mostrare un turno alla classe prima di cominciare; per controllare i conti di un gruppo; nel blocco D, per provare i parametri della propria specie e il suo stato proprio su una giornata intera. Un interruttore toglie l'isteresi per far vedere l'oscillazione.

**Cosa guardare durante il gioco**: quando la creatura entra in ogni stato, quanto ci resta, se oscilla, se qualche stato non compare mai, se si ammala.

### B · Grafici a parete e discussione (30')

Ogni gruppo trasforma il foglio di vita in un grafico: quattro linee colorate e la striscia degli stati sotto. I grafici si appendono vicini, ordinati da "attenta" ad "assente".

Domande per la discussione:

- La differenza tra le tre cure si vede dal grafico? Si vedrebbe guardando la creatura?
- Dove la creatura ha oscillato o è rimasta bloccata? Con quale attrezzo lo correggereste?
- Quale stato non è mai comparso, e perché?
- Quattro ore sono tante o poche per capire una creatura? E un giorno, una settimana?

### C · Il carattere e la giornata tipo (90')

Ogni gruppo comincia a progettare la propria specie a partire dal tempo. I cinque caratteri del documento (§9.4) sono esempi da trasformare, non un menu.

1. **Idea in una frase** (15'). Chi è la creatura e che rapporto vuole con le persone e con le altre.
2. **Ritmi** (20'). Per ogni bisogno: decade più in fretta o più piano della neutra? Quali soglie spostare (fame, sonno, noia, spavento, malattia)? Scrivere i valori nella tabella dei parametri: è la prima bozza di `config.h`.
3. **Lo stato proprio** (15'). Il momento che nessun'altra creatura ha. Un nome, quando scatta (bisogni, ora, vicini, contesto, contatori), quando finisce, cosa fa ai bisogni, come si riconosce senza confonderlo con il sonno, cosa costa a chi la cura. Controllare che stia nei limiti del §4.4.
4. **Giornata tipo** (30'). Su un foglio A1, una timeline di 24 ore: le quattro linee dei bisogni come ve le aspettate e la striscia degli stati, con lo stato proprio in un colore suo. Segnare i momenti chiave: quando dorme, quando chiede, quando sta meglio, quando entra nello stato proprio, cosa succede di notte, cosa succede se la si dimentica nel cassetto per un weekend.
5. **Lungo termine** (10'). Quali contatori di vita usa e cosa cambiano dopo una settimana o dopo una rinascita.

Domande guida al tavolo: *cosa fa quando nessuno la guarda? cosa la fa ammalare? quanto dura un ciclo di cura? la sua fragilità è un piacere o un fastidio per chi la cura?*

### D · Scambio: un altro gruppo vive la vostra creatura (60')

I gruppi si scambiano tabella dei parametri e regole (non la giornata tipo). Ogni gruppo simula la creatura di un altro per 12 turni con il mazzo "distratta" (30'), poi le due squadre confrontano il grafico ottenuto con la giornata tipo attesa (30').

Dove i due grafici divergono, il carattere non è ancora nei numeri: si corregge la tabella.

### E · Pin-up e critica (45')

Ogni gruppo ha 5 minuti più 3 di domande: idea, giornata tipo, la correzione più importante emersa dallo scambio. Si guarda soprattutto se il carattere si legge **nel tempo**, prima ancora che nell'espressione.

---

## Consegna

Per la lezione 3, in un'unica cartella di gruppo:

1. **Concept di specie in una pagina**: idea, carattere, rapporto con le persone e con le altre specie.
2. **Giornata tipo**: foto o scansione della timeline di 24 ore.
3. **Tabella dei parametri**: coefficienti di decadimento e soglie modificati rispetto alla neutra, con una riga di motivazione ciascuno.
4. **Scheda dello stato proprio**: nome, condizioni d'ingresso e d'uscita, effetti sui bisogni, durata, schizzo dell'espressione.
5. **Il grafico della simulazione** del blocco A e quello dello scambio.

## Punti da decidere

- [ ] Le carte evento: stamparle per il blocco A e usare il simulatore solo per verificare, oppure giocare direttamente col simulatore.
- [x] Se lasciare scegliere ai gruppi il proprio carattere già oggi o assegnare per sorteggio un esempio da trasformare. Deciso: i caratteri di base li decide la classe; gli esempi del §9.4 restano solo spunti.
- [x] Lo stato proprio nel simulatore: aggiunto. Il gruppo lo accende, gli dà un nome, sceglie fino a due condizioni d'ingresso, una d'uscita, durata, intervallo e coefficienti di decadimento; il simulatore segnala quando si va oltre i limiti del §4.4. Spento, le partite sono identiche a prima.
- [ ] Lo stato proprio nel gioco su carta: oggi non c'è. Nel blocco D o lo si gioca a mano, con una carta "stato proprio" scritta dal gruppo, o lo si prova solo nel simulatore.
