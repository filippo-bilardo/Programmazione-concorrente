### 3.1 Introduzione ai thread

I **thread** sono entità di esecuzione più leggere rispetto ai processi, e consentono a un programma di eseguire più operazioni contemporaneamente all'interno dello stesso spazio di indirizzamento. Mentre i processi hanno uno spazio di memoria separato, i thread di uno stesso processo condividono lo stesso spazio di memoria, il che rende più semplice e veloce la comunicazione tra di loro, ma richiede anche una gestione attenta della concorrenza per evitare problemi come le condizioni di gara.

#### **Thread vs Processo**

Un thread è spesso definito come un "processo leggero" per via delle seguenti differenze rispetto ai processi tradizionali:

- **Condivisione della memoria**: I thread all'interno dello stesso processo condividono lo stesso spazio di memoria, mentre i processi hanno spazi di memoria separati. Questo significa che i thread possono accedere direttamente alle variabili e alle strutture dati del processo in cui si trovano, rendendo la comunicazione tra thread più rapida rispetto a quella tra processi.
  
- **Costi di creazione e gestione**: Creare un thread richiede meno risorse rispetto alla creazione di un processo, poiché i thread non necessitano di creare un nuovo spazio di memoria o di duplicare la maggior parte delle risorse del processo padre.
  
- **Condivisione delle risorse**: Oltre alla memoria, i thread condividono altre risorse del processo come i file aperti, le variabili globali e i segnali. Questo facilita il coordinamento tra thread, ma introduce la necessità di meccanismi di sincronizzazione per prevenire accessi concorrenti alle risorse condivise.

#### **Vantaggi dell'uso dei thread**

1. **Parallelismo e prestazioni**: I thread permettono di sfruttare meglio i processori multicore, eseguendo diverse parti di un programma in parallelo. Questo può ridurre i tempi di esecuzione e migliorare l'efficienza.
   
2. **Risparmio di risorse**: Poiché i thread condividono lo stesso spazio di memoria, il consumo di memoria è inferiore rispetto all'uso di più processi separati.

3. **Comunicazione efficiente**: La comunicazione tra thread è più semplice e veloce rispetto a quella tra processi. Non è necessario utilizzare meccanismi di comunicazione inter-processo (IPC), come pipe o socket.

4. **Responsività**: In molte applicazioni, specialmente quelle interattive, l'uso di thread permette di mantenere l'interfaccia utente reattiva anche durante operazioni che richiedono molto tempo, come il caricamento di file o l'accesso alla rete.

#### **Svantaggi e difficoltà dell'uso dei thread**

1. **Concorrenza e sincronizzazione**: Poiché i thread condividono la memoria, è necessario gestire correttamente l'accesso alle risorse condivise per evitare **race condition** (condizioni di gara) e **deadlock** (impasse). La programmazione concorrente può essere complessa e difficile da debuggare.

2. **Sicurezza**: I thread condividono lo spazio di memoria del processo. Un thread malfunzionante o insicuro può compromettere l'integrità di tutto il programma, accedendo a dati condivisi o alterandoli in modo imprevisto.

3. **Efficienza relativa**: Sebbene i thread siano più leggeri dei processi, il sovraccarico di gestione (come il passaggio di contesto e la sincronizzazione) può comunque limitare i benefici del parallelismo in determinate situazioni, specialmente su hardware con poche CPU o in programmi con blocchi frequenti.

#### **Modelli di threading**

Esistono due principali modelli di threading che possono essere implementati nei sistemi operativi:

- **Thread a livello utente**: In questo modello, i thread sono gestiti direttamente dall'applicazione senza l'intervento del sistema operativo. Ciò consente un controllo più fine sui thread, ma rende più difficile sfruttare appieno i benefici del parallelismo, poiché il kernel vede il programma come un singolo thread e non può schedulare i thread dell'applicazione su diversi core.

- **Thread a livello kernel**: In questo modello, i thread sono gestiti direttamente dal sistema operativo, che è responsabile della loro creazione, gestione e schedulazione. Questo modello permette al sistema operativo di sfruttare meglio i core della CPU e garantisce che i thread possano essere eseguiti in parallelo.

In Linux, il modello comunemente utilizzato è quello dei **thread a livello kernel**, attraverso il supporto fornito da `pthread`, che è l'implementazione standard delle **POSIX threads**.

#### **Introduzione alla libreria POSIX Threads (pthread)**

La libreria **pthread** è una delle implementazioni più comuni per la gestione dei thread su sistemi Unix-like, incluso Linux. Offre un'ampia gamma di funzionalità per la creazione e la gestione dei thread, oltre a meccanismi di sincronizzazione come mutex e variabili di condizione.

Alcune delle funzioni più importanti della libreria **pthread** includono:

- **Creazione di thread** (`pthread_create()`)
- **Terminazione di thread** (`pthread_exit()`)
- **Attesa della terminazione di un thread** (`pthread_join()`)
- **Sincronizzazione tramite mutex** (`pthread_mutex_lock()`, `pthread_mutex_unlock()`)
- **Uso delle variabili di condizione** (`pthread_cond_wait()`, `pthread_cond_signal()`)

Esempio di creazione di un thread con `pthread_create()`:
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *print_message(void *arg) {
    printf("Hello from the thread!\n");
    return NULL;
}

int main() {
    pthread_t thread_id;
    
    // Creazione del thread
    if (pthread_create(&thread_id, NULL, print_message, NULL)) {
        fprintf(stderr, "Errore nella creazione del thread\n");
        return 1;
    }
    
    // Attende che il thread termini
    pthread_join(thread_id, NULL);
    
    printf("Thread terminato\n");
    return 0;
}
```

In questo esempio, un thread viene creato usando `pthread_create()`, e la funzione `pthread_join()` viene usata per aspettare che il thread termini la sua esecuzione.

#### **Conclusione**

L'introduzione ai thread mostra come essi siano uno strumento potente per gestire la concorrenza all'interno di un'applicazione, consentendo di eseguire più operazioni contemporaneamente nello stesso processo. Tuttavia, l'uso dei thread comporta anche sfide legate alla sincronizzazione e alla gestione della memoria condivisa, che devono essere affrontate con attenzione per evitare errori difficili da individuare.

---
[INDICE](README.md)