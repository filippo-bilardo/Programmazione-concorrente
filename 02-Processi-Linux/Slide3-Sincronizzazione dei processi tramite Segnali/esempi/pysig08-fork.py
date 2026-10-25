#!/usr/bin/env python3
"""
pysig08-fork.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio di gestione del segnale SIGINT con fork.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler(signum, frame):
    """Handler per il segnale"""
    print(f"\n[handler] Ricevuto il segnale. Processo {signum} {os.getpid()}")
    
    pid, status = os.wait()
    print(f"[handler] Stato figlio: {status >> 8}. Processo {os.getpid()}")
    
    print(f"[handler] Terminazione del processo {os.getpid()}.\n")
    sys.exit(3)

def processo_padre():
    """Funzione del processo padre"""
    # Ciclo infinito
    while True:
        print(f"[PADRE] Ciclo infinito ... Processo {os.getpid()}")
        time.sleep(3)

def processo_figlio():
    """Funzione del processo figlio"""
    # Ciclo infinito
    while True:
        print(f"[FIGLIO] Ciclo infinito ... Processo {os.getpid()}")
        time.sleep(1)

if __name__ == "__main__":
    print("\n[INIZIO]\n")
    signal.signal(signal.SIGINT, handler)
    
    pid = os.fork()
    if pid > 0:
        processo_padre()
    elif pid == 0:
        processo_figlio()
    else:
        # Errore
        pass
    
    # Codice mai eseguito
    print("\n\n[FINE]")
    sys.exit(0)
