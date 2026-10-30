# Lezione 12 · La stanza

Interaction Design 2026–27 · Creature in rete · piano di lezione · bozza 23 ottobre 2026 · Bruno

Allestimento, ultima prova di flotta, presentazione, critica collettiva e retrospettiva. **2 ore di allestimento + 5 ore di presentazione e critica**, pranzo compreso. È la seconda verifica del corso e l'unica valutata insieme alla revisione intermedia della lezione 7: si guarda soprattutto il comportamento delle specie nella stanza, non il codice.

## Obiettivi

Alla fine della giornata:

- la flotta intera vive nella stessa stanza per almeno un'ora, con tutte le specie, senza errori di convivenza;
- persone che non conoscono il progetto curano le creature e le suonano nell'unisono;
- ogni gruppo presenta la propria specie e riceve una critica sui criteri del corso;
- la classe sa dire cosa ha funzionato nel sistema comune (piattaforma, design system, protocollo) e cosa cambierebbe;
- il docente ha raccolto la consegna finale di ogni gruppo e le osservazioni per la valutazione.

## Materiali da preparare

- **Stanza** prenotata dalla mattina presto: tavoli per 4 o 5 postazioni di specie, una zona centrale libera, prese elettriche vicine a ogni postazione.
- **Alimentazione**: un caricatore USB per dispositivo, con cavi lunghi abbastanza da lasciare la creatura in mano. Con dieci creature accese tutto il giorno la batteria non basta (§12).
- Tutti i dispositivi, le due **riserve** con la specie neutra e i binari finali di ogni specie su una chiavetta e su un portatile, pronti da ricaricare.
- **Osservatore** acceso e in registrazione per tutta la giornata. Se c'è la visualizzazione per la stanza (decisione aperta 6), un proiettore o uno schermo.
- **Griglia di valutazione** stampata, una per gruppo (modello in fondo a questo piano), e la scheda di osservazione della lezione 11.
- Un **cartellino per postazione**, stampato su carta, con il nome della specie e i nove gesti comuni. Lo schermo resta senza testo (regola 2); il significato dei gesti non si scrive, perché scoprirlo è parte del gioco.
- Un telefono su treppiede per la ripresa della stanza, e un registratore audio al centro per le sessioni di unisono.
- Fogli A1 e pennarelli per la retrospettiva.

---

## Parte frontale · 2 ore · allestimento e ultima prova

La mattina è di tutta la classe insieme. Non ci sono contenuti nuovi: la stanza si prepara come un'installazione.

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | Allestimento della stanza | 40' |
| 0:40 | Binari finali e controllo di flotta | 25' |
| 1:05 | Ultima prova di flotta | 35' |
| 1:40 | Ruoli e scaletta del pomeriggio | 20' |

### 0:00 · Allestimento della stanza (40')

- **La disposizione.** Una postazione per specie, con i due individui. Le distanze contano: la vicinanza radio divide i vicini in tre fasce (oltre −55 dBm vicino, fino a −75 medio, sotto lontano, §7.2), e una specie che vuole pochi vicini non va messa al centro. Ogni gruppo dice dove vuole stare e perché; il docente compone la pianta.
- **Le altezze.** Le creature stanno all'altezza di una mano, non sul pavimento né dietro un vetro. Chi entra deve poterle prendere.
- **Il suono.** L'altoparlante da 1 W si sente a pochi metri. Si prova una nota di ogni specie dal punto più lontano della stanza: se non si sente, si stringe la disposizione.
- **La luce.** Le specie che usano il buio o il "coprire" verificano i conteggi del sensore nella postazione vera, con le soglie decise alla lezione 3. Una finestra o un faretto possono cambiare tutto.
- **Cavi e alimentazione**: nessun cavo attraversa i passaggi, ogni creatura si può sollevare senza staccarsi.

### 0:40 · Binari finali e controllo di flotta (25')

1. Ogni gruppo carica il binario finale su entrambi i dispositivi, circa un minuto ciascuno (§11.3).
2. Con il comando `info`, per ogni dispositivo: id, nome, specie, MAC, versione del firmware e del protocollo. Il docente confronta con la tabella di flotta (`flotta.h`): due dispositivi con lo stesso id rendono ambigue le elezioni dell'unisono.
3. Parametri al valore di presentazione e salvati con `save`. Il fattore tempo uguale per tutta la flotta, come deciso alla lezione 11.
4. Ora del giorno impostata con `ora AAAA-MM-GG HH:MM` su tutti i dispositivi: le specie che cambiano con la fascia del giorno devono sapere che è giorno.
5. Le riserve accese con la specie neutra, in un cassetto, pronte.

### 1:05 · Ultima prova di flotta (35')

Tutte le specie accese insieme, nella disposizione finale, con l'osservatore in registrazione.

