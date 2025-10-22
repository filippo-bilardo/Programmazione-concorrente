### 1.1 Concetti fondamentali sui processi  

Un **processo** è un programma in esecuzione all'interno di un sistema operativo. Esso rappresenta l'unità base di esecuzione in un sistema multitasking, che consente di eseguire più programmi contemporaneamente. Ogni processo è un'entità autonoma, che include il codice del programma, i dati, le risorse necessarie e lo stato corrente della sua esecuzione.

#### Caratteristiche principali di un processo  
1. **Stato del processo**  
   - Ogni processo può trovarsi in uno dei seguenti stati:
     - **Nuovo**: il processo è stato creato ma non è ancora in esecuzione.
     - **Pronto**: il processo è pronto per essere eseguito, ma sta aspettando che la CPU sia disponibile.
     - **Esecuzione**: il processo è attualmente in esecuzione sulla CPU.
     - **Bloccato**: il processo è in attesa di un evento, come l'I/O.
     - **Terminato**: il processo ha completato la sua esecuzione.

2. **Identificatore univoco**  
   Ogni processo è identificato da un **PID (Process ID)**, un numero univoco assegnato dal sistema operativo al momento della creazione del processo.

3. **Struttura del processo**  
   - **Segmento di codice**: contiene il codice del programma.
   - **Segmento di dati**: memorizza le variabili globali e statiche.
   - **Stack**: utilizzato per le chiamate di funzione e per la memorizzazione delle variabili locali.
   - **Heap**: spazio dinamico per l'allocazione di memoria durante l'esecuzione.

4. **Multitasking e contesto**  
   - Il sistema operativo permette l'esecuzione di più processi contemporaneamente (multitasking).  
   - Durante il passaggio da un processo all'altro, il sistema salva e ripristina il **contesto** del processo, che include registri, contatori di programma e stato della CPU.

---

#### Differenza tra processo e programma  
- **Programma**: è un'entità statica, rappresenta il codice scritto dall'utente e salvato su disco.  
- **Processo**: è un'entità dinamica, che rappresenta l'esecuzione di un programma con il suo stato e le sue risorse.

---

#### Tipi di processi  
1. **Processi di sistema**: gestiscono le operazioni di base del sistema operativo.  
2. **Processi dell'utente**: creati dagli utenti per eseguire applicazioni.  
3. **Processi sequenziali**: eseguono un'operazione alla volta.  
4. **Processi paralleli**: eseguono più operazioni contemporaneamente.

---

#### Il ruolo del sistema operativo  
Il sistema operativo:
- Assegna risorse (CPU, memoria, I/O) ai processi.  
- Gestisce la creazione, la terminazione e la comunicazione tra processi.  
- Implementa la sicurezza, impedendo ai processi di interferire tra loro.

---

#### Esempio pratico  
Un esempio comune di processo è un editor di testo in esecuzione. Quando l'utente apre il programma:  
- Il sistema operativo crea un processo associato al programma.  
- Viene assegnata memoria per il codice, i dati e le variabili.  
- Il processo passa tra gli stati durante l'esecuzione (ad esempio, quando legge un file o aspetta input dell'utente).  

Questo concetto sarà approfondito nei capitoli successivi, con particolare attenzione alla programmazione in linguaggio C per la gestione dei processi.

---
[INDICE](README.md)