#!/usr/bin/env python3
"""
pyforkwait05.py - © FB - 7/12/20 -> 26/11/25

Utilizzo istruzione wait con N_CHILD processi figli e terminazione volontaria.

Ver  Date      Comment
1.0  07/12/20  Versione iniziale
"""
import os
import sys

# Constants
N_CHILD = 4  # numero di processi da creare

def child_process(num):
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")
    sys.exit(num * 10)

if __name__ == "__main__":
    print(f"[INIZIO] - Prima della fork - pid={os.getpid()}, pid padre={os.getppid()}")
    
    pid = []
    for i in range(N_CHILD):
        p = os.fork()
        if p == 0:  # codice processo figlio
            child_process(i)
            # Nota: dopo sys.exit() non viene eseguito altro codice
        elif p > 0:  # codice processo padre
            pid.append(p)
            print(f"[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid[{i}]={p}")
        else:  # p < 0 - Errore
            print("Creazione del processo figlio fallita!")
    
    # Codice eseguito solo dal padre
    print(f"[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, num_figli={N_CHILD}")
    
    for i in range(N_CHILD):
        wait_retvalue, wait_status = os.wait()
        wait_status_high = wait_status >> 8  # wait_status // 256
        wait_status_low = wait_status & 0xFF
        print(f"[Padre] - wait_retvalue={wait_retvalue}, wait_status_high={wait_status_high}, wait_status_low={wait_status_low}")
    
    sys.exit(4)