- **10 minuti di vita**: nessuno tocca le creature. Si controlla con l'osservatore che i messaggi stiano sotto il limite di 5 al secondo per creatura e che nessuno risponda in automatico (§7.3).
- **5 minuti di cura**: ogni gruppo cura una specie che non è la sua, senza istruzioni. È l'ultima prova di leggibilità.
- **Un unisono completo**, 180 s, avviato con l'innesco previsto per il pubblico. Si ascolta dal centro della stanza. Si controlla l'uscita e i 30 s di riposo.
- **10 minuti per le correzioni d'urgenza**: solo parametri con `set` e `save`. Da qui in poi non si ricompila.

### 1:40 · Ruoli e scaletta del pomeriggio (20')

- **La scaletta** del pomeriggio, con gli orari, appesa alla porta.
- **I ruoli di ogni gruppo** durante la stanza aperta, a turno: un **mediatore** alla postazione (risponde solo se interrogato, non spiega le specie), un **osservatore** con la scheda della lezione 11, uno alle **riprese** per il video di classe. Nessuno resta alla propria postazione per tutto il tempo: ogni gruppo deve anche vedere le altre specie con gli occhi del pubblico.
- **La frase d'ingresso** per il pubblico, la stessa usata alla lezione 11 o la sua versione corretta.
- **Cosa fare se qualcosa si rompe**: dispositivo spento o bloccato, si sostituisce con una riserva e il binario della specie; una specie che disturba tutta la stanza si spegne e si annota, non si corregge davanti al pubblico.

---

## Lavoro di gruppo · 5 ore con il pranzo · presentazione e critica

| Ora | Blocco | Durata |
| --- | --- | --- |
| 0:00 | A · Presentazione delle specie | 60' |
| 1:00 | B · La stanza aperta | 60' |
| 2:00 | pranzo | 60' |
| 3:00 | C · Critica collettiva | 75' |
| 4:15 | D · Retrospettiva | 30' |
| 4:45 | E · Chiusura e consegna finale | 15' |

L'orario vale per entrambe le varianti, interna e aperta al pubblico (vedi Punti da decidere). Cambia chi è nella stanza durante i blocchi A e B.

### A · Presentazione delle specie (60')

Ogni gruppo presenta la propria specie alla sua postazione. Con 5 gruppi: **8 minuti di presentazione e 4 di domande**. Con 4 gruppi: 10 e 5.

Cosa si presenta, nell'ordine della scheda di specie:

1. Il **carattere** in una frase e la creatura accesa in mano, senza spiegare ancora cosa prova.
2. Il **significato di tre dei nove gesti**, mostrato sul dispositivo. Gli altri sei restano da scoprire.
3. Lo **stato proprio**: quando scatta e come lo si riconosce. Se non può scattare a comando, si mostra con `stato` forzato e lo si dice.
4. La **parte di unisono**: ruolo, pattern, come entra e come esce, cosa fa in mano.
5. Il **grafico di un'ora di vita** da seriale, stampato o proiettato.

Il video di un minuto non si proietta qui: si guarda durante la critica.

- **Variante interna**: ascoltano la classe, il docente e gli eventuali docenti invitati. Si gira con il gruppo da una postazione all'altra.
- **Variante aperta**: il pubblico segue il giro. La presentazione si accorcia a 6 minuti e le domande restano aperte a tutti; il grafico di vita e lo stato proprio forzato si mostrano solo se qualcuno chiede.

### B · La stanza aperta (60')

La flotta vive e le persone la curano e la suonano. Nessuna spiegazione oltre la frase d'ingresso e il cartellino dei nove gesti.

- **Unisoni**: almeno tre sessioni nell'ora, una ogni 15–20 minuti, avviate dal pubblico se l'innesco comune lo permette, altrimenti dal mediatore di turno. Tra una sessione e l'altra le creature tornano alla loro vita, ed è lì che si vede la cura.
- **Osservazione**: gli osservatori di turno compilano la scheda; il docente gira con la griglia di valutazione e annota per ogni specie, sui criteri.
- **Riprese**: la ripresa fissa della stanza resta accesa; il registratore audio al centro registra ogni sessione di unisono.
- **Variante interna**: il pubblico sono gli studenti stessi, a turno, sulle specie degli altri, più gli esterni invitati, anche gli stessi della lezione 11.
- **Variante aperta**: il pubblico entra ed esce liberamente. Il blocco si può allungare dentro la pausa pranzo, a turni, se l'affluenza lo richiede; in quel caso la critica comincia comunque alle 3:00.

### C · Critica collettiva (75')

A porte chiuse in entrambe le varianti. Con 5 gruppi, **15 minuti per gruppo**; con 4 gruppi, 18 minuti e 3 minuti di margine.

Per ogni gruppo:

