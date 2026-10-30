# Lezione 5 · Design system e suono

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

Le dieci regole del design system, la tavolozza comune su uno schermo da 2 pollici, il suono della flotta. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con i dispositivi di tutti i gruppi nella stessa stanza: è la prima lezione in cui le specie si guardano e si ascoltano una accanto all'altra.

## Obiettivi

Alla fine della giornata ogni studente sa:

- spiegare cos'è un **design system** per una flotta: regole comuni che rendono leggibili specie diverse, e il margine di variazione che resta a ciascuna;
- citare le **dieci regole** e dire per ognuna dove vive (piattaforma, `config.h`, `creatura.h`, involucro) e come si verifica;
- scegliere una **palette** di tre colori dalla tavolozza comune e provarla sul dispositivo, accanto alle altre specie, a tutte le luminosità;
- descrivere un **timbro** con famiglia d'onda e inviluppo, e conoscere i limiti dell'altoparlante da 1 W.

Ogni gruppo esce con l'**identità della specie** scritta in `config.h`: codice, palette, timbro. La classe esce con l'assegnazione dei primari e con una proposta di **volume di flotta**.

## Materiali da preparare

- Dispositivi dei gruppi, carichi, con lo sketch di prova della lezione 4 (`ProvaEspressione`) e la forma di ogni gruppo.
- **ProvaSonora** compilata e caricata su almeno tre dispositivi del docente, oppure la Raccolta, che la contiene.
- `ProvaEspressione` con la **palette da seriale**: un comando che sceglie primario, secondario e accento tra i dodici colori della Figura 12. Da aggiungere prima della lezione.
- Figura 12 (la tavolozza) stampata a colori in A3, una per gruppo, e una copia grande per la lavagna. La stampa non è lo schermo: serve solo a nominare i colori.
- **Scheda d'identità** stampata, una per gruppo (modello in fondo a questo piano).
- Un modello di `config.h` con la sola parte di identità, da distribuire. Vedi i Punti da decidere: il formato va allineato alla piattaforma.
- Un fonometro, anche quello di un telefono, per la prova di volume del blocco C.
- La stanza vera: si prova dove si farà la presentazione, o in una stanza simile.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: una flotta, non dieci app | 10' |
| 0:10 | Le dieci regole | 30' |
| 0:40 | La tavolozza su uno schermo da 2 pollici | 25' |
| 1:05 | Il suono della flotta | 35' |
| 1:40 | L'identità in `config.h` | 15' |
| 1:55 | Il pomeriggio | 5' |

### 0:00 · Apertura: una flotta, non dieci app (10')

- Le forme della lezione 4 tutte insieme, sui dispositivi in fila o nei video. Sono diverse, ed è giusto. Domanda alla classe: da cosa si capisce che appartengono allo stesso mondo?
- Un design system serve a due cose (§10): che la flotta si legga come un insieme, e che chi impara una creatura sappia già qualcosa di tutte le altre.
- Il paragone utile è quello di una segnaletica, non di un marchio: chi la progetta non decide ogni cartello, decide le regole con cui ogni cartello si legge.
- Oggi si decide l'identità di ogni specie, e la si decide **accanto alle altre**: un primario scelto da soli non dice niente.

### 0:10 · Le dieci regole (30')

| Regola | In breve | Dove vive | Come si verifica |
| --- | --- | --- | --- |
| 1 Nessun volto | forma astratta, movimento, colore, luce, suono | `creatura.h` | revisione: nessun occhio, bocca, emoji, pittogramma |
| 2 Nessun testo | testo solo nel debug, pressione lunga di 3 s | piattaforma | non esiste una funzione per scrivere in modalità normale |
| 3 Tavolozza comune | 12 colori più 3 neutri; tre per specie; primario unico | `config.h` | tabella dei primari di classe (oggi) |
| 4 Grammatica dei bisogni | nutrimento contrae, energia accelera, umore satura, socialità apre | `creatura.h` | lettura incrociata (lezione 4) |
| 5 Suono comune | pentatonica maggiore, do4, due ottave, una voce, quattro famiglie | piattaforma e `config.h` | il timbro si sceglie solo tra le quattro famiglie |
| 6 Ritmo comune | 90 bpm, griglia a quarti nell'unisono | piattaforma | lezione 10 |
| 7 Tre intensità | urgente al massimo 10 s, poi 60 s; suono al massimo 10% del tempo | piattaforma | registro della seriale |
| 8 Nove gesti | definizione della flotta, significato della specie | piattaforma e `creatura.h` | tabella dei gesti (lezione 4) |
| 9 Il sonno | luminosità 10%, respiro 0,2 Hz, nessun colore saturo, nessuna vibrazione | piattaforma e `creatura.h` | la prova sonno e stato proprio (lezione 4) |
| 10 Involucro | impronta 54 × 54 mm, finestre libere, rigido, niente metallo sull'antenna | involucro | lezione 7 |

