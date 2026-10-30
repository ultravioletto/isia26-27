# Lezione 11 · Unisono: autonoma e suonata

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

La voce autonoma, il rapporto con la mano, il visivo del battito e la prova generale. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con la flotta intera e con persone esterne al corso: è l'ultima lezione di lavoro prima della presentazione.

## Obiettivi

Alla fine della giornata ogni studente sa:

- descrivere come suona la **voce autonoma** di ciascuno dei quattro ruoli, e come pattern, ruolo e seme individuale si combinano;
- usare i quattro tratti che regolano il rapporto con le persone e con le altre creature: **docilità**, **ascolto**, **prontezza**, **quorum**;
- prevedere cosa fa la flotta intera a partire dai tratti di ogni specie: chi entra per primo, chi aspetta, chi non entra mai;
- progettare cosa mostra lo schermo durante la grazia dentro le regole del design system;
- preparare una flotta per un pubblico che non conosce le creature: come si fa partire la grazia, come si scopre che la creatura si può suonare.

Ogni gruppo esce con la **specie completa** e l'**involucro montato**. La classe esce con l'esito della prova generale e con l'elenco delle correzioni per la lezione 12.

## Materiali da preparare

- Tutti i dispositivi della classe caricati con la versione più recente di ogni specie, più le due riserve con la specie neutra. Cavi e caricatori USB per tenerli accesi tutta la giornata.
- L'**osservatore** acceso su un portatile, con la registrazione su file.
- Le registrazioni audio delle prove a coppie della lezione 10, pronte da far ascoltare.
- [`unisono-stati.html`](unisono-stati.html) aperta sulla regione della voce.
- **Scheda di osservazione** stampata, una per osservatore (modello in fondo a questo piano).
- **Persone esterne** per il blocco C: da 6 a 10, studenti di altri corsi o docenti, che non hanno mai visto le creature. Invitate prima, con l'orario.
- Accesso al laboratorio di fabbricazione per il blocco E, con le stampe e i tagli prenotati.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: lo strumento che non tace | 10' |
| 0:10 | La voce autonoma e i quattro ruoli | 30' |
| 0:40 | Docilità, ascolto, prontezza, quorum | 30' |
| 1:10 | Il visivo del battito | 20' |
| 1:30 | Come si presenta una flotta a un pubblico | 20' |
| 1:50 | Consegne del pomeriggio | 10' |

### 0:00 · Apertura: lo strumento che non tace (10')

- Il principio 4 del documento: **autonomia e collaborazione**. In unisono la creatura suona da sola e, presa in mano, accetta una mano. Chi la tiene non la sostituisce: partecipa.
- Due strumenti per confronto, a voce: un pianoforte, che tace finché non lo si tocca, e un carillon, che suona solo da sé. La creatura sta nel mezzo, e **quanto** sta nel mezzo è un tratto di carattere.
- Si ascoltano due registrazioni della lezione 10, una dopo l'altra, senza dire di chi sono. La classe prova a dire quale specie suona cosa.

### 0:10 · La voce autonoma e i quattro ruoli (30')

**Come decide la voce** (§8.5). A ogni passo della griglia la creatura si chiede se suonare: guarda la probabilità del passo nel pattern di 16, poi il ruolo sceglie il grado. Pattern e ruolo stanno in config, senza scrivere logica.

| Ruolo | Cosa fa | Cosa serve alla stanza | Si guasta quando |
| --- | --- | --- | --- |
| bordone | una nota lunga e grave sul primo passo ogni due battute, tonica o quinta | il pavimento: una nota ogni 5,3 s | è sotto il do4 e non si sente; ci sono tre bordoni |
| ritmo | note brevi sui passi in levare, sempre lo stesso grado | il battito che tiene insieme | i ritmi sono tutti sugli stessi passi |
| melodia | note sui passi del pattern, un grado alla volta, con tendenza alla tonica | il cammino che si segue | troppe melodie insieme diventano rumore |
| eco | ripete l'ultima nota sentita dai vicini, qualche passo dopo, più piano | la risposta: la stanza sembra ascoltarsi | non ha nessuno da ripetere |

