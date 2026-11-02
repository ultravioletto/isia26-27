# Lezione 3 · Laboratorio dei sensi

Interaction Design 2026–27 · Creature in rete · scheda d'uso · bozza 30 ottobre 2026 · Bruno

Come usare il laboratorio dei sensi dell'Esploratore CoreS3 per andare a fondo su soglia, isteresi, tempo minimo, finestra, pausa e filtro, **un senso alla volta**, con il dispositivo in mano e il Monitor seriale di Arduino. Serve nella parte frontale (blocco 0:35, gli attrezzi del filtro) e nel pomeriggio (blocchi B e C). Il codice è nella sezione "Laboratorio dei sensi" di [`EsploratoreCoreS3.ino`](../code/arduino/EsploratoreCoreS3/EsploratoreCoreS3.ino).

## Cos'è

Un laboratorio prende **una misura** (lo scarto dell'accelerazione da 1 g, il livello del microfono, la luce…), la passa da un **filtro** e poi a un **riconoscitore**: una piccola macchina a stati, la stessa della lezione 2, che decide quando c'è un evento. Tutti i parametri si cambiano scrivendo nel Monitor seriale, mentre il dispositivo continua a sentire. Così si vede subito cosa fa ogni attrezzo.

```
QUIETO --la misura passa la soglia--> DENTRO
DENTRO --esce entro la finestra, dopo il minimo--> evento --> PAUSA o QUIETO
DENTRO --esce troppo presto o troppo tardi--> scartato --> QUIETO
DENTRO --finestra 0 e minimo raggiunto--> evento "inizio" --> ATTIVO
ATTIVO --esce (soglia meno isteresi)--> evento "fine" --> PAUSA o QUIETO
PAUSA  --passata la pausa--> QUIETO
```

Lo stato in cui si trova la macchina è scritto sullo schermo del dispositivo, in alto a destra.

## Prepararsi

1. Caricare l'Esploratore dalla IDE di Arduino. In *Strumenti*: scheda **M5Stack › M5CoreS3** (non quella del pacchetto "esp32", che ha lo stesso nome), **USB CDC On Boot: Enabled**, la porta COM del dispositivo.
2. Aprire il **Monitor seriale** a **115200 baud**. Il fine riga va bene "Nuova riga"; con "Nessun fine riga" il comando parte dopo un attimo di pausa.
3. Premere il tasto di reset del CoreS3: compare `Laboratorio dei sensi: scrivi ? e premi Invio`.
4. Se il Monitor resta vuoto e lo schermo è nero o fermo, il dispositivo è finito in modo download aprendo il Monitor: chiudere il Monitor, reset, riaprire.

## I comandi

| Comando | Cosa fa |
| --- | --- |
| `?` | aiuto, e i parametri del laboratorio in corso con il loro significato |
| `lab` | elenco dei dieci laboratori |
| `scossa` | avvia il laboratorio "scossa" (vale per tutti i nomi) |
| `soglia 1,2` | cambia un parametro; va bene la virgola o il punto |
| `soglia` | mostra il valore attuale |
| `valori` | rimette i valori di partenza, quelli del §5.1 |
| `azzera` | azzera i conteggi |
| `testo` | un evento per riga nel Monitor (è il modo all'avvio) |
| `grafico` | solo numeri, per il Plotter seriale |
| `fine` | chiude il laboratorio e torna alle pagine; oppure tocca la barra in alto |

In modo **testo** ogni esito è una riga:

```
   12.34 s  scossa  #3  durata 44 ms  picco 2.21 g
   13.02 s  scartato, troppo breve  durata 12 ms  picco 1.62 g
   20.10 s  capovolto inizio  #1  durata 1000 ms  picco 0.98 g
   23.45 s  capovolto fine  durata 4350 ms  picco 1.00 g
```

Le righe si possono selezionare e copiare dal Monitor: sono già i dati della tabella delle misure.

In modo **grafico** il dispositivo manda circa 20 righe al secondo nel formato del Plotter (`misura:…,filtrata:…,soglia:…,uscita:…,evento:…`). Si chiude il Monitor, si apre *Strumenti › Plotter seriale*, e i comandi si scrivono nella casella in basso del Plotter. Tra una riga e l'altra il dispositivo tiene il valore più estremo, così un picco di pochi millisecondi si vede lo stesso. In questo modo le modifiche non danno conferma scritta: si vedono come una linea che si sposta. Per tornare a leggere: `testo`.

Sullo schermo del dispositivo ci sono sempre la misura filtrata, la traccia degli ultimi secondi con le linee di soglia (rossa la soglia, arancio l'uscita, viola il tetto), una riga verde per ogni evento e arancio per ogni scarto, i conteggi e l'ultimo esito.

## I parametri

| Parametro | Cosa fa | Domanda da farsi |
| --- | --- | --- |
| `soglia` | si entra quando la misura la supera (per il buio: quando ci scende sotto) | dove cade tra "non deve scattare" e "deve scattare"? |
| `isteresi` | si esce solo un'isteresi più in là della soglia | la misura tremola sulla soglia? |
| `minimo` | la condizione deve durare almeno tanto (ms) | ci sono picchi brevissimi che non sono gesti? |
| `finestra` | l'evento deve finire entro tanto (ms); 0 = evento con inizio e fine | il gesto è un colpo o una condizione che dura? |
| `pausa` | dopo un evento non si ascolta per tanto (ms) | lo stesso gesto conta due volte? |
| `filtro` | passa-basso, in Hz; liscia ma ritarda | il rumore è più veloce del gesto? |
| `tetto` | solo per la voce: sopra non è più voce vicina | |

Regola di lavoro: **si cambia un parametro alla volta**, si rifà lo stesso gesto cinque volte, si scrive cosa è cambiato. `valori` riporta al punto di partenza.

---

## Gli esperimenti, un senso alla volta

Ogni esperimento dura 10–15 minuti. Una persona fa il gesto, una scrive i comandi e legge, una annota. Ogni volta: **cosa si vede con i valori del §5.1**, **quale attrezzo lo corregge**, **cosa costa la correzione**.

### 1. Scossa · `scossa`

Misura: quanto il modulo dell'accelerazione si allontana da 1 g, cioè l'accelerazione senza la gravità. Ferma sul tavolo vale circa 0,01 g. Partenza: soglia 1,5 g, finestra 300 ms.

1. Una scossa decisa, avanti e indietro. Quanti eventi? Di solito due o tre: una scossa è un avanti-indietro-avanti e passa la soglia a ogni mezza oscillazione.
2. `pausa 300` e di nuovo. Ora una scossa conta una volta. Tre scosse di fila veloci quante ne contano?
3. Posare il dispositivo sul tavolo con forza. Scatta? Guardare la durata: un colpo secco sta sopra soglia pochissimo. Provare `minimo 20`: il colpo viene scartato come "troppo breve". Una scossa leggera passa ancora?
4. Camminare per la stanza con il dispositivo in tasca. Quante scosse false in un minuto? Alzare la soglia le toglie, ma quale scossa leggera si perde? Qui nessun valore va bene per tutto: serve il **contesto** "in tasca" (§5.3).
5. `soglia 3`: è la scossa forte. Quanto bisogna scuotere?

### 2. Capovolto · `capovolto`

Misura: l'asse z rivolto in basso, in g; vale −1 a schermo in su, +1 a schermo in giù. Partenza: soglia 0,8, minimo 1000 ms, finestra 0 (evento con inizio e fine).

1. Capovolgere il dispositivo sul tavolo: "inizio" dopo un secondo, "fine" quando lo si rigira. La durata della fine è quanto è rimasto capovolto.
2. `minimo 0`: girarlo in mano come per guardarlo sotto. Scatta anche così? A cosa serve il secondo d'attesa?
3. Tenerlo inclinato proprio al limite, intorno a 0,8. Inizio e fine si alternano: è il tremolio sulla soglia. `isteresi 0,3`: ora per uscire bisogna scendere sotto 0,5.

### 3. Inclinazione · `inclinata`

Misura: l'angolo dalla posizione piatta, in gradi. Fermo sul tavolo segna già 2–3 gradi. Partenza: soglia 30, filtro 5 Hz, come il §5.1.

1. In modo grafico, al Plotter: `filtro 0`, poi `filtro 5`, poi `filtro 1`. Tenendolo fermo in mano, quanto rumore resta?
2. Inclinarlo di scatto oltre 30 gradi. Con `filtro 1` quanto arriva in ritardo la curva filtrata? È il prezzo del filtro: meno rumore, più ritardo.
3. Tenerlo intorno ai 30 gradi: `isteresi 10`. Per una creatura che "si sporge" quando la si inclina, quale filtro e quale isteresi danno un movimento credibile?

### 4. Voce vicina · `voce`

Misura: il livello dei due microfoni insieme, da −60 a 0 dBFS portato su 0–1 (0,3 = −42 dBFS, 0,7 = −18 dBFS). Partenza: soglia 0,3, tetto 0,7, minimo 1000 ms, finestra 0.

1. Parlare normalmente a 20 cm. Probabilmente arrivano molti "scartato, troppo breve": tra una sillaba e l'altra il livello scende sotto la soglia e il tempo riparte da zero.
2. Due strade: `isteresi 0,1` (si esce solo sotto 0,2) oppure `filtro 2` (si liscia il livello). Provarle una alla volta. Quale delle due fa scattare la voce prima?
3. Parlare a 1 m, sussurrare a 20 cm, gridare vicino (oltre il tetto). Con la classe che lavora, il fondo da solo supera la soglia?

### 5. Colpo sonoro · `colpo`

Stessa misura della voce. Partenza: soglia 0,8 (−12 dBFS), finestra 300 ms.

1. Battere le mani a 50 cm, poi a 2 m. Da quanto lontano scatta?
2. Un "aaah" forte e lungo: viene scartato come "troppo lungo". È la finestra che separa il colpo da un suono forte che dura.
3. Un oggetto che cade sul tavolo: è un colpo per la creatura? Deve esserlo?

### 6. Buio · `buio`

Misura: la luce ambiente in **conteggi** del sensore, non in lux. Il sensore misura ogni 100 ms. Partenza: soglia 5, minimo 2000 ms.

1. Leggere sullo schermo i conteggi della stanza: vicino alla finestra, al centro, sotto il banco, in tasca. La soglia di 5 del §5.1 è in lux: in conteggi va trovata.
2. Mettere la soglia tra "sotto il banco" e "in tasca". Mettere il dispositivo in tasca: dopo 2 secondi "buio inizio".
3. Passarci sopra una mano: senza il minimo sarebbe buio? A cosa servono i 2 secondi?

### 7. Coperto · `coperto`

Misura: la stessa luce. Non è una soglia sola: la luce deve passare da sopra `da` (50) a sotto `a` (5) entro `finestra` (500 ms). Gli stati sono ALTO, CADUTA, BASSO.

1. Prima riportare `da` e `a` in conteggi, dalle misure dell'esperimento 6.
2. Coprire il sensore con la mano, di colpo: "coperto". Durata: quanto ci ha messo la luce a scendere. Le durate vengono a scatti di 100 ms, il ritmo del sensore.
3. Abbassare la mano lentamente sopra il sensore, o spegnere la lampada piano: "scartato, troppo lento". È la differenza tra la mano e il tramonto, fatta con un tempo.
4. Quanto deve essere stretta la finestra perché una nuvola non sembri una mano?

### 8. Mano vicina · `mano`

Misura: la prossimità a infrarossi, da 0 a 2047; sale avvicinando la mano. Partenza: soglia 150.

1. Annotare i conteggi con la mano a 1, 3 e 10 cm.
2. Tenere la mano ferma alla distanza della soglia: inizio e fine si inseguono. È il caso più chiaro per spiegare l'isteresi.
3. `isteresi 50`, poi `isteresi 100`. Quanto bisogna allontanare la mano perché la creatura "senta" che se n'è andata?

### 9. Toccare e tenere premuto · `tocco`

Misura: 1 se c'è un dito sullo schermo, 0 se no. Partenza: finestra 200 ms, cioè il tap. Toccare sotto la barra in alto: un tocco sulla barra chiude il laboratorio.

1. Tap veloci e tap lenti: quali vengono scartati come "troppo lunghi"? Le durate in ms sono quelle da mettere nella tabella delle misure.
2. Lo stesso segnale, altri parametri: `finestra 0` e `minimo 1000`. Ora è la **pressione lunga**: inizio dopo un secondo, fine quando si alza il dito. Con `minimo 3000` è il tocco che apre il debug.
3. Il doppio tap (due tocchi entro 400 ms) il laboratorio non lo riconosce: guardare gli orari di due tap consecutivi nel Monitor e dire se la finestra di 400 ms del §5.1 basta.
4. Tenere il dispositivo in mano come si tiene un telefono: il pollice tocca lo schermo senza volerlo?

### 10. Carezza · `carezza`

Misura: la velocità del dito sullo schermo, in pixel al secondo. Il dito deve fare almeno `distanza` (40 px) senza superare `velocita` (150 px/s). Gli stati sono LIBERO, CONTATTO, FATTA. La riga dell'evento dice anche la direzione.

1. Una carezza lenta, poi un trascinamento veloce: "carezza verso destra" contro "scartato, troppo veloce".
2. Al Plotter, `filtro 0`: la velocità salta tra zero e valori alti, perché lo schermo aggiorna la posizione meno spesso di quanto la si legge. Una carezza lenta viene scartata per un picco finto. `filtro 5` la salva.
3. Quanto è lenta una carezza vera? Cronometrare cinque persone e decidere se 150 px/s va bene per la classe.

---

## Nella lezione

- **Frontale, 0:35 · gli attrezzi del filtro.** Al posto della lavagna: laboratorio `scossa` al Plotter proiettato. Valori del §5.1, poi `pausa 300`, poi `minimo 20`, contando gli eventi falsi a ogni passo. Poi `mano` per l'isteresi.
- **Gruppo, A.** Caricando l'Esploratore si carica anche il laboratorio. Ogni gruppo prova `?` e `scossa` per controllare che la seriale funzioni.
- **Gruppo, B · le postazioni.** Le pagine dell'Esploratore danno i valori; il laboratorio dà le durate (tap, carezza, coperto) che le pagine non mostrano. Postazione 1: esperimenti 1–3. Postazione 2: 4–5. Postazione 3: 6–8. Postazione 4: 9–10.
- **Gruppo, C · dalle misure alle soglie.** Ogni proposta di soglia si prova nel laboratorio prima di scriverla: cinque gesti che devono scattare, cinque che non devono. Contare giusti, falsi e mancati.
- **Gruppo, E · soglie di flotta.** Per ogni valore proposto, il gruppo dice con quale parametro e quale esperimento l'ha deciso.

## Limiti da sapere

- Il dispositivo sente una cosa alla volta: un laboratorio, un sensore. La specie, nella piattaforma, sentirà tutto insieme.
- Ridisegnare lo schermo toglie circa 40 ms di misure. Il laboratorio non ridisegna mentre un evento breve è in corso, ma un picco che comincia proprio in quel momento può arrivare in ritardo.
- Luce e prossimità sono in conteggi grezzi. Le soglie del §5.1 in lux vanno tradotte con le misure della stanza, oppure riscritte direttamente in conteggi.
- I parametri cambiati non restano dopo un riavvio: si riparte sempre dai valori del §5.1. I valori scelti vanno scritti nella tabella.
- Non si sa ancora quanti punti mostri la finestra del Plotter seriale della IDE 2. Se è troppo corta per vedere un gesto intero, si legge in modo testo.