1. Il **video di un minuto** (2').
2. **Cosa si è visto nella stanza** (5'): gli osservatori degli altri gruppi leggono dalle schede cosa ha fatto il pubblico con quella specie. Prima le osservazioni, poi i giudizi.
3. **Critica sui criteri** (6'): il docente e la classe, un criterio alla volta.
4. **Risposta del gruppo** (2'): cosa conferma, cosa non si aspettava.

I criteri sono quelli del programma. I pesi sono ancora una proposta e non si usano durante la critica.

| Criterio | Cosa si guarda nella stanza |
| --- | --- |
| Carattere e leggibilità | si capisce cosa prova la creatura senza volto né testo; la grammatica dei bisogni è rispettata; il sonno è riconoscibile |
| Interazione | i gesti hanno significati coerenti e vengono scoperti con piacere da chi non conosce la specie |
| Comportamento nel tempo | la creatura cambia anche quando nessuno la tocca; il grafico di un'ora di vita lo mostra |
| Vita sociale e unisono | convive con le altre specie senza intasare la radio; la parte di unisono si riconosce nel coro e si lascia suonare |
| Involucro e documentazione | involucro rigido e conforme alla regola 10; scheda di specie chiara |

### D · Retrospettiva (30')

Tutta la classe, sui fogli A1. Non si parla delle singole specie ma del sistema comune.

- **Cosa ha tenuto insieme la flotta** (10'): quali regole della piattaforma e del design system hanno funzionato; quali sono state un ostacolo.
- **Cosa cambiereste** (10'): una proposta per gruppo sul documento di progetto. Le decisioni aperte del §12.1 si rileggono una per una: quali ha chiuso il corso, quali restano aperte.
- **Cosa portate via** (10'): una frase per studente, scritta, su cosa vuol dire progettare un comportamento invece di una schermata.

### E · Chiusura e consegna finale (15')

- Ogni gruppo consegna la cartella finale e firma il registro di riconsegna dei dispositivi, oppure lascia i dispositivi in carica con la specie caricata, secondo quanto deciso.
- Si spengono le creature. Allo spegnimento ognuna salva stato e orario: alla riaccensione l'RTC conterà il tempo passato, al massimo 8 ore.

---

## Consegna

**Consegna finale**, in un'unica cartella di gruppo (vedi Punti da decidere per il termine):

1. **Specie funzionante su due dispositivi**, con `config.h` e `creatura.h`.
2. **Involucro montato**, conforme alla regola 10.
3. **Scheda di specie**, al massimo 2 pagine: carattere, significato dei nove gesti, messaggi propri (codici da 100 a 199), parte di unisono.
4. **Video di un minuto** della specie in vita.
5. **Un'ora di vita registrata da seriale**, in forma di grafico: le quattro linee dei bisogni e la striscia degli stati, con lo stato proprio e le sessioni di unisono riconoscibili.

L'ora di vita va registrata **prima** della lezione: la giornata di oggi non ha un'ora libera in cui una creatura possa vivere da sola. Chi vuole, può aggiungere il grafico registrato durante la stanza aperta.

### Modello della griglia di valutazione

| Criterio | Cosa si è visto nella stanza | Cosa mostrano scheda, grafico e video | Note per il voto |
| --- | --- | --- | --- |
| Carattere e leggibilità | | | |
| Interazione | | | |
| Comportamento nel tempo | | | |
| Vita sociale e unisono | | | |
| Involucro e documentazione | | | |

In fondo: specie, gruppo, componenti, eventuali problemi tecnici durante la giornata e come sono stati gestiti. Un problema della piattaforma non pesa sulla specie.

## Punti da decidere

- [ ] **Presentazione interna o aperta al pubblico** (programma, Punti da decidere). Il piano tiene lo stesso orario per le due varianti; con il pubblico aperto servono un orario annunciato, una persona all'ingresso e forse un'ora di stanza aperta in più, a turni durante il pranzo.
- [ ] **Pesi della valutazione** e modalità d'esame previste dall'ordinamento ISIA. I pesi del programma sono una proposta; finché non sono fissati la griglia raccoglie le osservazioni per criterio senza punteggi.
- [ ] **Termine della consegna finale**: alla fine della giornata, oppure una settimana dopo per scheda, video e grafico. Il video di un minuto si gira meglio durante la stanza aperta, che però è il giorno stesso.
- [ ] **Innesco per il pubblico** (decisione 4): se non c'è un gesto comune, gli unisoni del blocco B li avvia il mediatore.
- [ ] **Fattore tempo** durante la stanza aperta, uguale per tutta la flotta.
- [ ] **Una creatura che muore davanti al pubblico**: lasciarla morire e rinascere con la pressione lunga di 5 s, come previsto, oppure tenere il fattore tempo basso perché non succeda.
- [ ] **Destino dei dispositivi dopo il corso**: riconsegna, prestito ai gruppi, o una flotta che resta accesa in un luogo dell'ISIA.
- [ ] **Visualizzazione per la stanza** (decisione 6): se l'osservatore alimenta uno schermo durante la stanza aperta. Richiede Wi-Fi o un cavo e resta fuori dalla piattaforma.
- [ ] **Piattaforma e osservatore**: nel repository non ci sono ancora il codice della piattaforma né il firmware dell'osservatore. Il piano presuppone entrambi.