- **Il seme individuale** (§9.2): due individui della stessa specie, con lo stesso pattern, non suonano mai le stesse note nello stesso ordine. Il seme dal MAC guida le scelte casuali della voce.
- **L'altoparlante** (§10.1): il bordone usa la quinta più della tonica grave, perché sotto il do4 il suono sparisce. Le famiglie d'onda contano più qui che altrove: il seno si perde nel coro, la quadra lo buca, il soffio non ha altezza netta e funziona bene come ritmo.
- **La flotta come orchestra.** Alla lavagna, i ruoli scelti alla lezione 10 da tutti i gruppi. Si guarda la composizione: c'è un pavimento? c'è un battito? quante melodie? Se serve, un gruppo cambia ruolo oggi, prima della prova generale.

### 0:40 · Docilità, ascolto, prontezza, quorum (30')

I quattro tratti dell'Appendice A, tutti in config, tutti con `set` al volo.

| Tratto | Valori | Cosa regola | Esempio del §9.4 |
| --- | --- | --- | --- |
| docilità | 0–1 | quanto la voce autonoma si ritira quando la creatura è in mano: la probabilità scende di un fattore (1 − docilità) | la timida, alta: si zittisce e ti lascia suonare |
| ascolto | 0–1 | con che probabilità imita la nota più recente di un vicino invece della propria scelta | la contagiosa, basso: va per la sua strada |
| prontezza | 0–10 s | quanto aspetta dopo l'invito prima di entrare | l'anziana 0 s, la timida 8 s |
| quorum | 0–4 | quanti partecipanti devono esserci già | la timida 3 |

- **Docilità.** Una creatura con docilità 1 in mano è un pianoforte; con docilità 0 è un carillon a cui ci si aggiunge. Quando viene posata, la voce autonoma torna in 2 s di dissolvenza: nessun salto. Il flag "in mano" dice agli altri che una persona sta suonando, e l'eco preferisce le note di una mano.
- **Ascolto.** Con ascolto alto la stanza converge: le creature si imitano e la musica si fa più semplice. Con ascolto basso resta polifonica. Tutto a 1 è un'eco infinita della prima nota.
- **Prontezza e quorum insieme** decidono la forma dell'**ingresso**: tutti subito, oppure a ondate, come gli strumenti che entrano uno dopo l'altro.

