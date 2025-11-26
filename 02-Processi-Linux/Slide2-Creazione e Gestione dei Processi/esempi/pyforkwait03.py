#!/usr/bin/env python3
"""
pyforkwait03.py - © FB - 7/12/20 -> 26/11/25

Esempio di utilizzo istruzione wait con un processo figlio e terminazione volontaria.
In caso di terminazione di un figlio, la variabile status raccoglie stato di 
terminazione; nell'ipotesi che lo stato sia un intero a 16 bit:
+ se il byte meno significativo di status e' zero, il più significativo rappresenta
  lo stato di terminazione (terminazione volontaria, ad esempio con exit)
+ in caso contrario, il byte meno significativo di status descrive il segnale
  che ha terminato il figlio (terminazione involontaria)

Ver  Date      Comment
1.0  07/12/20  Versione iniziale
"""
import os
import sys
import time

def child_process():
    time.sleep(2)
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}\n")
    sys.exit(100)

def father_process(childpid):
    print(f"[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid figlio={childpid}")
    wait_retvalue, wait_status = os.wait()
    wait_status_high = wait_status >> 8  # wait_status // 256
    wait_status_low = wait_status & 0xFF
    print(f"[Padre] - wait_retvalue={wait_retvalue}, wait_status_high={wait_status_high}, wait_status_low={wait_status_low}\n")

if __name__ == "__main__":
    print(f"\n[INIZIO] - Prima della fork - pid processo={os.getpid()}, pid padre={os.getppid()}")
    
    pid = os.fork()
    
    if pid == 0:  # Processo figlio
        child_process()
    elif pid > 0:  # Processo padre
        father_process(pid)
    else:  # Errore nella creazione del processo
        print("Creazione del processo figlio fallita!")
    
    sys.exit(4)
