#!/usr/bin/env python3
"""
pysig11-kill.py (duplicato di sig10-kill.c)
Date: 02/04/17
Author: Filippo Bilardo

Esempio di invio segnali tramite la system call kill tra processi.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time
import os

cont = 0

def handler(signum, frame):
    """Handler per il segnale SIGUSR1"""
    global cont
    cont += 1
    print(f"[handler] Proc. {os.getpid()}: ricevuti n. {cont} segnali {signum}")
    if cont == 5:
        print(f"\n[handler] fine processo figlio. Proc. {os.getpid()}\n")
        sys.exit(0)

def processo_padre(pidfiglio):
    """Funzione del processo padre"""
    for i in range(10, 0, -1):
        print(f"[PADRE] Proc. {os.getpid()}. iterazione {i} ...")
        os.kill(pidfiglio, signal.SIGUSR1)
        time.sleep(1)

def processo_figlio():
    """Funzione del processo figlio"""
    while True:
        print(f"[FIGLIO] Proc. {os.getpid()}. Ciclo infinito ...")
        time.sleep(2)

if __name__ == "__main__":
    print(f"\n[INIZIO] Proc. {os.getpid()}. \n")
    signal.signal(signal.SIGUSR1, handler)
    
    pid = os.fork()
    if pid > 0:
        processo_padre(pid)
    elif pid == 0:
        processo_figlio()
    else:
        # Errore
        pass
    
    # Codice mai eseguito
    print(f"\n[FINE] Proc. {os.getpid()}. \n")
    sys.exit(0)
