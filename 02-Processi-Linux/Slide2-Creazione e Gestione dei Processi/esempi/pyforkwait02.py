#!/usr/bin/env python3
"""
pyforkwait02.py - © FB - 7/12/20 -> 26/11/25

Esempio di utilizzo istruzione wait con un processo figlio 
- se tutti i figli non sono ancora terminati, il processo si sospende in attesa della 
  terminazione del primo di essi
- se almeno un figlio F e' gia' terminato ed il suo stato non e' stato ancora rilevato 
  (cioè F e' in stato zombie), wait() ritorna immediatamente con il suo stato di 
  terminazione
- se non esiste neanche un figlio, wait() NON e' sospensiva e solleva un'eccezione

Ver  Date      Comment
1.0  07/12/20  Versione iniziale
"""
import os
import sys
import time

def child_process():
    time.sleep(3)
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")
    sys.exit(100)

def father_process(childpid):
    print(f"\n[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid figlio={childpid}")
    wait_retvalue, wait_status = os.wait()
    print(f"[Padre] - wait_retvalue={wait_retvalue}, wait_status={wait_status}")

if __name__ == "__main__":
    print(f"\n[INIZIO] - Prima della fork - pid processo={os.getpid()}, pid padre={os.getppid()}")
    
    pid = os.fork()
    
    if pid == 0:  # Processo figlio
        child_process()
    elif pid > 0:  # Processo padre
        father_process(pid)
    else:  # Errore nella creazione del processo
        print("Creazione del processo figlio fallita!")
    
    print(f"[FINE] - Termine del processo con pid={os.getpid()} avente pid padre={os.getppid()}")
    sys.exit(3)
