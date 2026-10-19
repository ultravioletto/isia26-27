# Creature in rete

Tamagotchi avanzato: progetto del sistema. Bozza 0.1, ottobre 2026. Ultravioletto.

Creature da curare, con un corpo che sente, un'espressione senza volto, una vita sociale via radio e uno stato di grazia collettivo in cui diventano uno strumento musicale.

Questa è la versione testuale del documento di progetto. I dodici schemi (panoramica, livelli software, ciclo di esecuzione, macchina a stati, catena dei sensi, grammatica dei bisogni, rete e vicini, pacchetti, orologio condiviso, sessione di unisono, strumento, tavolozza) sono nel PDF `creature-in-rete-progetto.pdf`, nella stessa cartella. Il documento non contiene codice: è la specifica da cui il codice si scrive. Le cifre sono valori di partenza, raccolti nell'appendice A, tutte regolabili senza ricompilare.

Indice: 1 In breve, 2 Principi, 3 Architettura, 4 La creatura, 5 Sensi, 6 Espressione, 7 Rete, 8 Unisono, 9 Le specie, 10 Design system, 11 Strumenti di lavoro, 12 Rischi e punti aperti, Appendice A Parametri, Appendice B Pacchetti.

---

## 1. In breve

**Creature in rete** è un sistema di piccole creature elettroniche da curare. Ogni creatura vive in un M5Stack Core e ha quattro cose che un Tamagotchi non ha mai avuto: un corpo che sente come viene tenuta, un modo di esprimersi senza volto e senza parole, una vita sociale via radio con le altre creature nella stanza, e uno stato di grazia collettivo, l'**unisono**, in cui le differenze individuali si sospendono e la flotta intera diventa uno strumento musicale che suona da solo e può essere suonato.

Il sistema è pensato per una flotta di circa dieci dispositivi, divisi in quattro o cinque **specie**. Tutte le creature condividono la stessa **piattaforma**: stessi bisogni, stessi stati, stessi sensi, stesso protocollo radio, stesso design system. Ogni specie definisce il carattere, l'espressione, uno **stato proprio** e la propria parte nell'unisono, in due soli file.

**Corpo.** IMU, microfono, luce, touch. La creatura sa se è ferma, in mano, in tasca, al buio, se la stai accarezzando o scuotendo. Reagisce a come viene tenuta, non a cosa viene premuto.

**Cura.** Quattro bisogni che decadono nel tempo e una salute nascosta. La creatura cambia anche quando nessuno la tocca: vive di attenzione e si ammala di trascuratezza.

**Società.** Le creature si annunciano via radio, si riconoscono, si contagiano, si chiamano. Nessun router, nessuna configurazione, nessun telefono: solo la stanza.

**Grazia.** L'unisono. Un orologio condiviso, una scala comune, una parte per specie. Le creature suonano insieme da sole e, prese in mano, si lasciano suonare.

Questo documento descrive il sistema in modo che si possa costruire: architettura, modello della creatura, sensi, espressione, rete, unisono, specie, design system, strumenti di lavoro, rischi. Non contiene codice: è la specifica da cui il codice si scrive. Le cifre indicate sono valori di partenza, raccolti nell'appendice A, e sono tutte regolabili senza ricompilare.

***Figura 1.** Panoramica: una decina di creature in una stanza, divise in specie. Ognuna si annuncia via radio, ascolta le altre e, quando parte un unisono, si allinea a un orologio condiviso. (schema nel PDF)*

## 2. Principi di progetto

Sei regole decidono tutto il resto. Quando una scelta tecnica è in dubbio, si risolve con queste.

1.  **Il comportamento è nel tempo.** Una creatura non reagisce soltanto: cambia anche quando nessuno la tocca. Ogni bisogno ha una velocità di decadimento, ogni stato ha una durata, ogni cura ha un effetto che si consuma. La domanda di progetto non è "cosa succede se premo", ma "cosa succede tra un'ora".
2.  **Nessun volto.** Niente occhi, bocche, emoji, pittogrammi, testo. L'espressione passa per forma, movimento, colore, luce, suono, vibrazione. Il vincolo obbliga a progettare una grammatica invece di disegnare un'emozione, e rende la flotta leggibile come un insieme.
3.  **La radio è un linguaggio, non un cavo.** Le creature non si scambiano comandi: emettono segnali che chi ascolta interpreta a modo suo. Un messaggio è un verso, non un'istruzione. Nessuna creatura può obbligare un'altra a fare qualcosa.
4.  **Autonomia e collaborazione.** Nell'unisono la creatura suona da sola e, quando viene presa in mano, accetta una mano. Chi la tiene non la sostituisce: partecipa. Quanto la creatura si lascia guidare è un tratto di carattere.
5.  **Una piattaforma, molte specie.** Tutto ciò che è comune (bisogni, stati, sensi, rete, orologio, strumenti) sta nella piattaforma e non si modifica. Tutto ciò che è carattere sta nella specie, in due file. Lo stato proprio è il caso limite: la piattaforma ne fissa la cornice, la specie ne decide il contenuto. Le specie devono convivere anche se non si conoscono.
6.  **Attrito zero.** Ambiente, librerie a versione fissa, parametri modificabili al volo, debug a schermo e su seriale: tutto ciò che non è progettazione del comportamento è già pronto. Il tempo si spende sul comportamento, non sulla compilazione.

> **Decisione.** Toolchain Arduino (core ESP32 a versione fissa) con M5Unified, per la portabilità tra Core2 e CoreS3 e per lavorare in un ambiente conosciuto. Ogni specie è un binario separato, compilato dalla stessa piattaforma con i suoi due file.

## 3. Architettura

### 3.1 Hardware

Il sistema gira su M5Stack Core2 o CoreS3. Hanno lo stesso formato, lo stesso schermo e la stessa libreria, ma sensi diversi. La piattaforma espone le capacità disponibili e sostituisce ciò che manca, così la stessa specie gira su entrambi.

|                   | Core2 (v1.3)                                     | CoreS3                                                         | Uso nel sistema                                                    |
|-------------------|--------------------------------------------------|----------------------------------------------------------------|--------------------------------------------------------------------|
| Processore        | ESP32, 2 core a 240 MHz, 16 MB flash, 8 MB PSRAM | ESP32-S3, 2 core a 240 MHz, 16 MB flash, 8 MB PSRAM            | logica, grafica, radio                                             |
| Schermo           | 2", 320 × 240, touch capacitivo                  | 2", 320 × 240, touch capacitivo                                | espressione, gesti touch                                           |
| IMU               | 6 assi                                           | 6 assi più magnetometro                                        | scossa, inclinazione, capovolto, in mano, in tasca                 |
| Microfono         | 1 PDM                                            | 2 PDM                                                          | rumore, voce vicina, colpo sonoro                                  |
| Luce e prossimità | assenti                                          | luce ambiente e prossimità                                     | buio, coperto, mano vicina                                         |
| Altoparlante      | 1 W                                              | 1 W                                                            | suono, voce nell'unisono                                           |
| Vibrazione        | motore                                           | assente                                                        | feedback tattile; sul CoreS3 sostituito da un suono sordo di 30 ms |
| Bottoni           | 3 zone touch sotto lo schermo                    | emulati dalla piattaforma sul bordo inferiore dello schermo    | debug, rinascita                                                   |
| RTC               | sì                                               | sì                                                             | tempo trascorso da spenta, ore del giorno                          |
| Batteria          | 390 mAh                                          | 500 mAh                                                        | qualche ora a schermo acceso; di più nel sonno                     |
| Radio             | Wi-Fi 2,4 GHz, ESP-NOW                           | Wi-Fi 2,4 GHz, ESP-NOW                                         | rete tra creature; i due chip si parlano senza problemi            |
| Listino           | 42,90 \$                                         | 59,90 \$ (SE 38,90 \$; Lite 44,90 \$, con batteria da 200 mAh) |                                                                    |

> **Decisione aperta.** Un solo modello per tutta la flotta è preferibile, per parità di sensi tra le specie. Il CoreS3 SE non è adatto: senza IMU la creatura non ha corpo. Del CoreS3 Lite va verificato cosa conserva. Se la flotta è mista, le specie progettano solo sui sensi comuni (IMU, microfono, touch) e usano luce e vibrazione come bonus.

### 3.2 Livelli software

Quattro livelli, con una linea netta di responsabilità: sotto la linea nessuno modifica niente, sopra la linea ogni specie è libera.

