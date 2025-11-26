#!/usr/bin/env python3
"""
pysig07-SIGCHLD.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio di gestione del segnale SIGCHLD con due figli.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler(signum, frame):
    """Handler per il segnale SIGCHLD"""
    pid, status = os.wait()
    print(f"[handler] Terminazione del processo figlio={pid} stato={status >> 8}")

def father(childpid1, childpid2):
    """Funzione del processo padre"""
    signal.signal(signal.SIGCHLD, handler)
    print(f"[PADRE] Processo padre={os.getpid()}. Figlio1={childpid1}, Figlio2={childpid2} - AVVIO")
    for i in range(10):
        time.sleep(1)
    print(f"[PADRE] Processo padre={os.getpid()} - TERMINAZIONE")

def child1():
    """Funzione del primo processo figlio"""
    print(f"[FIGLIO1] Processo {os.getpid()} - AVVIO")
    for i in range(4):
        time.sleep(1)
    print(f"[FIGLIO1] Processo {os.getpid()} - TERMINAZIONE")
    sys.exit(1)

def child2():
    """Funzione del secondo processo figlio"""
    print(f"[FIGLIO2] Processo {os.getpid()} - AVVIO")
    for i in range(7):
        time.sleep(1)
    print(f"[FIGLIO2] Processo {os.getpid()} TERMINAZIONE")
    sys.exit(2)

if __name__ == "__main__":
    print(f"\n[INIZIO] Processo {os.getpid()}\n")
    
    pid1 = os.fork()
    if pid1 > 0:
        # Attività del padre
        pid2 = os.fork()
        if pid2 > 0:
            father(pid1, pid2)
        elif pid2 == 0:
            child2()
        else:  # pid2 < 0
            print("Si e' verificato un'errore nella creazione del processo figlio2.")
    elif pid1 == 0:
        child1()
    else:  # pid1 < 0
        print("Si e' verificato un'errore nella creazione del processo figlio1.")
    
    # Fine
    print(f"[FINE] Processo {os.getpid()}\n")
    sys.exit(0)
