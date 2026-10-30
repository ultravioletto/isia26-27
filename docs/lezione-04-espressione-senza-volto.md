# Lezione 4 · Espressione senza volto

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

I cinque canali, la grammatica dei bisogni, le tre intensità e il sonno, la grafica generativa con M5GFX. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, sul dispositivo vero. La piattaforma arriva alla lezione 6: oggi si lavora con uno **sketch di prova** in cui i bisogni si comandano a mano.

## Obiettivi

Alla fine della giornata ogni studente sa:

- nominare i **cinque canali** dell'espressione e i loro vincoli sul CoreS3;
- applicare la **grammatica dei bisogni**: nutrimento contrae, energia accelera, umore satura, socialità apre, con i campi della Figura 6;
- dosare un segnale nelle **tre intensità** e riconoscere la **convenzione del sonno**, l'unica obbligatoria per tutte le specie;
- disegnare una forma animata con le primitive di **M5GFX**, a 30 fotogrammi al secondo, su una tela fuori schermo.

Ogni gruppo esce con la **tabella dei nove gesti** della propria specie e con la **forma di base** che vive sul dispositivo e si deforma quando si cambiano a mano i quattro bisogni.

## Materiali da preparare

- Dispositivi dei gruppi, carichi. I portatili con l'ambiente della lezione 1.
- **Sketch di prova `ProvaEspressione`**, da scrivere prima della lezione: non esiste ancora nel repository. Parte da ProvaBlob (tela 320 × 240 fuori schermo, corpo disegnato a ventaglio di triangoli, tempo misurato a ogni fotogramma) e aggiunge: i quattro bisogni da 0 a 100; i quattro valori della Figura 6 già calcolati; i comandi da seriale `bisogno nome valore` e `stato DORME`, con la stessa forma del §11.2; un giro automatico che fa scorrere i bisogni; una funzione `disegnaForma()` che ogni gruppo riscrive. Va provato su due dispositivi.
- Raccolta caricata su un dispositivo del docente, per mostrare Blob, Sciame, Cimatica e Interference.
- Figura 6 del documento di progetto (le quattro direzioni) e tabelle del §6.1 e del §6.3 stampate in A3, una per gruppo.
- Le **soglie di flotta v0** decise alla lezione 3, stampate su un foglio per gruppo.
- Fogli A3 a quadretti con un rettangolo 320 × 240 in scala 1:1 (circa 41 × 30 mm per uno schermo da 2 pollici: conviene stamparne anche uno ingrandito 4:1).
- Un panno scuro per guardare il sonno a luminosità bassa.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: perché senza volto | 10' |
| 0:10 | I cinque canali | 20' |
| 0:30 | La grammatica dei bisogni | 30' |
| 1:00 | Le tre intensità e il sonno | 20' |
| 1:20 | Grafica generativa con M5GFX | 30' |
| 1:50 | Lo sketch di prova e il pomeriggio | 10' |

### 0:00 · Apertura: perché senza volto (10')