***Figura 2.** I livelli. La specie vede la piattaforma solo attraverso funzioni leggibili (eventi, bisogni, uscite, vicini, messaggi, unisono) e non tocca mai l'hardware direttamente. (schema nel PDF)*

### 3.3 Ciclo di esecuzione

Un solo ciclo, senza attese bloccanti. Ogni modulo lavora alla propria frequenza contando il tempo, così il disegno non rallenta la rete e la rete non rallenta i sensori.

***Figura 3.** Il ciclo principale. La specie riceve eventi già filtrati e bisogni già aggiornati; scrive solo reazioni ed espressione. (schema nel PDF)*

### 3.4 Struttura del progetto

| File          | Contenuto                                                                            | Chi lo modifica        |
|---------------|--------------------------------------------------------------------------------------|------------------------|
| Creatura.ino  | avvio, ciclo principale, cablaggio dei moduli                                        | piattaforma            |
| nucleo.h      | bisogni, salute, stati e transizioni, cornice dello stato proprio, decadimento, tempo spento, memoria persistente | piattaforma            |
| sensi.h       | filtri, riconoscimento di eventi e gesti, contesto (tavolo, mano, tasca)             | piattaforma            |
| espressione.h | canali di uscita, intensità, convenzione del sonno, schermo di debug, tabelle d'onda | piattaforma            |
| rete.h        | ESP-NOW, annuncio, tabella dei vicini, messaggi, formato dei pacchetti               | piattaforma            |
| unisono.h     | sessione, orologio condiviso, strumento, voce autonoma, visivo del battito           | piattaforma            |
| strumenti.h   | seriale, comandi, parametri in memoria, informazioni di flotta                       | piattaforma            |
| flotta.h      | tabella indirizzo MAC → id e nome per ogni dispositivo                               | chi gestisce la flotta |
| config.h      | identità della specie, carattere, soglie, durata e intervallo dello stato proprio, palette, timbro, pattern di unisono | la specie              |
| creatura.h    | reazioni agli eventi e ai messaggi, condizioni ed effetti dello stato proprio, disegno, parte di unisono | la specie              |

### 3.5 Identità e individualità

Ogni dispositivo ha un **id** numerico (da 1 a 250) e un nome breve, assegnati dalla tabella di flotta in base all'indirizzo MAC della radio. Lo stesso binario di una specie gira quindi su più dispositivi, ciascuno con la propria identità, e un dispositivo non in tabella si assegna un id dagli ultimi byte del MAC. Il nome serve solo al debug: non compare mai sullo schermo in modalità normale.

Dal MAC deriva anche un **seme individuale**, che fa variare del ±10% i parametri di carattere della specie: due individui della stessa specie si somigliano ma non sono mai identici. Lo stesso seme guida le piccole scelte casuali dell'espressione e della voce nell'unisono.

## 4. La creatura

### 4.1 Bisogni e salute

Quattro bisogni, ciascuno un livello da 0 a 100 in cui 100 è il benessere pieno. Decadono nel tempo e risalgono con le cure. Una quinta variabile, la **salute**, non si esprime direttamente: riassume quanto a lungo i bisogni sono rimasti trascurati, e governa malattia e morte.

| Bisogno    | Quando è basso | Decadimento di base (al minuto)                                                | Cosa lo fa salire                                                                            |
|------------|----------------|--------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------|
| Nutrimento | fame           | 1,5 da sveglia; 0,75 nel sonno                                                 | il gesto di nutrimento: +25, non più di una volta ogni 2 minuti (oltre, la creatura rifiuta) |
| Energia    | stanchezza     | 1,0 da sveglia; 3,0 nel gioco; +5,0 nel sonno                                  | il sonno; la ricarica USB accelera il recupero                                               |
| Umore      | tristezza      | 0,5 di base; 1,0 nella malattia                                                | carezza +5; gioco +10 al minuto; contagio dai vicini di buon umore                           |
| Socialità  | solitudine     | 1,0 senza vicini; +1,5 con almeno un vicino; +2,0 in mano                      | saluto o chiamata ricevuta +3; voce vicina +3 (al massimo una volta al minuto)               |
| Salute     | malattia       | 1,0 per ogni bisogno sotto 15; 5 per ogni spavento oltre il terzo in un minuto | +1,0 quando tutti i bisogni sono sopra 60                                                    |

Tutte le velocità sono moltiplicate per un **fattore tempo** globale: 1 per la vita normale, 5 o 10 per una dimostrazione in cui un ciclo di cura deve compiersi in pochi minuti. Ogni specie moltiplica a sua volta ciascun decadimento per un coefficiente di carattere.

La perdita di salute è tarata perché la trascuratezza pesi senza essere fulminea: con due bisogni sotto 15 la salute passa da 100 a 0 in circa 50 minuti, e dalla malattia (sotto 30) alla morte in circa 15. Con il valore iniziale di 2,0 bastavano 25 minuti, troppo pochi perché chi cura si accorga della malattia e intervenga.

### 4.2 Stati

Lo stato è la lettura sintetica dei bisogni e degli eventi recenti; è ciò che l'espressione mostra e ciò che la rete annuncia. SVEGLIO è il perno: tutti gli altri stati partono da lì e ci tornano.

***Figura 4.** La macchina a stati. Le condizioni indicate sono i valori di partenza; le soglie e i tempi sono parametri di carattere. (schema nel PDF; lo schema non mostra ancora lo stato proprio, collegato solo a SVEGLIO)*

| Stato      | Cosa significa e cosa cambia                                                                                                                                 |
|------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------|
| SVEGLIO    | stato di base: la creatura è presente, i bisogni decadono alla velocità normale, l'espressione è quella della grammatica dei bisogni                         |
| DORME      | energia in salita, nutrimento e socialità a metà velocità; espressione minima e uguale per tutte le specie (vedi 6.4); un risveglio forzato costa 5 di umore |
| AFFAMATO   | il nutrimento è sotto soglia: l'espressione si contrae, l'intensità sale fino a urgente, poi si acquieta per non assillare                                   |
| GIOCOSO    | c'è un'interazione di gioco in corso: umore in salita rapida, energia in discesa rapida; dura finché il gioco continua, più 30 s                             |
| ANNOIATO   | nessuno la tocca da troppo tempo e l'umore è calato: cerca attenzione a bassa intensità e si fa sentire dai vicini                                           |
| PROPRIO    | lo stato del carattere: nome, significato, condizioni, effetti ed espressione li decide la specie, dentro una cornice fissata dalla piattaforma (vedi 4.4)   |
| SPAVENTATO | reazione breve a una scossa forte o a un colpo sonoro: 15 s di espressione urgente, umore a meno 10; gli spaventi ripetuti intaccano la salute               |
| MALATO     | la salute è scesa sotto 30: l'umore decade al doppio, l'espressione perde saturazione e velocità; si guarisce con una cura costante                          |
| MORTO      | salute a zero: la creatura si spegne nell'espressione e nella rete (non si annuncia più); rinasce con una pressione lunga di 5 s, come nuova generazione     |
| UNISONO    | stato di grazia: bisogni congelati, spavento sospeso, espressione sostituita dalla parte musicale; dura fino alla fine della durata concordata, poi si torna allo stato precedente (vedi 8) |

### 4.3 Transizioni

Quando più condizioni valgono nello stesso istante, vince la più urgente, in quest'ordine: MORTO, UNISONO, MALATO, SPAVENTATO, PROPRIO, DORME, AFFAMATO, GIOCOSO, ANNOIATO, SVEGLIO. In UNISONO nessuna soglia viene valutata: la creatura resta in grazia fino alla fine della durata concordata, qualunque cosa succeda ai suoi bisogni. Ogni soglia ha un'isteresi (si entra a un valore e si esce a un altro) per evitare che la creatura oscilli tra due stati.

