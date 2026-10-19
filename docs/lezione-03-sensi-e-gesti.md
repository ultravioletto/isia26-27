# Lezione 3 · Sensi e gesti

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 19 ottobre 2026 · Bruno

Dalla misura all'evento, i nove gesti comuni e il contesto. **2 ore frontali + 5 ore di gruppo**, pranzo compreso, con i dispositivi: è la lezione in cui ogni gruppo riceve i suoi due CoreS3.

## Obiettivi

Alla fine della giornata ogni studente sa:

- seguire la **catena dei sensi**: sensore, misura grezza, filtro, soglia, evento, gesto, significato;
- usare gli attrezzi che trasformano un segnale rumoroso in un evento affidabile: filtro, soglia, isteresi, tempo minimo, finestra massima, pausa dopo l'evento;
- riconoscere i **nove gesti** e i tre **contesti**, e spiegare perché la piattaforma definisce il gesto e la specie ne decide il significato;
- dire cosa il CoreS3 sente bene, cosa sente male e cosa non sente affatto.

Ogni gruppo esce con due dispositivi funzionanti e registrati, e con la **tabella delle misure** della stanza vera. La classe esce con una prima proposta di soglie comuni a tutta la flotta.

## Materiali da preparare

- Dispositivi: 2 CoreS3 per gruppo più 2 di riserva, carichi, con un'etichetta numerata sul retro. Cavi USB-C, uno per dispositivo.
- Esploratore CoreS3 compilato e provato su un dispositivo del docente prima della lezione.
- Figura 5 del documento di progetto (la catena dei sensi) e la tabella del §5.1, stampate in A3, una per gruppo.
- **Scheda delle misure** stampata, una per gruppo (modello in fondo a questo piano).
- Per le postazioni di misura: una lampada da tavolo, un panno scuro, un righello, un cronometro (va bene il telefono).
- Un foglio grande o la lavagna per la **tabella di classe** del blocco E.

---

## Parte frontale · 2 ore

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Apertura: come viene tenuta | 10' |
| 0:10 | La catena dei sensi | 25' |
| 0:35 | Gli attrezzi del filtro | 30' |
| 1:05 | I nove gesti e il contesto | 25' |
| 1:30 | Cosa sente davvero il CoreS3 | 20' |
| 1:50 | Consegna dei dispositivi e regole | 10' |

### 0:00 · Apertura: come viene tenuta (10')

- Il principio del documento: la creatura reagisce a **come viene tenuta**, non a cosa viene premuto.
- Domanda alla classe: come fa il telefono a sapere che lo avete girato, che lo state portando in tasca, che lo avete appoggiato a faccia in giù? Si raccolgono le risposte. Dietro ognuna c'è un sensore, un filtro e una soglia.
- Il ponte con la lezione 2: le transizioni della macchina a stati partono da **eventi**. Oggi si vede da dove vengono gli eventi.

### 0:10 · La catena dei sensi (25')

- La Figura 5, anello per anello:
  1. **Sensore**: IMU, microfono, luce e prossimità, touch, alimentazione e orologio.
  2. **Misura grezza**: numeri che cambiano molte volte al secondo, con rumore.
  3. **Filtro**: toglie il rumore e la gravità, lascia il gesto.
  4. **Soglia, isteresi e tempi**: trasformano una misura continua in un evento con un inizio e una fine.
  5. **Evento** (scossa, capovolto, voce vicina) e **misura continua** (inclinazione, rumore).
  6. **Gesto**: uno o più eventi letti insieme, uguali per tutta la flotta.
  7. **Significato**: lo decide la specie.
- I primi cinque anelli sono della piattaforma, il settimo è della specie. Il sesto è il confine: il gesto è una **parola comune**, il significato è il **dialetto** della specie.
- Le unità con cui si lavora oggi, come le mostra l'Esploratore: **g** per l'accelerazione, **gradi** per l'inclinazione, **dBFS** per il suono, **conteggi** per la luce (non lux: vedi il blocco 1:30).

### 0:35 · Gli attrezzi del filtro (30')

