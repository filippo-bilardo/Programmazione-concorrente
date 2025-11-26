#!/usr/bin/env python3
"""
pyforkwait01.py - © FB - 7/12/20 -> 26/11/25

Lo stato di terminazione del processo figlio puo' essere rilevato dal processo padre, 
mediante la system call wait()

os.wait() ritorna una tupla (pid, status) dove:
- pid: PID del processo terminato
- status: codice di stato di terminazione

Se non esiste neanche un figlio, wait() NON e' sospensiva e solleva un'eccezione
ChildProcessError

Ver  Date      Comment
1.0  07/12/20  Versione iniziale
"""
import os
import sys

if __name__ == "__main__":
    try:
        wait_retvalue, wait_status = os.wait()
        print(f"[FINE] - wait_retvalue={wait_retvalue}, wait_status={wait_status}")
    except ChildProcessError:
        print(f"[FINE] - Nessun processo figlio da attendere (errore)")
        wait_retvalue = -1
        wait_status = -1
    
    print(f"[FINE] - Termine del processo con pid={os.getpid()} avente pid padre={os.getppid()}")
    print()
    
    sys.exit(0)
