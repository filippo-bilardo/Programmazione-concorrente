### 1.3 Il ruolo del kernel nella gestione dei processi  

Il **kernel** è il componente centrale di un sistema operativo. Esso gestisce le risorse hardware e software e fornisce un'interfaccia tra l'hardware e i processi in esecuzione. Nella gestione dei processi, il kernel svolge un ruolo fondamentale, assicurando che i processi possano coesistere e interagire senza interferenze o conflitti.  

---

#### Funzioni principali del kernel nella gestione dei processi  

1. **Creazione e terminazione dei processi**  
   - Il kernel si occupa di creare nuovi processi quando richiesto, utilizzando system call come `fork()`, e di terminare i processi con system call come `exit()`.  
   - Assegna a ogni processo un **Process ID (PID)** univoco.  
   - Mantiene informazioni su ogni processo in una struttura chiamata **Process Control Block (PCB)**.  

2. **Gestione dello scheduling**  
   - Il kernel decide quale processo deve essere eseguito in un dato momento.  
   - Lo **scheduler** assegna la CPU ai processi in base a politiche come:  
     - **First-Come, First-Served (FCFS)**  
     - **Round Robin (RR)**  
     - **Shortest Job Next (SJN)**  
     - **Priority Scheduling**  
   - Lo scopo dello scheduling è massimizzare l'utilizzo della CPU e garantire equità tra i processi.  

3. **Commutazione di contesto (Context Switching)**  
   - Quando la CPU passa da un processo a un altro, il kernel salva lo stato del processo corrente (nel PCB) e carica lo stato del processo successivo.  
   - Questo processo è cruciale per il multitasking e deve essere rapido per ridurre il **context-switch overhead**.  

4. **Gestione della memoria**  
   - Il kernel assegna memoria ai processi durante la loro esecuzione e libera la memoria quando i processi terminano.  
   - Utilizza tecniche come **paginazione** e **segmentazione** per isolare i processi e prevenire conflitti.  

5. **Sincronizzazione dei processi**  
   - Il kernel fornisce meccanismi per la sincronizzazione, come semafori, mutex e barriere, per evitare problemi come le **race conditions** e garantire l'accesso controllato alle risorse condivise.  

6. **Comunicazione tra processi (IPC)**  
   - I processi spesso necessitano di comunicare tra loro. Il kernel supporta vari meccanismi di **Inter-Process Communication (IPC)**, tra cui:  
     - **Pipe**  
     - **FIFO (named pipe)**  
     - **Socket**  
     - **Memoria condivisa**  

7. **Gestione dei segnali**  
   - Il kernel invia e gestisce segnali per notificare ai processi eventi come errori (es. divisione per zero) o interruzioni (es. `SIGINT` da tastiera).  

8. **Gestione dell'I/O**  
   - Il kernel coordina le richieste di input/output dei processi, fornendo un'interfaccia unificata per l'accesso ai dispositivi hardware.  
   - Garantisce che l'I/O sia eseguito in modo sicuro ed efficiente, spesso utilizzando buffer e tecniche di caching.  

---

#### Il **Process Control Block (PCB)**  

Il PCB è una struttura dati mantenuta dal kernel per tracciare lo stato di ogni processo. Contiene:  
- **PID**: identificatore univoco del processo.  
- **Stato**: pronto, in esecuzione, bloccato, terminato.  
- **Contesto della CPU**: valori dei registri, contatore del programma.  
- **Informazioni sulla memoria**: segmenti di codice, stack, heap, pagine allocate.  
- **Priorità**: utilizzata dallo scheduler.  
- **Stato delle risorse**: file aperti, dispositivi utilizzati.  

---

#### Esempio pratico: la creazione di un processo  

Quando si utilizza la system call `fork()`, il kernel:  
1. Crea una copia del PCB del processo padre.  
2. Duplica la memoria e le risorse necessarie per il processo figlio.  
3. Assegna un nuovo PID al processo figlio.  
4. Ritorna al processo padre e al processo figlio indicando quale processo è quale.  

Esempio in C:  

```c
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid == 0) {
        printf("Questo è il processo figlio (PID: %d)\n", getpid());
    } else if (pid > 0) {
        printf("Questo è il processo padre (PID: %d)\n", getpid());
    } else {
        perror("Errore nella creazione del processo");
    }

    return 0;
}
```

---

#### Conclusione  
Il kernel è il cuore della gestione dei processi. Grazie al kernel, i processi possono essere creati, eseguiti, sincronizzati e terminati in modo efficiente, garantendo stabilità, sicurezza e utilizzo ottimale delle risorse del sistema operativo.


---
[INDICE](README.md)