- **Regole che impone la piattaforma e regole di progetto.** Alcune non si possono violare perché il codice non lo permette (il testo, il limite dell'urgente). Altre si rispettano per scelta (il volto, la grammatica). Le seconde si verificano con le persone, non con il compilatore.
- **Cosa resta alla specie.** La forma, la palette dentro la tavolozza, il timbro dentro le quattro famiglie, il significato dei gesti, lo stato proprio. Il margine è stretto apposta: la differenza tra specie deve venire dal carattere, non dal rumore.
- **Esercizio lampo (8').** Ogni gruppo cerca nella propria forma della lezione 4 una regola violata o a rischio. Due gruppi la dicono. Di solito esce la regola 1 (due punti che sembrano occhi) o la 9 (un colore che resta saturo nel sonno).

### 0:40 · La tavolozza su uno schermo da 2 pollici (25')

La tavolozza comune della Figura 12, con i valori di partenza:

| corallo | arancio | giallo | lime | verde | acqua | azzurro | blu | viola | magenta | rosa | sabbia |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| FF5A4E | FF9F1C | FFD23F | 9BE564 | 2EC27E | 20C9C0 | 3FA7FF | 3457D5 | 7B4DFF | E040A0 | FF8FB1 | E8C9A0 |

Più tre **neutri comuni** a tutte le specie: il nero del fondo, il grigio del sonno, il colore della grazia, che è il colore dell'unisono e del battito.

- **Primario, secondario, accento.** Il primario è la firma della specie e nessun'altra lo usa (regola 3). Secondario e accento si possono condividere.
- **Lo schermo non è la stampa.** Lo schermo IPS da 2 pollici lavora a 16 bit: 5 bit per il rosso e il blu, 6 per il verde. Due colori vicini sulla carta possono diventare uguali sul dispositivo. I valori della Figura 12 sono di partenza e vanno provati sui dispositivi reali.
- **La grammatica consuma il colore.** L'umore porta la saturazione dal 100% al 10% (Figura 6). A umore basso tutti i primari scivolano verso il grigio: due specie tristi si riconoscono dalla forma, non dal colore. Domanda alla classe: allora a cosa serve il primario?
- **Il sonno consuma il colore.** Al 10% di luminosità e senza colori saturi, la palette quasi sparisce. Anche qui resta la forma.
- **Chi non distingue i colori.** Circa un uomo su dodici ha una forma di daltonismo. Coppie come verde e corallo possono confondersi. Un significato non deve stare solo nella differenza tra due colori della stessa specie.
- **Dimostrazione (5').** Sei dispositivi in fila con `ProvaEspressione`, sei primari diversi. Guardarli a un metro, poi a tre metri, poi con l'umore a 10, poi in DORME.

### 1:05 · Il suono della flotta (35')

**La scala.** Pentatonica maggiore su do4, due ottave: do, re, mi, sol, la. Cinque note senza semitoni: qualsiasi combinazione suona accettabile, ed è per questo che dieci creature possono suonare insieme senza accordarsi.

**Le quattro famiglie d'onda** (§10.1):

| Famiglia | Carattere | Parametri della specie | In ProvaSonora |
| --- | --- | --- | --- |
| seno | morbida, rotonda, si perde nel coro | attacco 5..200 ms, decadimento 50..800 ms, vibrato opzionale | sinusoide |
| triangolo | dolce ma presente, simile a un flauto | come sopra | triangolo |
| quadra | netta, da chip, buca il coro | come sopra, più larghezza d'impulso | quadra |
| soffio | rumore filtrato, percussiva, senza altezza netta | colore del filtro, decadimento | rumore, non filtrato |

**L'inviluppo.** Attacco e decadimento decidono il gesto della nota più del timbro. Un attacco di 5 ms fa clic, uno di 200 ms entra piano. ProvaSonora non ha i parametri in millisecondi: ha quattro inviluppi pronti, secco, morbido, lungo e percussivo. Bastano per ascoltare la differenza.

**I limiti dell'altoparlante da 1 W.**
- Rende poco sotto il do4: per questo la scala parte da lì e il bordone usa la quinta più che la tonica grave.
- Il sonno chiede "una nota grave e sommessa": vuol dire grave dentro la scala, non sotto il do4.
- Il volume alto gracchia, soprattutto con la quadra.

**Il suono fuori dall'unisono.** Una nota alla volta, al massimo il 10% del tempo. Normale: una nota di 300 ms al massimo ogni 10 s. Urgente: fino a tre note consecutive. Sussurro: nessun suono. La vibrazione, sul CoreS3, è un suono sordo di 30 ms. Fuori dall'unisono il suono di una creatura è un segnale raro, non una musica.

**Dimostrazione con ProvaSonora (12').**
1. SWEEP da 100 a 5000 Hz in 8 s: la classe alza la mano quando comincia a sentire il suono. È il pavimento dell'altoparlante.
2. NOTA in loop con i quattro timbri che corrispondono alle famiglie, a volume basso e poi alto.
3. Gli stessi timbri con inviluppo secco e morbido: il clic dell'attacco.
4. Con **Tutti** su tre dispositivi: tre altoparlanti insieme, quanto cresce il volume.

Attenzione: con Tutti gli altri dispositivi **adottano le stesse impostazioni** di chi preme. Serve a sentire il coro, non a confrontare timbri diversi. Per confrontare le specie, nel pomeriggio, ogni dispositivo suona da solo.

**Il ponte con lo stato proprio** (§4.4): lo stato proprio ha un'espressione fatta di forma, colore e suono. Un segno sonoro proprio, raro e riconoscibile, è uno dei modi per distinguerlo dal sonno.

### 1:40 · L'identità in `config.h` (15')

- Il §9.1: l'identità è **codice di specie, nome, palette di tre colori**. Il timbro sta nell'espressione, sempre in `config.h`. È la prima parte del file che ogni gruppo consegnerà alla fine del corso.
- Il codice di specie viaggia in ogni pacchetto radio (un byte nell'intestazione, appendice B). È così che una creatura sa di quale specie è una vicina, anche nel suo stato proprio.
- Un esempio, in forma indicativa finché la piattaforma non fissa i nomi:

```cpp
// identità
#define SPECIE_CODICE      3
#define SPECIE_NOME        "nomade"
#define PALETTE_PRIMARIO   ACQUA
#define PALETTE_SECONDARIO SABBIA
#define PALETTE_ACCENTO    CORALLO
// timbro
#define TIMBRO_ONDA        TRIANGOLO
#define TIMBRO_ATTACCO_MS  20
#define TIMBRO_DECADIMENTO_MS 400
#define TIMBRO_VIBRATO     0
```

- I valori si scrivono in `config.h`, non nel codice di disegno: alla lezione 6 la piattaforma li legge da lì.

### 1:55 · Il pomeriggio (5')

Presentazione dei blocchi e della consegna. La regola del pomeriggio: ogni prova si fa con almeno un'altra specie accanto.

---

## Lavoro di gruppo · 5 ore con il pranzo · l'identità accanto alle altre

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · I primari e la palette | 45' |
| 0:45 | B · Il timbro | 60' |
| 1:45 | pranzo | 60' |
| 2:45 | C · La prova in stanza | 75' |
| 4:00 | D · Correzioni e `config.h` | 35' |
| 4:35 | E · Tabella di classe | 25' |

### A · I primari e la palette (45')

1. **Assegnazione dei primari** (15'), con tutta la classe. Ogni gruppo scrive su un foglietto tre primari in ordine di preferenza, con una riga di motivo legata al carattere. Si leggono insieme. Dove due gruppi vogliono lo stesso primario, decide il motivo più legato al carattere; se la classe non decide, si guarda la seconda scelta. Un primario resta alla specie neutra delle riserve.
2. **Secondario e accento** (15'). Con `ProvaEspressione` e la palette da seriale, provare almeno tre combinazioni sulla propria forma. Ruoli: il primario sul corpo, il secondario dove la forma si estende, l'accento nei momenti rari (urgente, stato proprio). È una proposta, non una regola.
3. **Le tre prove del colore** (15'), sul dispositivo:
   - umore a 10: si riconosce ancora la specie?
   - `stato DORME`: nessun colore saturo? la forma si vede?
   - lo stato proprio, se il gruppo l'ha già disegnato: si distingue dal sonno anche nel colore?

### B · Il timbro (60')

1. Con ProvaSonora, ogni gruppo ascolta le quattro famiglie con i quattro inviluppi, sulla tonica do4 e sul la4 (20'). Una persona suona, le altre scrivono una parola per ogni combinazione.
2. Scegliere famiglia e inviluppo che somigliano al carattere della specie (15'). Domanda guida: *la specie si perde nel coro o lo buca?*
3. Progettare tre **segni sonori**, dentro i limiti del §6.3 (25'):
   - il richiamo a intensità normale: una nota di 300 ms al massimo;
   - il richiamo urgente: fino a tre note consecutive;
   - la nota del sonno: grave, sommessa, al massimo ogni 20 s.
   
   Scriverli come note della scala, durata e inviluppo. Se c'è, aggiungere il segno dello stato proprio.

### C · La prova in stanza (75')

Tutti i gruppi nella stessa stanza, con i due dispositivi accesi.

1. **La fila dei colori** (20'). Tutti i dispositivi in fila sul tavolo, con la propria palette e la forma della lezione 4. Ogni gruppo guarda la fila da 1 e da 3 metri e scrive quali specie confonde. Poi lo stesso con tutti a umore 10 e tutti in DORME.
2. **Il coro dei timbri** (25'). Un dispositivo per gruppo, ognuno con il proprio timbro in loop su NOTA, senza Tutti. Si accende un gruppo alla volta, poi tutti. Chi ascolta, a occhi chiusi, prova a dire quali timbri sente. Il timbro che sparisce nel coro va rivisto; quello che copre gli altri anche.
3. **Il volume di flotta** (20'). Tutti i dispositivi con lo stesso timbro e lo stesso volume, al centro della stanza. Si sale a passi di 16 fino al punto in cui il suono è chiaro a 3 metri e nessun dispositivo gracchia. Si misura con il fonometro. È la proposta di **volume di flotta v0**, come le soglie di flotta della lezione 3.
4. **I segni sonori** (10'). Ogni gruppo fa sentire il richiamo urgente. La classe dice se è riconoscibile e se è sopportabile dieci volte in un'ora.

### D · Correzioni e `config.h` (35')

1. Correggere palette e timbro con quello che è emerso in C (15').
2. Compilare la **scheda d'identità** (10').
3. Scrivere la parte d'identità di `config.h` sul modello distribuito, con un commento di una riga per ogni scelta (10').

### E · Tabella di classe (25')

Alla lavagna, una riga per specie: codice, primario, secondario, accento, famiglia, inviluppo. Con tutta la classe:

- nessun primario ripetuto;
- nessuna coppia di specie che la fila dei colori ha confuso a 3 metri senza che la forma le distingua;
- nessun timbro sparito nel coro;
- il volume di flotta v0.

La tabella di classe è la seconda decisione comune della flotta, dopo le soglie della lezione 3.

---

## Consegna

Per la lezione 6, in un'unica cartella di gruppo:

1. **Identità della specie in `config.h`**: codice, nome, palette (primario, secondario, accento), timbro (famiglia, attacco, decadimento, vibrato o larghezza d'impulso o colore del filtro), con un commento per ogni scelta.
2. **Scheda d'identità** compilata (modello sotto).
3. `ProvaEspressione` aggiornata con la palette scelta, caricata sui due dispositivi.

### Modello della scheda d'identità

| Voce | Scelta | Perché, in una riga |
| --- | --- | --- |
| Codice di specie | | assegnato in classe |
| Nome della specie | | |
| Primario | | |
| Secondario | | |
| Accento | | |
| Famiglia d'onda | | |
| Attacco e decadimento | ms | |
| Vibrato, impulso o filtro | | |
| Richiamo normale | note, durata | |
| Richiamo urgente | note, durata | |
| Nota del sonno | nota, ogni quanti secondi | |
| Segno dello stato proprio, se c'è | | |
| Specie con cui si confonde, e come si distingue | | |

## Punti da decidere

- [ ] **Formato di `config.h`**: la piattaforma non c'è ancora (lezione 6). I nomi dei campi d'identità e di timbro vanno fissati prima di questa lezione, altrimenti il file consegnato oggi va riscritto alla 6. Piano B: consegnare l'identità come scheda e tabella, e scrivere `config.h` alla lezione 6.
- [ ] **Codici di specie**: chi li assegna e con quale regola (per esempio 0 per la neutra, da 1 a 5 per i gruppi). Il campo è di un byte.
- [ ] **Primario della neutra**: quale colore resta alle riserve, e quindi quanti primari restano ai gruppi.
- [ ] **ProvaSonora e §10.1**: ProvaSonora ha nove timbri e quattro inviluppi pronti; il §10.1 ha quattro famiglie con attacco e decadimento in millisecondi, vibrato, larghezza d'impulso e colore del filtro. Il "rumore" di ProvaSonora non è filtrato. Oggi si sceglie a orecchio, ma i parametri esatti si sentono solo con la piattaforma. Valutare se aggiungere a ProvaSonora una pagina con i parametri del §10.1.
- [ ] **Inviluppo e intensità normale**: il §6.3 limita la nota normale a 300 ms, il §10.1 consente un decadimento fino a 800 ms. Decidere se la durata della nota comprende il decadimento.
- [ ] **Volume di flotta**: se il volume è uno solo per la flotta, fissato dalla piattaforma, o un parametro di specie entro un massimo. Dopo la lezione, riportare nel documento il valore v0 deciso dalla classe.
- [ ] **Neutri comuni**: i valori del grigio del sonno e del colore della grazia sono nella Figura 12 ma non nel testo del §10. Riportarli nel §10 e nell'appendice A.
- [ ] **Dove vivono le regole**: la colonna "Dove vive" della tabella delle dieci regole è una proposta di questo piano; il §10 dice solo che alcune le impone la piattaforma. Confermarla quando la piattaforma è scritta.