**Dimostrazione dal vivo (5').** Un dispositivo fermo sul tavolo, pagina Accelerometro proiettata. Il modulo dovrebbe essere 1 g e invece oscilla: è il rumore. Poi lo si sposta piano, poi lo si scuote.

**Gli attrezzi**, ciascuno con un esempio del §5.1:

| Attrezzo | Cosa fa | Esempio nel documento |
| --- | --- | --- |
| Filtro | liscia la misura: meno rumore, ma un po' di ritardo | inclinazione filtrata a 5 Hz |
| Soglia | separa "niente" da "qualcosa" | scossa oltre 1,5 g |
| Isteresi | si entra a un valore, si esce a un altro | la stessa idea delle soglie dei bisogni, lezione 2 |
| Tempo minimo | la condizione deve durare | capovolto per 1 s, buio per 2 s, voce per più di 1 s |
| Finestra massima | l'evento deve essere breve | scossa sotto 300 ms, tap sotto 200 ms |
| Pausa dopo l'evento | un gesto non conta due volte | tre scosse di fila sono tre scosse o una sola? |

Alla lavagna: lo stesso segnale rumoroso letto con una soglia sola, con isteresi, con isteresi e tempo minimo. Si contano gli eventi falsi nei tre casi.

**Esercizio lampo (10').** A coppie, senza guardare il documento: definire **coprire** oppure **carezza** con un sensore, una soglia e un tempo. Due coppie leggono la loro definizione, poi si confronta con il §5.1. Di solito "coprire" viene confuso con "fare buio": il documento li distingue con la velocità della caduta di luce, cioè con un tempo.

### 1:05 · I nove gesti e il contesto (25')

| Gesto | Sensore | Definizione di partenza (§5.1) | Si confonde con |
| --- | --- | --- | --- |
| scuotere | IMU | oltre 1,5 g per meno di 300 ms; forte oltre 3 g | camminare con la creatura in tasca, posarla con forza |
| accarezzare | touch | trascinamento oltre 40 px, sotto 150 px al secondo | un trascinamento veloce |
| toccare | touch | contatto sotto 200 ms; doppio entro 400 ms | — |
| tenere premuto | touch | contatto oltre 1 s (3 s debug, 5 s rinascita) | tenerla in mano con il pollice sullo schermo |
| capovolgere | IMU | asse z verso il basso per 1 s | posarla a faccia in giù per caso |
| coprire | luce | da oltre 50 a meno di 5 lux in meno di 0,5 s | il tramonto, una nuvola, la tasca |
| parlare vicino | microfono | livello tra 0,3 e 0,7 per più di 1 s | la musica, la classe che parla |
| tenere in mano | IMU, touch | micromovimenti o contatto; finisce dopo 3 s di quiete | il tavolo che vibra |
| posare | IMU | fine di "in mano"; fermo dopo 30 s | — |

- **Il contesto** (§5.3): sul tavolo, in mano, in tasca. In tasca si riconosce dal buio e dal passo, tra 1 e 3 Hz.
- **Perché la piattaforma definisce il gesto.** Chi impara i nove gesti può avvicinarsi a qualsiasi creatura. Se ogni specie definisse "scuotere" a modo suo, la stessa mano spaventerebbe una creatura e non ne sveglierebbe un'altra.
- **Perché la specie decide il significato.** Per una specie la carezza è affetto, per un'altra fastidio. La neutra assegna: pressione lunga = nutrire, carezza = affetto, tap ripetuti = gioco, voce vicina = compagnia, scossa = spavento, capovolgere o coprire = dormire, tenere in mano = compagnia lenta.
- **Il ponte con lo stato proprio** (§4.4): la condizione d'ingresso può usare gesti, contesto e ora del giorno. Un buon stato proprio nasce spesso da un gesto che la neutra non usa.

### 1:30 · Cosa sente davvero il CoreS3 (20')

Quello che abbiamo già scoperto provando i dispositivi, da dire prima che i gruppi ci sbattano contro.

- **I due microfoni non danno la direzione.** Sono troppo vicini: la differenza di livello e il ritardo tra i due canali non bastano a capire da dove arriva il suono. Si trattano come un microfono solo.
- **La bussola non funziona con la base standard.** I quattro magneti della base saturano il magnetometro: un asse va fuori scala, gli altri leggono 5–10 volte il campo terrestre. Con quella base la bussola non si usa; la pagina Bussola dell'Esploratore lo segnala.
- **Il sensore di luce non dà lux.** La pagina Luce mostra conteggi grezzi. Le soglie del §5.1 sono in lux: oggi si misura la stanza in conteggi, e la traduzione si fa con le misure.
- **Il microfono si legge in dBFS.** L'Esploratore mostra da −60 a 0 dBFS e lo porta su una scala da 0 a 1. Se la piattaforma userà la stessa scala, le soglie del §5.1 diventano:

| Soglia del §5.1 | Livello da 0 a 1 | dBFS sulla pagina Microfono |
| --- | --- | --- |
| voce vicina, minimo | 0,3 | −42 |
| voce vicina, massimo | 0,7 | −18 |
| colpo sonoro | oltre 0,8 | oltre −12 |

- **Le soglie del §5.1 sono valori di partenza**, scritti prima di avere i dispositivi in mano. La giornata di oggi serve a verificarli.

### 1:50 · Consegna dei dispositivi e regole (10')

- Ogni gruppo riceve due dispositivi numerati e firma il registro.
- Regole: si caricano a fine giornata; non si apre la base; non si toglie l'etichetta; un dispositivo che non si accende va al docente, che consegna una riserva.
- Il protocollo del pomeriggio, blocco per blocco.

---

## Lavoro di gruppo · 5 ore con il pranzo · misurare la stanza

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Accendere, caricare, registrare | 45' |
| 0:45 | B · Le quattro postazioni di misura | 75' |
| 2:00 | pranzo | 60' |
| 3:00 | C · Dalle misure alle soglie | 45' |
| 3:45 | D · Il corpo della creatura | 30' |
| 4:15 | E · Tabella di classe e soglie di flotta | 45' |

### A · Accendere, caricare, registrare (45')

1. Compilare e caricare l'Esploratore CoreS3 su entrambi i dispositivi, dai portatili preparati alla lezione 1.
2. Percorrere le 15 pagine una volta, senza misurare: cosa c'è, cosa si muove, cosa non funziona.
3. Dalla pagina **Info**, copiare nel registro il **MAC** di ogni dispositivo e scegliere un nome breve. Servono per la tabella di flotta (§3.5): il MAC dà l'identità e il seme individuale di ogni creatura.
4. Controllare la batteria dalla pagina Alimentazione.

### B · Le quattro postazioni di misura (75')

I gruppi ruotano su quattro postazioni, circa 18 minuti ciascuna. In ogni postazione: una persona fa il gesto, una legge la pagina, una scrive. Ogni situazione si ripete **5 volte**, possibilmente da persone diverse, e si annotano il minimo, il valore tipico e il massimo.

**1. Scossa e inclinazione** (pagina Accelerometro: picco scossa in g, beccheggio e rollio in gradi)
- scossa leggera, scossa decisa, scossa forte;
- posare il dispositivo sul tavolo, normalmente e con forza;
- camminare per la stanza con il dispositivo in tasca o in mano;
- l'inclinazione a cui "si vede" che è inclinato; capovolgerlo.

**2. Voce e rumore** (pagina Microfono: dBFS)
- la stanza in silenzio; la stanza con tutti che lavorano;
- voce normale a 20 cm e a 1 m; sussurro a 20 cm;
- battito di mani a 50 cm; un oggetto che cade sul tavolo.

**3. Luce e prossimità** (pagina Luce e prossimità: conteggi)
- vicino alla finestra, al centro della stanza, sotto il banco;
- sotto la lampada; coperto dalla mano; coperto dal panno; in tasca;
- prossimità: la mano a 1, 3 e 10 cm.

**4. Touch** (pagina Touch: posizione dei contatti e ultimo gesto riconosciuto, cioè tap, pressione lunga o trascinamento; la pagina non mostra durate né velocità, che si stimano con il cronometro e il righello)
- tap, doppio tap, pressione lunga, cronometrandoli;
- carezza lenta e trascinamento veloce: lunghezza e durata;
- toccare lo schermo con il dispositivo in mano, senza volerlo.

### C · Dalle misure alle soglie (45')

Per ogni senso il gruppo disegna una **retta dei valori**: sotto, le situazioni che non devono far scattare l'evento; sopra, quelle che devono. Poi ci colloca la soglia del §5.1.

- La soglia cade nello spazio vuoto tra i due gruppi di valori? Allora funziona.
- Cade in mezzo a un gruppo? Ci saranno eventi falsi (scatta senza motivo) o mancati (non scatta quando dovrebbe). Quale dei due è peggio per una creatura?
- Serve un'isteresi? Un tempo minimo? Una finestra più stretta?

Il caso da guardare con attenzione: **camminare con la creatura in tasca supera 1,5 g?** Se sì, ogni passeggiata è una serie di scosse, e la creatura neutra si spaventa a ogni passo.

### D · Il corpo della creatura (30')

Dalle misure al carattere. Ogni gruppo, sulla propria idea di specie della lezione 2:

- disegna il dispositivo e segna dove la creatura "si tocca" e come "si tiene";
- sceglie **tre gesti** che per la propria specie sono importanti e scrive in una riga cosa potrebbero significare;
- se ha già un'idea di **stato proprio**, scrive con quale gesto, contesto o ora del giorno si entra.

È una prima ipotesi: la tabella gesto → significato completa è la consegna della lezione 4.

### E · Tabella di classe e soglie di flotta (45')

Ogni gruppo riporta i propri valori tipici in una tabella comune alla lavagna. Con tutta la classe:

- si guarda quanto variano le misure tra gruppi e tra dispositivi;
- si decide una proposta di **soglie di flotta v0**: per ogni evento del §5.1, la soglia confermata o quella nuova, con il motivo.

Le soglie di flotta valgono per tutti: un gruppo non può cambiarle per la propria specie. È la prima decisione comune della flotta.

---

## Consegna

Per la lezione 4, in un'unica cartella di gruppo:

1. **Tabella delle misure** compilata (modello sotto).
2. **Registro dei dispositivi**: numero dell'etichetta, MAC, nome breve.
3. **Tre gesti** della specie con una riga di significato e, se c'è, l'ingresso dello stato proprio.

### Modello della tabella delle misure

| Senso | Situazione | Pagina | Unità | Min | Tipico | Max | Soglia §5.1 | Scatta? |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| scossa | scossa leggera | Accelerometro | g | | | | 1,5 g | |
| scossa | scossa forte | Accelerometro | g | | | | 3 g | |
| scossa | posata con forza | Accelerometro | g | | | | 1,5 g | non deve |
| scossa | camminare con il dispositivo in tasca | Accelerometro | g | | | | 1,5 g | non deve |
| inclinazione | "si vede che è inclinato" | Accelerometro | gradi | | | | — | |
| suono | stanza in silenzio | Microfono | dBFS | | | | — | |
| suono | voce normale a 20 cm | Microfono | dBFS | | | | −42…−18 | deve |
| suono | voce a 1 m | Microfono | dBFS | | | | −42…−18 | |
| suono | battito di mani a 50 cm | Microfono | dBFS | | | | oltre −12 | deve |
| luce | stanza, centro | Luce | conteggi | | | | — | |
| luce | coperto dalla mano | Luce | conteggi | | | | sotto 5 lux | deve |
| luce | in tasca | Luce | conteggi | | | | — | |
| prossimità | mano a 3 cm | Luce | conteggi | | | | "pochi cm" | deve |
| touch | tap | Touch | ms | | | | sotto 200 ms | |
| touch | carezza lenta | Touch | px e s | | | | oltre 40 px, sotto 150 px/s | deve |

## Punti da decidere

- [ ] **Firmware della giornata**: l'Esploratore da solo, compilato dai gruppi, oppure la Raccolta caricata dal docente prima della lezione. La prima strada verifica l'ambiente di ogni portatile, la seconda fa risparmiare mezz'ora.
- [ ] **Base del CoreS3**: tenere la base standard, che toglie la bussola, oppure procurare basi o involucri senza magneti per chi vuole usarla.
- [ ] **Luce in lux**: tarare il sensore con un luxmetro, anche quello di un telefono, oppure riscrivere le soglie di luce del §5.1 direttamente in conteggi.
- [ ] **Scala del microfono**: confermare che la piattaforma userà la stessa scala dell'Esploratore (da −60 a 0 dBFS su 0–1).
- [ ] **Chi tiene il registro dei dispositivi** e dove vive la tabella di flotta (`flotta.h`, §3.4).
- [ ] **Soglie di flotta v0**: dopo la lezione, riportare nel documento di progetto (§5.1 e appendice A) i valori decisi dalla classe.