| Da                               | A                | Condizione                                                     | Effetto e note                                                                                      |
|----------------------------------|------------------|----------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
| SVEGLIO                          | AFFAMATO         | nutrimento sotto 30                                            | torna SVEGLIO sopra 50                                                                              |
| SVEGLIO                          | DORME            | energia sotto 20, oppure buio o capovolto per 5 s              | la specie può consentire anche un gesto proprio di addormentamento                                  |
| DORME                            | SVEGLIO          | energia sopra 80, oppure scossa, colpo sonoro, luce improvvisa | risveglio forzato: umore meno 5                                                                     |
| SVEGLIO                          | GIOCOSO          | umore sopra 70, energia sopra 50 e un gesto di gioco           | la specie definisce quali gesti sono gioco                                                          |
| GIOCOSO                          | SVEGLIO          | 30 s senza gesti di gioco                                      |                                                                                                     |
| SVEGLIO                          | ANNOIATO         | 10 min senza interazione umana e umore sotto 50                | in ANNOIATO la creatura invia un saluto ai vicini ogni minuto                                       |
| ANNOIATO                         | SVEGLIO          | qualsiasi interazione umana                                    |                                                                                                     |
| SVEGLIO                          | PROPRIO          | condizione d'ingresso della specie                             | solo se è passato l'intervallo minimo dall'ultima volta                                             |
| PROPRIO                          | SVEGLIO          | condizione d'uscita della specie, oppure durata massima        | da SVEGLIO, al tick successivo, valgono di nuovo tutte le soglie                                    |
| qualsiasi, tranne MALATO, MORTO e UNISONO | SPAVENTATO | scossa forte o colpo sonoro                                | umore meno 10; se dormiva, prima si sveglia; tre spaventi in un minuto costano 5 di salute ciascuno |
| SPAVENTATO                       | stato precedente | dopo 15 s                                                      |                                                                                                     |
| qualsiasi, tranne MORTO          | MALATO           | salute sotto 30                                                | esce a SVEGLIO con salute sopra 60                                                                  |
| MALATO                           | MORTO            | salute a 0                                                     | i contatori restano in memoria                                                                      |
| MORTO                            | SVEGLIO          | pressione lunga di 5 s                                         | rinascita: generazione più 1, bisogni a 70, salute 100                                              |
| qualsiasi, tranne MORTO          | UNISONO          | innesco proprio o invito accettato (vedi 8.3)                  | entrano anche la creatura che dorme e quella malata; durante l'unisono lo spavento è sospeso        |
| UNISONO                          | stato precedente | fine della durata concordata, l'unico modo di uscire           | i bisogni ripartono da dove erano, i timer dello stato precedente da dove si erano fermati          |

### 4.4 Lo stato proprio

Ogni specie ha uno stato che le altre non hanno: il momento del suo carattere (una muta, un letargo, una festa, una nostalgia). Nella macchina a stati è un posto previsto dalla piattaforma, lo stesso per tutte, che ogni specie riempie a modo suo. Gli stati comuni restano uguali per tutte; lo stato proprio è l'unico che la specie progetta da zero.

| Lo decide la specie                     | Come                                                                                                                                                                           |
|-----------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| nome e significato                      | solo nella documentazione della specie e nel debug: sullo schermo non c'è testo                                                                                                |
| condizione d'ingresso                   | in creatura.h, con tutto ciò che la creatura sa: bisogni, eventi, contesto, ora del giorno, vicini, contatori di vita                                                          |
| condizione d'uscita                     | in creatura.h; può essere anche solo il tempo                                                                                                                                  |
| effetti sui bisogni                     | un coefficiente per ogni decadimento, da 0 a 2 volte la velocità di SVEGLIO; le cure restano quelle della specie                                                                |
| espressione                             | forma, colore, suono, dentro le tre intensità                                                                                                                                  |
| durata massima e intervallo minimo      | in config, dentro i limiti della piattaforma                                                                                                                                   |

| Lo garantisce la piattaforma              | Perché                                                                                                                                                                 |
|-------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| si entra solo da SVEGLIO e si torna a SVEGLIO | SVEGLIO resta il perno: lo stato proprio è un ramo come gli altri, non una scorciatoia tra stati comuni                                                                 |
| durata massima di 10 min                  | nessuno stato proprio diventa uno stato pozzo; la specie può accorciarla, non allungarla                                                                               |
| intervallo minimo di 20 min tra un'uscita e l'ingresso successivo | lo stato resta speciale e non diventa la vita normale della creatura; la specie può allungarlo, non accorciarlo                                                        |
| priorità subito sotto SPAVENTATO          | MORTO, UNISONO, MALATO e SPAVENTATO lo interrompono sempre, quindi non può nascondere una malattia né impedire l'unisono; vince su sonno, fame, gioco e noia, che aspettano la sua fine |
| la salute segue le regole comuni          | lo stato proprio non tocca la salute: se un bisogno scende sotto 15 la salute cala come sempre, e MALATO lo interrompe                                                 |
| non imita il sonno                        | la convenzione del sonno (6.4) resta riconoscibile: uno stato proprio quieto deve distinguersi da una creatura che dorme                                                |
| si annuncia con un codice comune          | gli altri sanno che una vicina è nel suo stato proprio, e di quale specie, anche senza sapere cosa significhi                                                           |
| non sopravvive allo spegnimento           | alla riaccensione la creatura riparte da SVEGLIO; l'intervallo minimo si conta da quel momento                                                                          |

Dallo stato proprio si va in SPAVENTATO, MALATO e UNISONO come da qualsiasi altro stato, e si torna nello stato proprio con i timer dove erano, compreso quello della durata massima. Una specie senza stato proprio lascia vuota la condizione d'ingresso: la creatura non ci entra mai.

### 4.5 Il tempo

Il nucleo esegue un **tick** a 10 Hz, in cui valuta eventi e transizioni, e applica il **decadimento** una volta al secondo, accumulando le frazioni così che una velocità di 1,5 al minuto si applichi davvero come 1,5 al minuto. Il fattore tempo scala tutto, cure comprese, in modo che una dimostrazione accelerata resti coerente.

Il **tempo spento** conta. Allo spegnimento la creatura salva stato e orario; all'accensione l'RTC dice quanto è passato e il nucleo applica il decadimento del sonno per quel tempo, al massimo per 8 ore. Una creatura lasciata in un cassetto per una settimana torna affamata e sola, forse malata, ma mai morta: la morte si guadagna solo con la trascuratezza da accesa, davanti a qualcuno.

### 4.6 Memoria

Ogni 60 secondi e a ogni cambio di stato, la creatura salva in memoria non volatile: bisogni, salute, stato, generazione, parametri modificati, e i **contatori di vita**: carezze ricevute, pasti, spaventi, minuti in compagnia, minuti in mano, minuti in unisono, sessioni di unisono iniziate da lei, volte nello stato proprio. I contatori sopravvivono alla morte e alla rinascita. Le specie possono leggerli per costruire comportamenti a lungo termine: una creatura che è stata accarezzata mille volte può essere diversa da una che non lo è mai stata.

## 5. Sensi