- Il principio 2 e le regole 1 e 2 del design system: niente occhi, bocche, emoji, pittogrammi, testo. Il testo esiste solo nello schermo di debug.
- Il motivo pratico: un volto si disegna una volta per emozione e non si combina. Una grammatica invece si somma: una creatura affamata ma felice è **contratta e satura** (§6.2).
- Il motivo di flotta: dieci creature di cinque specie diverse si leggono come un insieme solo se dicono le stesse cose nello stesso modo.
- Dimostrazione (3'): Blob e Sciame dalla Raccolta, proiettati. Nessuno dei due ha un volto, eppure la classe dice "è agitato", "si è calmato". Domanda: da cosa lo avete capito? Velocità, ampiezza, compattezza: sono già le variabili della grammatica.

### 0:10 · I cinque canali (20')

| Canale | Cosa offre (§6.1) | Vincoli | Sul CoreS3 |
| --- | --- | --- | --- |
| Schermo | 320 × 240, primitive: cerchi, poligoni, linee, curve, campi di rumore, maschere | niente testo, volti, immagini caricate; 30 fotogrammi al secondo | IPS da 2 pollici |
| Colore | primario, secondario, accento della specie più i neutri comuni | solo la tavolozza comune (§10, lezione 5) | 16 bit per pixel |
| Suono | una voce: nota della scala, timbro, inviluppo, volume | una nota alla volta; fuori dall'unisono non più del 10% del tempo | altoparlante da 1 W (lezione 5) |
| Vibrazione | impulsi da 30 a 300 ms | solo alle intensità normale e urgente | **non c'è il motore**: diventa un suono sordo di 30 ms |
| Luminosità | retroilluminazione dal 5% al 100% | nel sonno non oltre il 10% | — |

- Il canale principale è lo schermo. Gli altri accompagnano. La regola di convivenza del §9.3 vale anche qui: la vibrazione è un bonus, mai l'unico canale di un significato.
- Oggi si lavora su schermo, colore provvisorio e luminosità. Palette e timbro si decidono alla lezione 5.

### 0:30 · La grammatica dei bisogni (30')

Ogni bisogno ha una direzione espressiva fissa per tutta la flotta. **La forma è della specie, la deformazione è della grammatica.** I valori sono quelli della Figura 6:

| Bisogno | Direzione | Variabile della forma | Da basso ad alto | Si legge come |
| --- | --- | --- | --- | --- |
| Nutrimento | contrazione | scala della forma | da 0,4 a 1,0 | la fame stringe |
| Energia | velocità | velocità del movimento | da 0,25× a 2× | la stanchezza rallenta |
| Umore | saturazione | saturazione del colore | dal 10% al 100% | la tristezza sbiadisce |
| Socialità | apertura | estensione verso i bordi | dal 30% al 100% | la solitudine chiude |

- **Indipendenti e sommabili.** Le quattro variabili non si toccano tra loro. Esempio alla lavagna: nutrimento 20, energia 80, umore 90, socialità 40. Con una lettura lineare: scala circa 0,5, velocità circa 1,6×, saturazione circa 91%, estensione circa 58%. Disegnarlo con un cerchio e poi con una forma a stella.
- **La trappola della contrazione e dell'apertura.** Scala ed estensione sembrano la stessa cosa. Vanno separate: la scala cambia la dimensione del corpo, l'estensione quanto il corpo, o qualcosa del corpo, si spinge verso i bordi dello schermo (appendici, particelle, onde, raggi). Una forma che ha solo un raggio non sa distinguere fame e solitudine.
- **La trappola della velocità.** Velocità vuol dire ritmo del movimento, non posizione. Una forma che respira, gira o pulsa ha una velocità; una forma ferma no.
- **Lo stato non è un quinto canale.** Gli stati comuni (§4.2) si leggono attraverso la grammatica: AFFAMATO è contratto, MALATO perde saturazione e velocità. La specie può aggiungere segni propri a uno stato, ma non deve contraddire la grammatica.
- **Esercizio lampo (8').** A coppie, su carta: una forma di base qualsiasi in quattro versioni, una per bisogno al minimo. Poi la si fa indovinare alla coppia accanto, senza dire quale bisogno è basso. Di solito fame e solitudine si confondono: è la trappola di prima.

### 1:00 · Le tre intensità e il sonno (20')

| Intensità | Schermo | Suono | Vibrazione | Quando |
| --- | --- | --- | --- | --- |
| sussurro | variazioni entro il 20% dell'ampiezza, ritmo fino a 0,5 Hz | nessuno | nessuna | bisogni sopra 50, sonno, in tasca |
| normale | ampiezza piena, ritmo fino a 2 Hz | una nota di 300 ms al massimo ogni 10 s | un impulso di 100 ms al massimo ogni 10 s | un bisogno tra 30 e 50, interazione in corso |
| urgente | ampiezza piena, ritmo fino a 4 Hz, luminosità massima | fino a tre note consecutive | impulsi fino a 300 ms | un bisogno sotto 30, spavento |

- **Nessuna creatura urla sempre** (§6.3): l'urgente dura al massimo 10 s, poi 60 s di raffreddamento a normale, anche se il bisogno resta. È la regola dell'assillo vista alla lezione 2, applicata all'espressione.
- L'intensità e la grammatica sono due cose diverse: la grammatica dice **cosa** cambia, l'intensità **quanto** si fa notare. Una creatura affamata a sussurro è contratta e quieta; a urgente è contratta e pulsa a 4 Hz.
- **Il sonno** (§6.4, regola 9): luminosità al 10% o meno, nessun colore saturo, un respiro della forma di base a 0,2 Hz (un ciclo ogni 5 s), nessuna vibrazione, al massimo una nota grave e sommessa ogni 20 s. È uguale per tutte le specie perché chiunque deve riconoscere una creatura che dorme senza conoscerla.
- **Il ponte con lo stato proprio** (§4.4): la piattaforma garantisce che lo stato proprio **non imiti il sonno**. Uno stato proprio quieto, come l'*assopita* dell'anziana, deve distinguersi da DORME: con il colore, con un ritmo diverso da 0,2 Hz, con un segno della forma. Lo si verifica oggi nel blocco D.

### 1:20 · Grafica generativa con M5GFX (30')

Le tecniche che si trovano già negli sketch del repository, aperti e proiettati.

- **La tela fuori schermo.** Tutte le prove disegnano su una `M5Canvas` di 320 × 240 e la spingono sullo schermo una volta per fotogramma con `pushSprite`. Così non si vede lo sfarfallio. A 16 bit la tela occupa 320 × 240 × 2 = 150 kB.
- **Il tempo, non i fotogrammi.** Ogni movimento si calcola con il tempo trascorso (`dt`), come in ProvaBlob. Se un fotogramma tarda, la forma non rallenta: la velocità della grammatica resta vera. Mai `delay()`: alla lezione 6 il ciclo della piattaforma non lo permette (§3.3).
- **Il colore a 16 bit.** Gli sketch usano una funzione `rgb()` che porta tre valori da 0 a 255 nel formato a 16 bit dello schermo. Per la saturazione si mescola il colore della specie con il grigio, come fa la funzione `mescola()` di ProvaBlob.
- **Le primitive.** Linee, rettangoli, cerchi, triangoli pieni. Da queste nascono quattro famiglie di forme, una per prova:
  - **contorno di punti**: ProvaBlob, un anello di punti con molle, riempito a ventaglio di triangoli dal centro. Scala, respiro e apertura sono parametri del contorno;
  - **particelle**: ProvaSciame, 180 punti con le regole dei boids. L'estensione è il raggio dell'orbita, la velocità è la velocità di giro;
  - **campo**: ProvaCimatica e InterferencePatterns calcolano un valore per pixel. Belle, ma costose: Cimatica calcola la forma una volta all'avvio e a ogni fotogramma moltiplica solo per un numero;
  - **linee**: ProvaScatola, wireframe senza superfici.
- **Il bilancio dei 30 fotogrammi.** 33 ms a fotogramma per calcolare e disegnare. ProvaBlob mostra gli fps toccando lo schermo: è il primo numero da guardare quando una forma diventa troppo ricca.

### 1:50 · Lo sketch di prova e il pomeriggio (10')

- Cosa fa `ProvaEspressione` e cosa si riscrive: solo `disegnaForma()` e i colori provvisori. I bisogni arrivano da seriale o dal giro automatico.
- Perché i comandi hanno la forma del §11.2: alla lezione 6 la piattaforma li accetta uguali, e il disegno di oggi si sposta in `creatura.h`.
- I blocchi del pomeriggio e la consegna.

---

## Lavoro di gruppo · 5 ore con il pranzo · dare corpo al carattere

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · I nove gesti della specie | 45' |
| 0:45 | B · La forma di base su carta | 45' |
| 1:30 | pranzo | 60' |
| 2:30 | C · La forma sul dispositivo | 75' |
| 3:45 | D · Le quattro direzioni, il sonno e lo stato proprio | 45' |
| 4:30 | E · Lettura incrociata | 30' |

### A · I nove gesti della specie (45')

Dai tre gesti della lezione 3 alla tabella completa. Si parte dalle **soglie di flotta v0**: la definizione dei gesti è quella e non si cambia; si decide solo il significato.

1. Copiare la colonna della specie neutra (§5.2) nella tabella del modello in fondo (5').
2. Per ogni gesto: cosa significa per la specie, quale bisogno tocca e di quanto, se porta in uno stato o lo interrompe, quale limite ha (20'). Un gesto può anche non significare niente: va scritto.
3. Controllare la tabella con quattro domande (15'):
   - **Nutrire.** Con quale gesto si dà da mangiare? Senza un gesto di nutrimento la creatura muore di fame. Il limite di una volta ogni 2 minuti vale per tutte.
   - **Dormire.** Come si mette a dormire? La piattaforma la addormenta già con buio o capovolto per 5 s (§4.3); la specie può aggiungere un gesto proprio.
   - **Spavento.** Quale gesto spaventa? La scossa forte e il colpo sonoro spaventano sempre (§4.3): è della piattaforma.
   - **Stato proprio.** Quale gesto, contesto o ora porta nello stato proprio? Si entra solo da SVEGLIO.
4. Un gesto deve essere **scoperto con piacere** da chi non conosce la specie (criterio Interazione della valutazione). Segnare i due gesti che un estraneo proverà per primi e cosa vedrà (5').

### B · La forma di base su carta (45')

1. **Tre forme in dieci minuti** a persona, sul rettangolo 1:1 e su quello ingrandito. Niente che somigli a un volto: due cerchi vicini sono già occhi.
2. **Scelta** di una forma del gruppo. Criterio: deve avere almeno **quattro variabili separabili** (dimensione, ritmo, colore, estensione).
3. **Le quattro deformazioni**, una per bisogno, disegnate ai due estremi della Figura 6. Poi una combinazione: affamata e felice, stanca e sola.
4. **Le tre intensità** dello stesso stato, per esempio la fame a sussurro, normale e urgente.
5. **Scegliere la famiglia di partenza** tra quelle del frontale (contorno, particelle, campo, linee) e quindi lo sketch da cui copiare la tecnica.

Domande guida al tavolo: *si vede la forma a un metro? a luminosità 10%? cosa resta della specie quando la saturazione è al 10%?*

### C · La forma sul dispositivo (75')

1. Caricare `ProvaEspressione` su un dispositivo e verificare i comandi da seriale a 115200 baud (10').
2. Riscrivere `disegnaForma()` con la forma del gruppo, prima ferma, poi con il respiro (45').
3. Guardare gli fps. Sotto 30, semplificare: meno punti, meno particelle, calcoli fuori dal fotogramma (10').
4. Caricare la stessa versione sul secondo dispositivo (10'). Un lavoro che vive su un solo portatile è un lavoro a rischio.

Il docente gira ai tavoli. Il problema più comune: la forma è bella ferma e illeggibile in movimento.

### D · Le quattro direzioni, il sonno e lo stato proprio (45')

1. **Le quattro direzioni comandate a mano** (20'). Da seriale, un bisogno alla volta: `bisogno nutrimento 10`, poi 100, e così per gli altri. Poi le combinazioni del blocco B. Ogni direzione deve leggersi da sola e insieme alle altre.
2. **Il sonno** (10'). `stato DORME`: luminosità al 10%, nessun colore saturo, respiro a 0,2 Hz. Guardarlo sotto il panno scuro e a un metro.
3. **Lo stato proprio** (15'). Se il gruppo ha già il suo stato proprio, una prima versione dell'espressione, da mettere accanto al sonno sui due dispositivi. Chi guarda deve dire senza esitare quale dei due dorme. Se esita, lo stato proprio imita il sonno e va cambiato.

### E · Lettura incrociata (30')

I gruppi si scambiano i dispositivi, senza la tabella. Un gruppo mette il giro automatico o comanda i bisogni da seriale; l'altro guarda e scrive, per ogni momento, quale bisogno gli sembra basso e quanto è urgente (15'). Poi si confronta con il registro della seriale (15').

Dove la lettura sbaglia, la grammatica non è ancora nella forma. È la stessa prova che si farà senza spiegazioni alla revisione della lezione 7.

---

## Consegna

Per la lezione 5, in un'unica cartella di gruppo:

1. **Tabella gesto → significato → effetto sui bisogni**, completa per i nove gesti (modello sotto).
2. **Forma di base animata**: lo sketch con la `disegnaForma()` del gruppo, caricato sui due dispositivi, con le quattro direzioni comandate a mano da seriale e lo stato DORME secondo il §6.4.
3. Facoltativo: un **video di 30 secondi** del giro automatico, per confrontare le forme alla lezione 5 anche a dispositivi spenti.

### Modello della tabella dei gesti

| Gesto | Neutra (§5.2) | Significato per la specie | Effetto sui bisogni | Stato o transizione | Limite |
| --- | --- | --- | --- | --- | --- |
| scuotere | spavento | | | | la forte spaventa sempre |
| accarezzare | affetto, +5 umore | | | | |
| toccare | tap ripetuti = gioco | | | GIOCOSO se umore > 70 ed energia > 50 | |
| tenere premuto | nutrire, +25 nutrimento | | | | una volta ogni 2 min |
| capovolgere | dormire | | | DORME dopo 5 s | |
| coprire | dormire | | | DORME dopo 5 s di buio | |
| parlare vicino | compagnia, +3 socialità | | | | una volta al minuto |
| tenere in mano | compagnia lenta | | | | |
| posare | — | | | | |
| ingresso nello stato proprio | — | | | PROPRIO, solo da SVEGLIO | intervallo minimo 20 min |

## Punti da decidere

- [ ] **Chi scrive `ProvaEspressione`** e quando: deve essere pronto e provato su due dispositivi prima della lezione. Piano B: i gruppi partono direttamente da ProvaBlob e aggiungono a mano i quattro bisogni e la lettura da seriale, con mezz'ora in meno per la forma.
- [ ] **Interfaccia del disegno**: quali argomenti riceverà la funzione di disegno in `creatura.h` (i quattro bisogni grezzi, i quattro valori della Figura 6, l'intensità, lo stato). Va fissata prima della lezione, perché `disegnaForma()` si possa spostare nella piattaforma alla lezione 6 senza riscriverla.
- [ ] **Valori della Figura 6**: i campi (scala da 0,4 a 1,0, velocità da 0,25× a 2×, saturazione dal 10% al 100%, estensione dal 30% al 100%) sono solo nello schema del PDF, non nel testo del §6.2. Riportarli nel §6.2 e nell'appendice A.
- [ ] **Scossa nella neutra**: il §5.2 dice "scossa = spavento", ma il §4.3 porta in SPAVENTATO solo la scossa forte. Decidere cosa fa la scossa semplice nella neutra.
- [ ] **Sonno e saturazione**: in DORME la regola "nessun colore saturo" vince sulla grammatica dell'umore. Scriverlo nel §6.4, insieme a una soglia (per esempio saturazione non oltre il 30%).
- [ ] **Luminosità**: M5GFX la imposta da 0 a 255 e gli sketch usano 160. Verificare a che valore corrispondono il 5% e il 10% del §6.1 sul CoreS3 e se a quei valori la forma si vede ancora.
- [ ] **Colore provvisorio**: oggi ogni gruppo usa un colore qualsiasi della tavolozza. Dire che non è ancora la scelta del primario, che si fa alla lezione 5.
