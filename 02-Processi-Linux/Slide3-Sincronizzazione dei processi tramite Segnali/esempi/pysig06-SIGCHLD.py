#!/usr/bin/env python3
"""
pysig06-SIGCHLD.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio di gestione del segnale SIGCHLD.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler(signum, frame):
    """Handler per il segnale SIGCHLD"""
    print(f"[handler] Ricevuto il segnale {signum}")
    
    pid, status = os.wait()
    print(f"[handler] Stato figlio: {status >> 8}")
    
    sys.exit(0)

def processo_padre():
    """Funzione del processo padre"""
    signal.signal(signal.SIGCHLD, handler)
    
    # Ciclo infinito
    while True:
        print("[PADRE] Ciclo infinito ...")
        time.sleep(1)

def processo_figlio():
    """Funzione del processo figlio"""
    print("[FIGLIO] Attesa 5 sec ...")
    time.sleep(5)
    sys.exit(1)

if __name__ == "__main__":
    print("\n[INIZIO]\n")
    
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