La specie non legge mai dati grezzi: riceve **eventi** (è successo qualcosa) e **misure continue** già filtrate (quanto è inclinata, quanto rumore c'è). Il filtraggio sta nella piattaforma, così tutte le specie sentono allo stesso modo e un gesto ha la stessa definizione su tutta la flotta.

***Figura 5.** La catena dei sensi: ogni evento nasce da una misura filtrata, con isteresi e un tempo minimo, così da non scattare per il rumore del sensore. (schema nel PDF)*

### 5.1 Eventi e misure

| Evento o misura            | Sorgente           | Definizione di partenza                                                                                                              | Note                                                |
|----------------------------|--------------------|--------------------------------------------------------------------------------------------------------------------------------------|-----------------------------------------------------|
| scossa, scossa forte       | IMU                | accelerazione oltre 1,5 g per meno di 300 ms, dopo rimozione della gravità; forte oltre 3 g                                          | eventi; la forte genera spavento                    |
| inclinazione               | IMU                | beccheggio e rollio in gradi, filtrati a 5 Hz                                                                                        | misura continua                                     |
| capovolto                  | IMU                | asse z rivolto in basso per 1 s                                                                                                      | evento con fine                                     |
| fermo                      | IMU                | varianza minima per 30 s                                                                                                             | evento con fine                                     |
| in mano                    | IMU, touch         | micromovimenti continui o contatto touch; esce dopo 3 s di quiete                                                                    | evento con fine; attiva la docilità nell'unisono    |
| tap, doppio tap            | touch              | contatto sotto 200 ms; due contatti entro 400 ms                                                                                     | evento con posizione                                |
| pressione lunga            | touch              | contatto oltre 1 s (3 s apre il debug, 5 s rinasce)                                                                                  | evento                                              |
| carezza                    | touch              | trascinamento oltre 40 px a meno di 150 px al secondo                                                                                | evento con direzione                                |
| rumore                     | microfono          | livello RMS da 0 a 1 su scala logaritmica                                                                                            | misura continua                                     |
| voce vicina                | microfono          | livello tra 0,3 e 0,7 per più di 1 s                                                                                                 | evento con fine                                     |
| colpo sonoro               | microfono          | picco improvviso oltre 0,8                                                                                                           | evento; genera spavento                             |
| buio, coperto, mano vicina | luce, prossimità   | buio: sotto 5 lux per 2 s; coperto: caduta da oltre 50 a meno di 5 lux in meno di 0,5 s; mano vicina: oggetto entro pochi centimetri | solo CoreS3; coperto distingue la mano dal tramonto |
| caricando, ora del giorno  | alimentazione, RTC | USB collegato; fascia del giorno: notte, mattina, giorno, sera                                                                       | l'orario si imposta da seriale                      |

### 5.2 Il vocabolario dei gesti

Nove gesti umani, uguali per tutta la flotta: **scuotere**, **accarezzare**, **toccare**, **tenere premuto**, **capovolgere**, **coprire**, **parlare vicino**, **tenere in mano**, **posare**. La piattaforma li riconosce; la specie decide cosa significano. Per una specie accarezzare è affetto, per un'altra è fastidio; per una il cibo arriva con la pressione lunga, per un'altra con il capovolgimento. Chi impara i nove gesti può interagire con qualsiasi creatura, anche senza sapere come li interpreta: scoprirlo è parte del gioco.

La specie neutra della piattaforma assegna: pressione lunga = nutrire; carezza = affetto; tap ripetuti = gioco; voce vicina = compagnia; scossa = spavento; capovolgere o coprire = dormire; tenere in mano = compagnia lenta.

### 5.3 Contesto

Dalla combinazione dei sensi la piattaforma deriva un **contesto**, sempre disponibile alla specie: **sul tavolo** (ferma, non in mano), **in mano** (micromovimenti o contatto), **in tasca** (buio con movimento ritmico tra 1 e 3 Hz, il passo di chi cammina; sul Core2, senza sensore di luce, il contesto è **in movimento**). Il contesto permette alle creature di cambiare carattere a seconda di dove vivono, senza che la specie debba interpretare i sensori.

## 6. Espressione

La creatura non ha volto. Si esprime con cinque canali e una grammatica comune, così che chi guarda una specie impari a leggere anche le altre.

### 6.1 Canali

| Canale     | Cosa offre alla specie                                                                                                               | Vincoli                                                           |
|------------|--------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------|
| Schermo    | tutta l'area 320 × 240, con primitive: cerchi, poligoni, linee, curve, campi di rumore, maschere; disegno a 30 fotogrammi al secondo | niente testo, niente volti, niente immagini caricate              |
| Colore     | la palette della specie (primario, secondario, accento) più i neutri comuni; fondo e forme                                           | solo colori della tavolozza comune (vedi 10)                      |
| Suono      | una voce: nota della scala comune, timbro della specie, inviluppo, volume                                                            | una nota alla volta; fuori dall'unisono non più del 10% del tempo |
| Vibrazione | impulsi da 30 a 300 ms (Core2); sul CoreS3 diventa un suono sordo di 30 ms                                                           | solo alle intensità normale e urgente                             |
| Luminosità | retroilluminazione da 5% a 100%                                                                                                      | nel sonno non oltre il 10%                                        |

### 6.2 La grammatica dei bisogni

Ogni bisogno ha una **direzione espressiva** fissa per tutta la flotta. La specie sceglie la forma, la grammatica dice come la forma si deforma quando il bisogno cambia. Le quattro direzioni sono indipendenti e si sommano: una creatura affamata ma felice è contratta e satura.

***Figura 6.** Le quattro direzioni espressive. Chi guarda impara una sola grammatica e la ritrova in tutte le specie. (schema nel PDF)*

### 6.3 Intensità

Ogni segnale ha tre intensità. Nessuna creatura può urlare sempre: l'intensità urgente dura al massimo 10 secondi, poi la creatura torna a normale per 60 secondi di raffreddamento, anche se il bisogno resta. È la regola che rende sopportabile una stanza con dieci creature.

| Intensità | Schermo                                                    | Suono                                   | Vibrazione                                | Quando                                                    |
|-----------|------------------------------------------------------------|-----------------------------------------|-------------------------------------------|-----------------------------------------------------------|
| sussurro  | variazioni entro il 20% dell'ampiezza, ritmo fino a 0,5 Hz | nessuno                                 | nessuna                                   | bisogni sopra 50, sonno, contesto in tasca                |
| normale   | ampiezza piena, ritmo fino a 2 Hz                          | una nota di 300 ms al massimo ogni 10 s | un impulso di 100 ms al massimo ogni 10 s | un bisogno tra 30 e 50, interazione in corso              |
| urgente   | ampiezza piena, ritmo fino a 4 Hz, luminosità massima      | fino a tre note consecutive             | impulsi fino a 300 ms                     | un bisogno sotto 30, spavento; dura 10 s poi si raffredda |

### 6.4 Il sonno

Il sonno è l'unica convenzione espressiva obbligatoria, uguale per tutte le specie: chiunque deve riconoscere una creatura che dorme senza conoscerla. Schermo al 10% di luminosità o meno, nessun colore saturo, un respiro lento a 0,2 Hz (un ciclo ogni 5 secondi) della forma di base, nessuna vibrazione, al massimo una nota grave e sommessa ogni 20 secondi. Quando arriva un unisono, anche la creatura che dorme entra in grazia; alla fine della sessione torna a dormire.

### 6.5 Schermo di debug

Il testo esiste solo qui. Una pressione lunga di 3 secondi nell'angolo in basso a destra apre due pagine: la prima con stato, bisogni, salute, generazione, sessione di unisono, vicini (id, specie, stato, vicinanza, ultimo annuncio), batteria, id, nome, MAC, versione del firmware; la seconda con i valori grezzi dei sensori. Si chiude allo stesso modo o da sola dopo 30 secondi. In modalità normale non c'è alcun modo di far comparire testo.

## 7. Rete

### 7.1 Perché ESP-NOW

Le creature si parlano con ESP-NOW, il protocollo di Espressif che usa la radio Wi-Fi senza rete Wi-Fi: niente router, niente indirizzi IP, niente associazione. Ogni dispositivo manda pacchetti all'indirizzo di un altro o in **broadcast** a tutti quelli in ascolto sullo stesso canale, e arrivano in pochi millisecondi. Si accende e funziona: questo è ciò che rende possibile una flotta in qualsiasi stanza, senza configurazione.

I limiti sono di progetto, non di ostacolo: ogni pacchetto porta al massimo 250 byte, quindi si mandano stati e numeri, non immagini; il broadcast non ha conferma di ricezione, quindi tutto ciò che conta si ripete; la portata in interni è di una decina o ventina di metri, quanto basta per una stanza; tutti devono stare sullo stesso canale (il canale 1) e il Wi-Fi vero resta spento. L'intensità del segnale ricevuto (RSSI) dà una stima grossolana di chi è vicino.

> **La metafora.** Un messaggio è un verso: lo emetti senza sapere chi sente, e se qualcuno risponde è lì. Non esiste una funzione di scoperta: ogni creatura annuncia sé stessa, chi la sente la ricorda, chi non si fa sentire sparisce.

### 7.2 Annuncio e vicini

Ogni 2 secondi, con un ritardo casuale fino a 0,5 s per non sovrapporsi, ogni creatura manda in broadcast un **annuncio**: id, specie, stato, bisogni, salute, flag (in unisono, in mano, caricando) e sessione corrente. Chi lo riceve aggiorna la propria **tabella dei vicini**, con l'RSSI lisciato e l'orario dell'ultimo annuncio. Un vicino che tace per 10 secondi viene rimosso. La tabella tiene fino a 16 vicini e la specie la interroga con domande semplici: quanti vicini ci sono, quanti vicino, quanti della mia specie, chi è in unisono, chi è annoiato, chi è nel suo stato proprio e di quale specie.

***Figura 7.** La creatura 7 e ciò che sa della stanza. La vicinanza è una stima: l'RSSI oscilla, per questo viene lisciata e divisa in tre fasce larghe. (schema nel PDF)*

### 7.3 Messaggi

Oltre all'annuncio, una creatura può mandare un **messaggio**: un codice, un valore e un destinatario (uno specifico o tutti). I codici da 1 a 99 sono comuni e hanno un significato condiviso; i codici da 100 a 199 sono liberi, e ogni specie documenta i propri. Un codice sconosciuto viene ignorato senza errori: le specie convivono anche se non si conoscono.

| Codice   | Nome     | Valore                          | Significato comune                                                               |
|----------|----------|---------------------------------|----------------------------------------------------------------------------------|
| 1        | SALUTO   | nessuno                         | sono qui e ti ho sentito; chi lo riceve guadagna socialità                       |
| 2        | UMORE    | umore del mittente, 0..100      | contagio: la specie decide quanto lasciarsi influenzare, pesando sulla vicinanza |
| 3        | REGALO   | quantità di nutrimento          | una creatura ne nutre un'altra; la specie decide se accettare                    |
| 4        | SPAVENTO | intensità 0..100                | la paura si propaga: chi è vicino può spaventarsi a sua volta, attenuato         |
| 5        | CHIAMATA | nessuno; destinatario specifico | ti sto cercando; chi la riceve può rispondere o ignorare                         |
| 6        | RISPOSTA | nessuno; destinatario specifico | ci sono                                                                          |
| 7        | CAREZZA  | intensità 0..100                | affetto a distanza, tra creature legate                                          |
| 100..199 | liberi   | definito dalla specie           | semantica propria, documentata; le altre specie li ignorano                      |

Due regole di convivenza: nessuna creatura invia più di 5 messaggi al secondo, e nessun messaggio ricevuto genera automaticamente un altro messaggio, se non una RISPOSTA a una CHIAMATA. Senza queste regole dieci creature che si rispondono a vicenda intasano la stanza in pochi secondi.

### 7.4 Pacchetti

Tutti i pacchetti condividono un'intestazione di 8 byte e stanno in meno di 24 byte. La versione del protocollo nell'intestazione permette di ignorare dispositivi con firmware non aggiornato invece di interpretarli male. I campi sono elencati nell'appendice B.

***Figura 8.** I pacchetti principali. MESSAGGIO (codice, destinatario, valore, extra) e UNISONO_FINE (sessione, motivo) seguono lo stesso schema; un byte è un quadrato. (schema nel PDF)*

### 7.5 Carico e affidabilità

Dieci creature producono 5 annunci al secondo; in unisono si aggiungono al massimo poche decine di note al secondo. ESP-NOW ne gestisce centinaia. Non c'è conferma di ricezione in broadcast e non serve: l'annuncio si ripete, la sincronizzazione si ripete, una nota persa è una nota persa, come in qualsiasi coro. Nessuna parte del sistema dipende dall'arrivo di un singolo pacchetto.

## 8. Unisono

### 8.1 Lo stato di grazia

L'unisono è uno stato in cui le creature smettono di essere individui e fanno una cosa sola: suonano insieme. Entrando, i **bisogni si congelano**: il decadimento si ferma, i contatori si fermano, lo stato precedente viene messo da parte e ritrovato all'uscita. Una creatura affamata non stona: per il tempo dell'unisono è libera.

Si entra da qualsiasi stato tranne MORTO: anche la creatura che dorme e quella malata, perché con i bisogni congelati la grazia non le fa peggiorare. Durante l'unisono lo **spavento è sospeso**: una scossa forte o un colpo sonoro non spaventano. Si esce in un solo modo: alla **fine della durata concordata**. Non esistono chiusura anticipata, uscita individuale né scioglimento.

Ciò che resta è il **carattere**: il timbro della specie, il ruolo, il pattern ritmico, quanto ascolta i vicini, quanto si lascia guidare dalla mano, la palette. Le differenze nell'unisono sono tutte caratteriali, nessuna è di stato. Questo rende l'unisono un luogo in cui le specie si confrontano alla pari.

> **Punto rimandato.** Se e come la cura influisca sull'unisono è una scelta aperta. L'opzione più semplice, se si vorrà, è agire solo sulla soglia d'ingresso: una creatura trascurata ci mette più tempo a entrare in grazia, ma una volta dentro è uguale alle altre.

La macchina a stati completa dell'unisono (stati, transizioni, eccezioni, recupero e invarianti) è nel documento *Lo stato di grazia*; questo capitolo ne riassume le regole.

### 8.2 L'orologio condiviso

Tutto l'unisono poggia su un solo meccanismo: un **tempo comune**. Chi dirige la sessione manda ogni 2 secondi un pacchetto SYNC con il tempo trascorso dall'inizio, il bpm, la griglia, la scala, la tonica e la durata. Chi lo riceve ricava l'istante d'inizio nel proprio orologio (adesso meno il trascorso, meno qualche millisecondo di latenza radio) e da lì in poi calcola da solo in che **passo** si trova. A 90 bpm con griglia a quarti un passo dura 166,7 ms. Le correzioni successive sono lisciate, al massimo 10 ms al secondo, così nessuno salta; la deriva tra i quarzi è di pochi millisecondi al minuto e la risincronizzazione la annulla. Uno scarto oltre 150 ms (all'ingresso, o tornando da un lungo isolamento) si corregge invece di colpo: un passo può saltare, nessun passo viene ripetuto. Nelle prove con tre CoreS3 (`code/arduino/TestSincronia`) gli schermi lampeggiano entro 0,3 ms l'uno dall'altro.

La scala di partenza è la pentatonica maggiore su do4, su due ottave; il SYNC può cambiarla per una sessione, così una stessa flotta può suonare in tonalità diverse.

***Figura 9.** L'orologio condiviso. Ogni creatura calcola il passo da sola: i pacchetti servono solo a concordare l'inizio e a correggere la deriva. (schema nel PDF)*

### 8.3 La sessione

Una sessione ha un'**origine**, che tiene l'orologio, e dei **partecipanti**. Il protocollo è volutamente piccolo.

- **Innesco.** La specie decide come la propria creatura propone un unisono: con un gesto umano (la specie neutra usa tre scosse in due secondi), per una condizione interna (umore alto e almeno tre vicini), o mai, limitandosi ad accettare. Chi propone diventa origine, sceglie un numero di sessione casuale e manda il primo SYNC, che vale da invito. Se negli ultimi 3 secondi la creatura ha sentito un SYNC, non apre una sessione parallela: entra subito in quella.
- **Ingresso.** Chi riceve un SYNC e decide di entrare alza il flag di unisono nel proprio annuncio, con il numero di sessione, e inizia a inoltrare i SYNC che riceve con hop aumentato di uno, dopo un ritardo casuale tra 50 e 300 ms, al massimo una volta ogni 2 secondi e fino a hop 2. Così l'invito raggiunge anche chi è fuori portata dall'origine. La **prontezza** (da 0 a 10 s di attesa prima di entrare) e il **quorum** (quanti partecipanti servono prima di aggiungersi) sono tratti di carattere. Se alla fine della sessione mancano meno di 10 secondi, l'invito non viene accettato.
- **Continuità.** Chi non riceve SYNC da 3 secondi diventa *orfano*: continua a suonare sul proprio orologio e aspetta. Se l'origine tace per 6 secondi, tra i partecipanti prende l'orologio quello con l'id più basso, con la stessa sessione e lo stesso t0 stimato: la musica non si ferma se chi l'ha iniziata viene spento o portato via. Una creatura isolata a lungo diventa origine di sé stessa, continua da sola e, tornata in portata, si riallinea.
- **Conflitti.** Se due sessioni si incontrano, o due creature tengono l'orologio della stessa sessione, vince l'origine con l'id più basso e l'altra cede. Ogni dispositivo applica la regola da solo e tutti arrivano alla stessa conclusione.
- **Recupero.** Una creatura che si riavvia a sessione in corso ascolta per 6 secondi: se la sessione esiste ancora, rientra subito, senza prontezza né quorum; altrimenti ritrova lo stato di vita salvato.
- **Fine.** La sessione finisce solo alla durata concordata (180 s di partenza; la durata non può essere 0). Ogni dispositivo calcola l'istante di fine dal SYNC, sul proprio orologio, ed esce allo stesso passo degli altri anche senza sentire altro; l'origine manda un FINE solo come conferma. Chi perde l'orologio continua fino alla fine; chi si spegne non esce ma si interrompe, e alla riaccensione vale il recupero. L'uscita è una dissolvenza: la voce sfuma fino alla fine della battuta, al massimo 2 secondi; se le creature escono tutte insieme o una alla volta lo decide la specie, attraverso la durata di questa dissolvenza. Seguono 30 secondi di riposo, in cui non si propone né si accetta una sessione nuova.

***Figura 10.** Una sessione tra tre creature, una delle quali fuori portata dall'origine. Le note, come i SYNC, vengono inoltrate una volta, con hop, così la stanza intera le sente. (schema nel PDF)*

### 8.4 Lo strumento

In unisono la creatura è uno strumento: suona da sola e, presa in mano, si lascia suonare da una persona (vedi 8.5). Lo strumento deve restare semplice, perché deve suonare bene in mano a chiunque, e comune a tutte le specie.

> **Da definire.** Come si suona una creatura in mano durante l'unisono: quali gesti, come le note si allineano alla griglia, come si usa il pacchetto NOTA. La versione precedente (inclinazione per grado e ottava, tap per suonare) è sospesa finché la modalità non viene decisa.

### 8.5 Autonoma e suonata

Lo strumento non tace quando lo si posa. Ogni creatura ha una **voce autonoma**: a ogni passo della griglia decide se suonare, in base a un **pattern di 16 passi** (una battuta di quattro quarti) con una probabilità per passo, e a un **ruolo** che guida la scelta del grado. Pattern e ruolo sono definiti dalla specie, in config, senza scrivere logica.

| Ruolo   | Comportamento della voce autonoma                                                                                           |
|---------|-----------------------------------------------------------------------------------------------------------------------------|
| bordone | una nota lunga e grave sul primo passo ogni due battute, tonica o quinta: il pavimento                                      |
| ritmo   | note brevi sui passi in levare, sempre lo stesso grado: il battito                                                          |
| melodia | note sui passi del pattern, con un cammino casuale di un grado alla volta e tendenza a tornare alla tonica                  |
| eco     | ripete l'ultima nota sentita dai vicini, dopo un ritardo di alcuni passi, più piano; preferisce le note suonate da una mano |

Due tratti regolano il rapporto con le persone. La **docilità** (da 0 a 1) dice quanto la creatura cede alla mano: quando è in mano, la probabilità della voce autonoma si riduce in proporzione e la persona suona; una creatura docile si zittisce e ti lascia suonare, una testarda continua la sua parte e tu ti aggiungi. L'**ascolto** (da 0 a 1) dice con che probabilità, al posto della propria scelta, la creatura imita la nota più recente di un vicino. Quando viene posata, la voce autonoma torna in 2 secondi di dissolvenza. Il flag "in mano" nell'annuncio fa sapere agli altri che una persona sta suonando, e le specie possono reagire (l'eco, per esempio, risponde volentieri a una mano).

### 8.6 Il visivo dell'unisono

> **Da definire.** Il suono è la parte principale; cosa mostra lo schermo durante l'unisono non è ancora deciso. Nel TestSincronia gli schermi lampeggiano a ogni quarto, ma solo come strumento di verifica della sincronia.

## 9. Le specie

Una specie è un carattere installato su due o più dispositivi. Non aggiunge bisogni o sensi e non cambia gli stati comuni: li interpreta. L'unico stato che le appartiene è lo stato proprio (4.4), un posto che la piattaforma prevede per tutte e che ogni specie riempie. Tutto ciò che la definisce sta in `config.h` (valori) e `creatura.h` (reazioni e disegno). La piattaforma garantisce che una specie scritta male non possa rompere la stanza: non può urlare oltre i limiti, non può intasare la radio, non può costringere le altre.

### 9.1 Cosa definisce una specie

| Ambito      | Cosa decide                                                                                                                                                                                                                                                                                                     | Dove             |
|-------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|------------------|
| Identità    | codice di specie, nome, palette di tre colori dalla tavolozza comune                                                                                                                                                                                                                                            | config           |
| Carattere   | coefficienti di decadimento per ogni bisogno; soglie di fame, sonno, noia, spavento; sensibilità a scossa, rumore e luce; socievolezza (numero di vicini preferito, minimo e massimo); contagio (quanto subisce l'umore altrui); curiosità (quanto in fretta si annoia); resilienza (quanto in fretta guarisce) | config           |
| Stato proprio | nome e significato; condizioni d'ingresso e d'uscita; coefficienti di decadimento durante lo stato; espressione; durata massima e intervallo minimo, dentro i limiti della piattaforma (vedi 4.4) | creatura, config |
| Reazioni    | significato dei nove gesti; risposta ai messaggi comuni; messaggi propri (codici da 100 a 199) e quando inviarli; comportamento per contesto (tavolo, mano, tasca)                                                                                                                                              | creatura         |
| Espressione | la forma di base e come la grammatica la deforma; il timbro (famiglia d'onda, inviluppo, vibrato); come rende le tre intensità                                                                                                                                                                                  | creatura, config |
| Unisono     | ruolo, pattern di 16 passi, ottava preferita, docilità, ascolto, prontezza, quorum, innesco, se entra quando dorme, come esce                                                                                                                                                                                   | config           |
| Memoria     | quali contatori di vita usa e per cosa                                                                                                                                                                                                                                                                          | creatura         |

### 9.2 Individualità

Il seme individuale, derivato dal MAC, fa variare del ±10% ogni coefficiente di carattere e guida le scelte casuali della voce e dell'espressione. Due creature della stessa specie hanno lo stesso carattere ma non la stessa vita: una si annoia un po' prima, l'altra dorme un po' di più. Nell'unisono, due individui della stessa specie con lo stesso pattern non suonano mai le stesse note nello stesso ordine.

### 9.3 Regole di convivenza

- Ignorare i messaggi che non si conoscono, senza errori e senza rispondere.
- Non rispondere mai in automatico a un messaggio con un altro messaggio, tranne RISPOSTA a CHIAMATA.
- Rispettare le intensità: la piattaforma le impone, ma una specie che le progetta bene è più leggibile.
- Restare nell'unisono fino alla fine della durata concordata: nessuna specie può farne uscire una creatura prima.
- Accettare un SYNC con scala o tonica diverse dalle proprie preferenze: la sessione comanda.
- Tollerare la flotta mista: usare luce e vibrazione solo come bonus, mai come unico canale di un significato.

### 9.4 Esempi di carattere

Cinque caratteri possibili, per mostrare come i tratti si traducono in parametri. Non sono prescrizioni.

| Carattere     | Idea                                                                                    | Tratti                                                                                                                                  |
|---------------|-----------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| la timida     | sta bene con pochi vicini; si spaventa se la stanza si affolla, si spegne se resta sola | vicini preferiti 1..2; contagio basso; prontezza 8 s; quorum 3; docilità alta; ruolo eco, voce piano                                    |
| la contagiosa | il suo umore si trasmette a chi le sta vicino, nel bene e nel male                      | manda UMORE ogni 30 s; contagio ricevuto basso; ruolo ritmo; ascolto basso; innesco per condizione interna                              |
| la nomade     | vuole essere portata in giro; cambia carattere tra tasca, mano e tavolo                 | socialità decade in fretta sul tavolo e sale in tasca; timbro diverso per contesto; ruolo melodia; docilità media                       |
| la coppia     | due creature legate: una soffre se l'altra è lontana o trascurata                       | CHIAMATA alla gemella ogni minuto; CAREZZA inoltrata; socialità legata alla vicinanza della gemella; stesso pattern sfalsato di 8 passi |
| l'anziana     | ricorda chi l'ha curata, invecchia, cambia abitudini con l'ora del giorno               | usa i contatori di vita; decadimento lento; dorme di sera; ruolo bordone; docilità bassa; prontezza 0 s                                 |

Alcuni tratti non si ottengono con le transizioni comuni, perché nessuna dipende dall'ora del giorno o dai vicini: sono il materiale dello stato proprio. L'anziana che "dorme di sera" può avere uno stato proprio *assopita*, che scatta la sera con l'energia sotto 60; la timida che "si spegne se resta sola" può avere uno stato proprio *sola*, che scatta quando non sente vicini per 5 minuti.

## 10. Design system

Dieci regole tengono insieme le specie. Alcune le impone la piattaforma, le altre sono vincoli di progetto che ogni specie rispetta. Lo scopo è doppio: che la flotta si legga come un insieme, e che chi impara una creatura sappia già qualcosa di tutte le altre.

1.  **Nessun volto.** Niente occhi, bocche, emoji, pittogrammi. L'espressione è forma astratta, movimento, colore, luce, suono, vibrazione.
2.  **Nessun testo** in modalità normale. Il testo esiste solo nello schermo di debug, dietro una pressione lunga di 3 secondi.
3.  **Tavolozza comune** di dodici colori più tre neutri. Ogni specie ne sceglie tre (primario, secondario, accento) e nessuna condivide il primario con un'altra.
4.  **Grammatica dei bisogni** uguale per tutte: nutrimento contrae, energia accelera, umore satura, socialità apre (vedi 6.2).
5.  **Suono comune.** Scala pentatonica maggiore su due ottave, tonica di partenza do4, una voce per creatura, un timbro per specie scelto tra quattro famiglie d'onda con inviluppo proprio.
6.  **Ritmo comune.** 90 bpm di partenza e griglia a quarti nell'unisono; le specie si distinguono per cosa fanno sul battito, non per il tempo.
7.  **Tre intensità** per ogni segnale: sussurro, normale, urgente. L'urgente dura al massimo 10 secondi, poi 60 di raffreddamento. Fuori dall'unisono nessuna creatura suona per più del 10% del tempo.
8.  **Nove gesti comuni**: scuotere, accarezzare, toccare, tenere premuto, capovolgere, coprire, parlare vicino, tenere in mano, posare. Il significato è della specie, la definizione è della flotta.
9.  **Il sonno è riconoscibile** da chiunque: luminosità al 10%, respiro a 0,2 Hz, nessun colore saturo, nessuna vibrazione. È l'unica convenzione espressiva obbligatoria.
10. **Involucro** sulla stessa impronta del Core (54 × 54 mm), materiale libero. Deve lasciare liberi schermo, microfono, altoparlante, USB-C e la finestra del sensore di luce; deve essere rigido, perché l'IMU senta la mano e non il guscio; niente metallo intorno all'antenna, sul bordo superiore.

***Figura 12.** La tavolozza comune: dodici colori distinguibili su uno schermo IPS da due pollici e tre neutri. I valori sono di partenza e vanno provati sui dispositivi reali. (schema nel PDF)*

### 10.1 Suono

| Famiglia d'onda | Carattere                                        | Parametri della specie                                       |
|-----------------|--------------------------------------------------|--------------------------------------------------------------|
| seno            | morbida, rotonda, si perde nel coro              | attacco 5..200 ms, decadimento 50..800 ms, vibrato opzionale |
| triangolo       | dolce ma presente, simile a un flauto            | come sopra                                                   |
| quadra          | netta, da chip, buca il coro                     | come sopra, più larghezza d'impulso                          |
| soffio          | rumore filtrato, percussiva, senza altezza netta | colore del filtro, decadimento                               |

Le tabelle d'onda vengono generate all'avvio dalla piattaforma, a partire dai parametri in config. L'altoparlante da 1 W rende poco sotto il do4: per questo la scala parte da lì, e il bordone usa la quinta più che la tonica grave. Dieci altoparlanti in una stanza sono un piccolo coro, e la leggera imprecisione tra i dispositivi è parte del suono.

## 11. Strumenti di lavoro

La piattaforma include tutto ciò che serve per capire cosa fa una creatura e regolarla senza ricompilare. È ciò che rende sostenibile una flotta di dieci dispositivi e molte iterazioni.

### 11.1 Seriale

A 115200 baud, una volta al secondo, la creatura stampa una riga con: tempo, stato, i quattro bisogni, salute, contesto, numero di vicini, RSSI medio, sessione di unisono, batteria. Il formato è a valori separati da virgola, così una sessione di seriale si incolla in un foglio e diventa un grafico della vita della creatura. Gli eventi (gesti, messaggi, transizioni) si stampano quando avvengono, su righe marcate.

### 11.2 Comandi

| Comando                     | Effetto                                                                                       |
|-----------------------------|-----------------------------------------------------------------------------------------------|
| set nome valore             | cambia un parametro al volo, in RAM (per esempio la velocità di decadimento del nutrimento)   |
| get nome                    | legge un parametro; `get *` li elenca tutti                                                   |
| save, load, fabbrica        | salva i parametri in memoria non volatile, li ricarica, li riporta ai valori della specie     |
| tempo n                     | fattore tempo (1 vita normale, 10 dimostrazione)                                              |
| evento nome                 | inietta un evento (scossa, carezza, buio, voce) senza fare il gesto                           |
| stato NOME                  | forza uno stato per provarne l'espressione                                                    |
| bisogno nome valore         | imposta un bisogno (per esempio nutrimento 10 per vedere la fame)                             |
| msg codice valore dest      | invia un messaggio                                                                            |
| unisono avvia               | apre una sessione da questa creatura, o entra in quella che sente; finisce alla durata        |
| vicini                      | stampa la tabella dei vicini                                                                  |
| ora AAAA-MM-GG HH:MM        | imposta l'RTC                                                                                 |
| info                        | id, nome, specie, MAC, versione del firmware e del protocollo, generazione, contatori di vita |

### 11.3 Flotta

Un binario per specie, caricato via USB (circa un minuto per dispositivo). La tabella di flotta assegna id e nome dal MAC, quindi lo stesso binario va bene per tutti gli individui della specie. Due dispositivi di **riserva** restano caricati con la specie neutra della piattaforma: se una creatura si rompe, la riserva prende il suo posto e, se serve, il suo binario. Il numero di versione del protocollo nell'intestazione dei pacchetti fa sì che un dispositivo con firmware vecchio venga ignorato e non interpretato male.

### 11.4 L'osservatore

Un undicesimo dispositivo (qualsiasi ESP32, anche senza schermo) caricato con un firmware di solo ascolto, che stampa su seriale ogni pacchetto che sente nella stanza: annunci, messaggi, SYNC, note. Collegato a un computer, permette di vedere la vita sociale della flotta in tempo reale e di registrarla. È lo strumento con cui si capisce se le creature si parlano davvero o se si parlano addosso.

## 12. Rischi e punti aperti

| Rischio                          | Mitigazione                                                                                                                           |
|----------------------------------|---------------------------------------------------------------------------------------------------------------------------------------|
| Flotta mista Core2 e CoreS3      | la piattaforma espone le capacità e sostituisce ciò che manca; le specie progettano sui sensi comuni; meglio un solo modello          |
| Altoparlante piccolo             | scala dal do4 in su, timbri con attacco netto, prova in stanza con tutti i dispositivi prima di fissare il volume                     |
| Batteria                         | qualche ora a schermo acceso: luminosità legata all'intensità, sonno a 10%, alimentazione USB nelle esposizioni lunghe                |
| Falsi spaventi dall'IMU          | soglie con isteresi e tempo minimo; il contesto in tasca alza le soglie; taratura con il comando `set`                                |
| Radio affollata (2,4 GHz)        | pacchetti piccoli, ripetizione di tutto ciò che conta, nessuna dipendenza da un singolo pacchetto; scelta del canale in base al luogo |
| Deriva degli orologi             | risincronizzazione ogni 2 s con correzione lisciata; deriva residua di pochi millisecondi, sotto la percezione                        |
| Tempeste di messaggi             | limite di 5 messaggi al secondo e divieto di risposta automatica, imposti dalla piattaforma                                           |
| Compilazioni lente               | parametri modificabili da seriale e salvati in memoria; si ricompila solo per cambiare logica o disegno                               |
| Usura della memoria non volatile | salvataggio ogni 60 s e ai cambi di stato: poche migliaia di scritture al giorno, anni di vita                                        |
| Touch sotto un involucro         | lo schermo deve restare scoperto; i gesti touch si provano con l'involucro montato                                                    |

### 12.1 Decisioni aperte

1.  **Modello della flotta**: Core2 (vibrazione, prezzo) o CoreS3 (luce, prossimità, due microfoni, batteria). Il sistema non dipende dalla scelta.
2.  **Cura e unisono**: se la cura influisca sull'ingresso in grazia, e come. Rimandato.
3.  **Rinascita**: se la generazione debba cambiare qualcosa nel carattere, oltre a contare.
4.  **Innesco dell'unisono**: quanto lasciare alle specie e quanto fissare come gesto comune, perché il pubblico sappia come far partire la grazia.
5.  **Scala e tonica**: se tenerle fisse o permettere sessioni in tonalità diverse; il protocollo lo consente.
6.  **Dashboard**: se l'osservatore debba alimentare una visualizzazione per la stanza; richiede Wi-Fi o un cavo, e resta fuori dalla piattaforma base.
7.  **Suonare nell'unisono**: i gesti con cui una persona suona una creatura in mano (vedi 8.4).
8.  **Visivo dell'unisono**: cosa mostra lo schermo durante la grazia (vedi 8.6).
9.  **Limiti dello stato proprio**: se 10 minuti di durata e 20 di intervallo bastano a ogni specie, da verificare con gli stati propri progettati dai gruppi (vedi 4.4).

Prossimo passo: scrivere la piattaforma a partire da questo documento, nell'ordine nucleo, sensi, espressione, rete, unisono, strumenti, e costruire la specie neutra come prova di tutto il sistema su due dispositivi.

## Appendice A. Parametri di partenza

Valori iniziali della specie neutra. Ogni specie li ridefinisce in config; ciascuno si cambia al volo con `set`.

| Gruppo                | Parametro                                             | Valore                                                  |
|-----------------------|-------------------------------------------------------|---------------------------------------------------------|
| Tempo                 | fattore tempo                                         | 1,0 (dimostrazione 5..10)                               |
|                       | tick del nucleo                                       | 10 Hz                                                   |
|                       | tempo spento conteggiato al massimo                   | 8 h, a ritmo di sonno                                   |
| Decadimento al minuto | nutrimento, sveglia e nel sonno                       | 1,5 e 0,75                                              |
|                       | energia, sveglia, nel gioco, nel sonno                | 1,0; 3,0; +5,0                                          |
|                       | umore, base e nella malattia                          | 0,5 e 1,0                                               |
|                       | socialità, senza vicini, con vicini, in mano          | 1,0; +1,5; +2,0                                         |
| Cure                  | nutrire                                               | +25 nutrimento, non più di una volta ogni 2 min         |
|                       | carezza                                               | +5 umore                                                |
|                       | gioco                                                 | +10 umore al minuto, energia 3,0 al minuto              |
|                       | voce vicina; saluto o chiamata ricevuti               | +3 socialità (al massimo una volta al minuto); +3       |
|                       | spavento; risveglio forzato                           | umore meno 10; meno 5                                   |
| Soglie                | fame, ingresso e uscita                               | 30 e 50                                                 |
|                       | sonno, ingresso e uscita                              | 20 e 80                                                 |
|                       | gioco: umore ed energia minimi                        | 70 e 50                                                 |
|                       | noia: tempo senza cure e umore massimo                | 10 min e 50                                             |
|                       | spavento: scossa, colpo sonoro, durata                | 3 g, 0,8, 15 s                                          |
|                       | malattia, ingresso e uscita; morte                    | 30 e 60; 0                                              |
| Stato proprio         | durata massima; intervallo minimo                     | 10 min (la specie può accorciarla); 20 min (può allungarlo) |
|                       | coefficienti di decadimento durante lo stato          | 1,0 (da 0 a 2)                                          |
| Salute al minuto      | per ogni bisogno sotto 15                             | meno 1,0                                                |
|                       | per ogni spavento oltre il terzo in un minuto         | meno 5                                                  |
|                       | con tutti i bisogni sopra 60                          | +1,0                                                    |
| Sensi                 | scossa, scossa forte, finestra                        | 1,5 g; 3 g; 300 ms                                      |
|                       | capovolto; fermo; fine di in mano                     | 1 s; 30 s; 3 s                                          |
|                       | tap; doppio tap; pressione lunga, debug, rinascita    | 200 ms; 400 ms; 1 s, 3 s, 5 s                           |
|                       | carezza: spostamento e velocità massima               | 40 px; 150 px al secondo                                |
|                       | voce vicina; colpo sonoro                             | 0,3..0,7 per 1 s; oltre 0,8                             |
|                       | buio; coperto                                         | sotto 5 lux per 2 s; da oltre 50 a sotto 5 lux in 0,5 s |
|                       | in tasca: frequenza del passo                         | 1..3 Hz, con buio                                       |
| Espressione           | urgente: durata e raffreddamento                      | 10 s e 60 s                                             |
|                       | sonno: luminosità e respiro                           | 10% e 0,2 Hz                                            |
|                       | suono fuori dall'unisono                              | al massimo il 10% del tempo                             |
|                       | fotogrammi al secondo                                 | 30                                                      |
| Rete                  | canale; annuncio; scadenza dei vicini; vicini massimi | 1; 2 s più fino a 0,5 s casuali; 10 s; 16               |
|                       | vicinanza: vicino, medio, lontano                     | oltre -55 dBm; fino a -75; sotto -75                    |
|                       | lisciatura dell'RSSI                                  | 0,3                                                     |
|                       | messaggi al secondo per creatura                      | 5                                                       |
| Unisono               | bpm; griglia; scala; tonica; estensione               | 90; quarti; pentatonica maggiore; do4; 2 ottave         |
|                       | SYNC; hop massimo; ritardo di inoltro                 | ogni 2 s; 2; 50..300 ms                                 |
|                       | orfano; elezione dopo silenzio; durata                | 3 s; 6 s; 180 s (mai 0)                                 |
|                       | prontezza; quorum; riposo tra sessioni                | 0..10 s; 0..4; 30 s                                     |
|                       | durata residua minima per entrare; recupero           | 10 s; 6 s dopo il riavvio                               |
|                       | dissolvenza d'uscita                                  | fine della battuta, al massimo 2 s                      |
|                       | dissolvenza tra mano e voce autonoma                  | 2 s                                                     |
|                       | correzione massima dell'orologio; soglia di salto     | 10 ms al secondo; 150 ms                                |
| Memoria               | salvataggio                                           | ogni 60 s e a ogni cambio di stato                      |
| Individualità         | variazione dal seme                                   | ±10% sui coefficienti di carattere                      |

## Appendice B. Pacchetti

Tutti i valori interi sono senza segno salvo indicazione; i campi a più byte sono in ordine little-endian, quello nativo dell'ESP32. Ogni pacchetto inizia con l'intestazione comune.

Un pacchetto inoltrato conserva id e seq del mittente originale nell'intestazione; chi lo riceve scarta le copie già viste. Il flag "in mano" nell'annuncio e la provenienza della nota permettono alle specie di distinguere ciò che suona una persona da ciò che suona una creatura.

| Pacchetto         | Campo                                         | Byte | Significato                                                                                      |
|-------------------|-----------------------------------------------|------|--------------------------------------------------------------------------------------------------|
| Intestazione      | magic                                         | 2    | costante che identifica il sistema; pacchetti diversi vengono ignorati                           |
|                   | versione                                      | 1    | versione del protocollo; se diversa, il pacchetto viene ignorato                                 |
|                   | tipo                                          | 1    | 1 ANNUNCIO, 2 MESSAGGIO, 3 UNISONO_SYNC, 4 UNISONO_NOTA, 5 UNISONO_FINE                          |
|                   | specie                                        | 1    | codice della specie del mittente                                                                 |
|                   | id                                            | 1    | id del mittente, 1..250                                                                          |
|                   | seq                                           | 2    | contatore progressivo del mittente, per scartare i duplicati inoltrati                           |
| ANNUNCIO (10)     | stato                                         | 1    | codice dello stato; lo stato proprio ha un codice comune, e la specie nell'intestazione dice di chi è |
|                   | nutrimento, energia, umore, socialità, salute | 5    | 0..100                                                                                           |
|                   | flag                                          | 1    | bit 0 in unisono, bit 1 in mano, bit 2 caricando, bit 3 dorme                                    |
|                   | generazione                                   | 1    | quante rinascite                                                                                 |
|                   | sessione                                      | 2    | sessione di unisono corrente, 0 se nessuna                                                       |
| MESSAGGIO (8)     | codice                                        | 1    | 1..99 comuni, 100..199 della specie                                                              |
|                   | destinatario                                  | 1    | id, oppure 255 per tutti                                                                         |
|                   | valore                                        | 2    | intero con segno                                                                                 |
|                   | extra                                         | 4    | libero, definito dal codice                                                                      |
| UNISONO_SYNC (14) | sessione                                      | 2    | numero casuale scelto dall'origine                                                               |
|                   | origine                                       | 1    | id di chi tiene l'orologio                                                                       |
|                   | hop                                           | 1    | 0 dall'origine, 1 o 2 se inoltrato                                                               |
|                   | bpm, griglia, scala, tonica                   | 4    | tempo, suddivisioni per battito, codice della scala, nota MIDI della tonica                      |
|                   | trascorso                                     | 4    | millisecondi dall'inizio della sessione, nell'orologio dell'origine                              |
|                   | durata                                        | 2    | secondi totali della sessione, mai 0: la sessione finisce solo alla durata                       |
| UNISONO_NOTA (8)  | sessione, passo                               | 4    | sessione; indice del passo sulla griglia dall'inizio della sessione                              |
|                   | grado, ottava, intensità, durata              | 4    | 0..4 nella scala; 0..1; 0..100; in passi                                                         |
| UNISONO_FINE (4)  | sessione, motivo, riservato                   | 4    | sessione; motivo: 1 durata raggiunta (l'unico usato, il FINE è una conferma); un byte riservato  |