**Esercizio lampo (10').** Alla lavagna, i valori di prontezza e quorum dichiarati dai gruppi, più l'origine. Si simula a mano l'ingresso secondo per secondo: chi entra a 0 s, chi a 3, chi aspetta che altri siano entrati. Poi si cambia un valore: se tutte le specie hanno quorum 2 e solo l'origine è dentro, non entra nessuno. Nessuno è sbagliato da solo: è la combinazione che blocca la stanza. Il quorum si conta sugli annunci con il flag della sessione, e l'origine conta.

### 1:10 · Il visivo del battito (20')

- **Lo stato della decisione.** §8.6: il suono è la parte principale; cosa mostra lo schermo durante l'unisono non è ancora deciso (decisione aperta 8). Nel TestSincronia gli schermi lampeggiano a ogni quarto, ma solo per verificare la sincronia.
- **I vincoli che valgono comunque**, dal design system: nessun volto, nessun testo, solo la palette della specie e i neutri, le tre intensità. In grazia l'espressione dei bisogni è sostituita: lo schermo non dice più se la creatura ha fame.
- **Tre famiglie di proposte**, da discutere con la classe:
  1. **Battito comune**: tutte le creature pulsano sul quarto, nella propria palette. La flotta si legge come un corpo solo; il pubblico vede la sincronia.
  2. **Nota visibile**: lo schermo reagisce solo quando la creatura suona, con una forma che dura quanto la nota. Si vede chi suona cosa.
  3. **Le due insieme**: un battito comune in sussurro e un accento quando la creatura suona. Una mano che suona si vede da lontano.
- Domande: la forma di base della specie resta riconoscibile in grazia? Come si distingue a colpo d'occhio una creatura in unisono da una che dorme, quando l'unisono è quieto? Cosa vede chi entra nella stanza a sessione iniziata?
- Si decide in classe una regola comune (cosa pulsa, quando, a quale intensità) e cosa resta alla specie. La regola vale da oggi per tutti.

### 1:30 · Come si presenta una flotta a un pubblico (20')

- **Il pubblico non conosce niente.** Né i nove gesti, né le specie, né l'esistenza dell'unisono. Cosa scopre da solo, cosa gli va detto, chi glielo dice.
- **Come parte la grazia.** Se l'innesco è lasciato tutto alle specie (decisione aperta 4), il pubblico non sa come far partire un unisono. Le strade: un gesto comune a tutte le specie; un innesco per condizione interna che parte da solo quando la stanza è piena; un mediatore che lo fa partire al momento giusto con `unisono avvia`.
- **Il ritmo della stanza.** Una sessione dura 180 s e dopo ci sono 30 s di riposo. In un'ora ci stanno al massimo 16 o 17 sessioni: quante ne vogliamo? Una ogni 10 minuti lascia spazio alla vita delle creature, che è l'altra metà della presentazione.
- **Il fattore tempo.** Con il fattore 1 un pubblico che resta 20 minuti non vede mai una creatura affamata. Con 5 o 10 la cura si vede, ma la creatura trascurata si ammala davanti a tutti. Si discute quale fattore usare alla lezione 12.
- **La stanza fisica**: dove stanno le specie, a che distanza, all'altezza di un tavolo o di una mano. Le creature si devono poter prendere; la vicinanza radio conta per i vicini (§7.2), quindi la disposizione cambia la vita sociale.
- **Cosa osservare nel pomeriggio**: la scheda di osservazione, colonna per colonna.

### 1:50 · Consegne del pomeriggio (10')

- I blocchi del pomeriggio, l'orario delle persone esterne, i ruoli di ogni gruppo durante il blocco C (chi media, chi osserva, chi registra).
- La regola sul visivo decisa al blocco 1:10 e il modo di suonare la creatura in mano, come deciso dopo la lezione 10 (vedi Punti da decidere).

---

## Lavoro di gruppo · 5 ore con il pranzo · la prova generale

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Messa a punto: tratti e visivo | 45' |
| 0:45 | B · Prova generale: l'unisono autonomo | 45' |
| 1:30 | pranzo | 60' |
| 2:30 | C · Prova generale: l'unisono suonato | 60' |
| 3:30 | D · Correzioni | 30' |
| 4:00 | E · Involucro definitivo | 60' |

### A · Messa a punto: tratti e visivo (45')

1. **I quattro tratti** (15'). Docilità, ascolto, prontezza e quorum, con una riga di motivazione ciascuno, coerente con il carattere della specie. Il quorum si controlla con la simulazione alla lavagna: la combinazione della classe non deve bloccare l'ingresso.
2. **Il visivo** (25'). La forma della specie in grazia, dentro la regola comune decisa la mattina. Si programma in `creatura.h` e si prova con `unisono avvia` su un dispositivo solo.
3. **Il binario** (5'). Ogni gruppo carica la versione per la prova generale su entrambi i dispositivi e controlla con `info` versione del firmware e del protocollo.

### B · Prova generale: l'unisono autonomo (45')

La flotta intera nella stanza, tutte le specie, l'osservatore acceso e in registrazione. Nessuno tocca le creature.

- **Prima sessione** (circa 4 minuti con il riposo): parte dall'innesco di una specie, scelta a sorte. Si ascolta da tre punti della stanza.
- **Seconda e terza sessione**: partono dagli inneschi di altre due specie. Con lo stesso ingresso a ondate, ogni volta l'ordine di entrata cambia.
- **Durante**: un gruppo per volta tiene la scheda di osservazione; gli altri ascoltano con gli occhi chiusi per una battuta su quattro.
- **Dopo** (15'): cosa si sente della propria specie? Cosa copre cosa? Le uscite sono come le avevamo progettate? L'osservatore conferma che non ci sono sessioni parallele e che le note per secondo stanno nel conto della lezione 10?

### C · Prova generale: l'unisono suonato (60')

Entrano le persone esterne. Nessuna spiegazione sulle specie; una frase sola all'ingresso, uguale per tutti, scritta prima: per esempio "sono creature, si possono prendere in mano".

- **20 minuti di vita libera.** Le persone girano, prendono le creature, le toccano. Gli osservatori annotano quali gesti provano, quali scoprono, quali non provano mai.
- **Le sessioni.** Almeno due unisoni durante la presenza degli esterni: il primo deve partire da un gesto del pubblico, se l'innesco lo permette; il secondo, se il primo non parte, lo avvia un mediatore.
- **Cosa guardare**: le persone capiscono che la creatura in mano si può suonare? Cosa fanno quando se ne accorgono? Una creatura docile in mano sembra rotta o sembra in ascolto? Una testarda sembra viva o sembra sorda?
- **10 minuti di intervista** alla fine, a coppie di studenti con una persona esterna: cosa hai fatto, cosa è successo, cosa ti aspettavi.

### D · Correzioni (30')

Ogni gruppo legge le schede di osservazione che lo riguardano e sceglie **al massimo tre correzioni**, scritte in una tabella: cosa si è visto, cosa si cambia, in quale parametro o funzione. Le correzioni di parametri si fanno con `set` e `save`; quelle di logica e disegno si ricompilano.

Le osservazioni che riguardano tutta la flotta (una specie che copre tutte le altre, un innesco che il pubblico non trova, una disposizione della stanza che non funziona) si portano al docente e si decidono insieme per la lezione 12.

### E · Involucro definitivo (60')

L'involucro si progetta dalla lezione 7. Oggi si monta quello definitivo e si verifica con la regola 10 e con le cose che l'unisono aggiunge.

- **Impronta** 54 × 54 mm; materiale libero, ma rigido, perché l'IMU senta la mano e non il guscio.
- **Finestre libere**: schermo, microfono, altoparlante, USB-C, finestra del sensore di luce. In grazia l'altoparlante è il canale principale: si fa una prova di ascolto con e senza involucro, alla stessa distanza.
- **Antenna**: niente metallo sul bordo superiore. Si controlla l'RSSI medio da seriale con e senza involucro.
- **Gesti**: i gesti touch si provano con l'involucro montato (§12); l'inclinazione e il tap, se servono a suonare, anche.
- **Base**: la base standard ha quattro magneti che saturano la bussola. Se l'involucro sostituisce la base, si annota se è senza magneti.

---

## Consegna

Per la lezione 12, in un'unica cartella di gruppo:

1. **Specie completa** su due dispositivi: `config.h` e `creatura.h` con carattere, stato proprio, reazioni ai nove gesti, messaggi propri, espressione, parte di unisono completa (ruolo, pattern, ottava, innesco, uscita, docilità, ascolto, prontezza, quorum) e visivo della grazia.
2. **Involucro montato** su entrambi i dispositivi, conforme alla regola 10, con le prove del blocco E annotate: ascolto, RSSI, gesti touch.
3. La **tabella delle correzioni** del blocco D, con cosa è stato fatto.

Promemoria per la consegna finale della lezione 12, da preparare da oggi: scheda di specie, video di un minuto, un'ora di vita registrata da seriale in forma di grafico.

### Modello della scheda di osservazione

| Ora | Specie | Chi | Gesto o evento | Cosa fa la creatura | Cosa fa la persona | Note |
| --- | --- | --- | --- | --- | --- | --- |
| | | esterno / studente | | | | |

In fondo alla scheda, per ogni sessione di unisono: chi l'ha fatta partire e come, quanti secondi per l'ingresso di tutta la flotta, se qualcuno ha suonato una creatura in mano, cosa si è sentito all'uscita.

## Punti da decidere

- [ ] **Come si suona la creatura in mano** (§8.4, decisione 7). Senza una decisione presa prima di oggi il blocco C prova solo l'unisono autonomo con persone intorno. La proposta in `unisono-stati.html`: ogni tap suona sempre, quantizzato sul passo più vicino; l'inclinazione sceglie grado e ottava.
- [ ] **Regola comune del visivo** (§8.6, decisione 8): quella decisa in classe va riportata nel documento di progetto e, se la piattaforma la impone, in `unisono.h`.
- [ ] **Innesco per il pubblico** (decisione 4): gesto comune, innesco interno o mediatore. Da fissare prima della lezione 12.
- [ ] **Piattaforma con l'unisono**: questo piano presuppone che `unisono.h` esista e sia caricato. Nel repository non c'è ancora. Senza, la prova generale non si può fare e la lezione 11 diventa una seconda giornata di prove su carta e con il TestSincronia.
- [ ] **Persone esterne**: chi invitare, quante, e se lo stesso gruppo di persone torna alla lezione 12 o se ne serve uno nuovo, che non abbia già visto le creature.
- [ ] **Fattore tempo alla lezione 12**: 1, 5 o 10.
- [ ] **Firmware dell'osservatore**: §11.4 lo descrive, ma non è tra gli sketch di `code/arduino/`. Va scritto o recuperato dalla lezione 9.
- [ ] **Basi senza magneti**: se qualche gruppo vuole la bussola, l'involucro deve sostituire la base standard.
