### 1.5 Ciclo di vita e Stati dei processi

Il ciclo di vita di un processo descrive le diverse fasi attraverso cui un processo passa dal momento in cui viene creato fino alla sua terminazione. Durante la sua esecuzione, un processo può trovarsi in vari stati, che rappresentano il suo attuale status rispetto alla CPU e alle risorse.

#### **Stati principali di un processo:**

1. **New (Nuovo)**  
   Quando un processo viene appena creato, si trova nello stato *new*. In questa fase, il processo non è ancora stato caricato in memoria principale ed è in attesa di iniziare la sua esecuzione.

2. **Ready (Pronto)**  
   Un processo passa nello stato *ready* quando è stato caricato in memoria ed è pronto per l'esecuzione. Tuttavia, non sta ancora utilizzando la CPU perché c'è un altro processo in esecuzione. Il processo aspetta che il pianificatore (scheduler) assegni la CPU.

3. **Running (In esecuzione)**  
   Quando il processo ha il controllo della CPU e sta effettivamente eseguendo le sue istruzioni, si trova nello stato *running*. In questo stato, il processo utilizza le risorse per compiere il suo lavoro.

4. **Waiting (In attesa)**  
   Un processo entra nello stato *waiting* se ha bisogno di attendere che si verifichi un evento esterno, come la disponibilità di una risorsa o il completamento di un'operazione I/O. Durante questo stato, il processo non può proseguire finché l'evento richiesto non si verifica.

5. **Terminated (Terminato)**  
   Quando un processo completa la sua esecuzione o viene fermato forzatamente, si trova nello stato *terminated*. Il sistema operativo dealloca le risorse associate al processo. In questa fase, il processo non è più attivo, ma potrebbe ancora rimanere in memoria per raccogliere informazioni sul suo stato di uscita (exit status).

#### **Ciclo di vita dei processi:**

1. **Creazione del processo (New → Ready)**  
   Un nuovo processo viene creato (ad esempio tramite la system call `fork()`) e viene caricato in memoria. Da qui entra nello stato *ready* in attesa di essere eseguito.

2. **Assegnazione della CPU (Ready → Running)**  
   Il pianificatore del sistema operativo decide di assegnare la CPU al processo in stato *ready*, cambiando il suo stato in *running*. A questo punto, il processo inizia effettivamente a lavorare.

3. **Interruzione (Running → Ready)**  
   Se il processo non ha finito ma deve temporaneamente cedere la CPU (ad esempio, per la scadenza di un time slice o a causa di una interruzione), ritorna nello stato *ready* per essere riassegnato in futuro.

4. **Attesa di un evento (Running → Waiting)**  
   Il processo entra in uno stato di *waiting* se richiede l'attesa di un evento esterno (come l'I/O o un segnale). Rimane in questo stato finché l'evento non si verifica.

5. **Ripresa (Waiting → Ready)**  
   Quando l'evento di attesa si verifica, il processo ritorna allo stato *ready*, pronto per essere eseguito di nuovo non appena la CPU sarà disponibile.

6. **Terminazione (Running → Terminated)**  
   Alla conclusione dell'esecuzione, il processo termina e si sposta nello stato *terminated*. In questa fase, il sistema operativo dealloca le risorse associate e, se necessario, gestisce il risultato dell'esecuzione del processo.

#### **Transizioni di stato tipiche:**
- **New → Ready:** Il processo viene creato.
- **Ready → Running:** Il processo riceve la CPU.
- **Running → Ready:** Il processo è interrotto e ritorna in attesa.
- **Running → Waiting:** Il processo attende un evento esterno.
- **Waiting → Ready:** L'evento di attesa si è verificato.
- **Running → Terminated:** Il processo termina la sua esecuzione.

Il ciclo di vita del processo è fondamentale per comprendere come il sistema operativo gestisce la concorrenza, l'allocazione delle risorse e la pianificazione dei processi.

---
[INDICE](README.md)