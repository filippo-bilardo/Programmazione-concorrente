### 1.4 Panoramica sulle System Call  

Le **system call** sono il meccanismo principale attraverso cui un programma applicativo comunica con il sistema operativo. Permettono ai processi di accedere ai servizi e alle risorse gestite dal kernel, come file, dispositivi di input/output, memoria e processi stessi.  

---

#### Cos’è una system call?  
Una **system call** è una funzione speciale che trasferisce il controllo dall’ambiente utente al kernel, permettendo al processo di eseguire operazioni privilegiate.  
- Il passaggio avviene attraverso una **trap**, che commuta il processore dalla modalità **utente** a quella **kernel**.  
- Dopo l'esecuzione della system call, il controllo torna al processo in modalità utente.  

---

#### Perché utilizzare le system call?  
1. **Accesso sicuro alle risorse**: Le risorse hardware e software sono protette e accessibili solo tramite il kernel.  
2. **Astrazione hardware**: Nascondono la complessità dell’interazione con dispositivi hardware, fornendo un'interfaccia uniforme.  
3. **Sincronizzazione e gestione**: Forniscono meccanismi per sincronizzare processi e gestire eventi.  

---

#### Tipi principali di system call  

1. **Gestione dei file**  
   - Creazione, apertura, lettura, scrittura e chiusura di file.  
   - Esempi: `open()`, `read()`, `write()`, `close()`.  

2. **Gestione dei processi**  
   - Creazione e terminazione di processi, cambio di stato, sincronizzazione.  
   - Esempi: `fork()`, `exec()`, `exit()`, `wait()`.  

3. **Gestione della memoria**  
   - Allocazione e deallocazione di memoria.  
   - Esempi: `brk()`, `mmap()`.  

4. **Comunicazione tra processi (IPC)**  
   - Scambio di dati tra processi tramite pipe, memoria condivisa, messaggi, segnali.  
   - Esempi: `pipe()`, `shmget()`, `kill()`.  

5. **Gestione dei dispositivi**  
   - Operazioni di input/output sui dispositivi, come tastiera o disco.  
   - Esempi: `ioctl()`, `read()`, `write()`.  

6. **Networking**  
   - Comunicazione tra dispositivi in rete.  
   - Esempi: `socket()`, `bind()`, `send()`, `recv()`.  

---

#### Come funzionano le system call?  

1. **Invocazione**  
   Il programma utente invoca una funzione di libreria (es. `printf()` o `fopen()`), che internamente chiama una system call appropriata (es. `write()` o `open()`).  

2. **Trap**  
   Una trap software commuta il processore in modalità kernel, trasferendo il controllo al sistema operativo.  

3. **Esecuzione**  
   Il kernel esegue l'operazione richiesta, accedendo alle risorse e aggiornando il contesto del processo.  

4. **Ritorno**  
   Il kernel ritorna alla modalità utente, passando il risultato della system call al processo.  

---

#### Esempio pratico: system call `write()`  

La system call `write()` permette di scrivere dati su un file o un dispositivo.  

Sintassi:  
```c
ssize_t write(int fd, const void *buf, size_t count);
```  

- **fd**: descrittore del file.  
- **buf**: puntatore al buffer di dati da scrivere.  
- **count**: numero di byte da scrivere.  
- **Ritorno**: numero di byte effettivamente scritti o `-1` in caso di errore.  

Esempio:  

```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);

    if (fd < 0) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    const char *message = "Hello, world!\n";
    ssize_t bytes_written = write(fd, message, 14);

    if (bytes_written < 0) {
        perror("Errore nella scrittura");
    } else {
        printf("Scritti %ld byte nel file\n", bytes_written);
    }

    close(fd);
    return 0;
}
```

---

#### Differenza tra chiamate di libreria e system call  
- **Chiamate di libreria**: Forniscono un'astrazione di livello superiore. Spesso invocano system call, ma possono anche eseguire operazioni senza interagire con il kernel.  
  Esempio: `printf()` utilizza `write()` per inviare dati alla console.  
- **System call**: Interagiscono direttamente con il kernel per accedere alle risorse del sistema.  

---

#### Vantaggi delle system call  
1. **Efficienza**: Consentono un accesso diretto e ottimizzato alle risorse.  
2. **Flessibilità**: Offrono una vasta gamma di funzionalità, dai file ai processi, fino alla rete.  
3. **Sicurezza**: Il kernel garantisce che le operazioni siano eseguite in modo controllato.  

#### Limiti delle system call  
- **Complessità**: Il loro utilizzo diretto può richiedere una conoscenza dettagliata del sistema operativo.  
- **Overhead**: La transizione tra modalità utente e kernel introduce un sovraccarico.  

---

Le system call sono il fondamento della programmazione di sistema in C e saranno approfondite nei capitoli successivi, con esempi pratici per gestire file, processi e comunicazioni.

---
[INDICE](README.md)