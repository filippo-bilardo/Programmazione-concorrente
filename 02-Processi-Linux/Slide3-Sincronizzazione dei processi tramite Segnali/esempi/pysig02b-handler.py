#!/usr/bin/env python3
"""
pysig02-handler.py
Date: 21/03/17
Author: Filippo Bilardo

In questo esempio al segnale SIGINT (CTRL-C), associamo una funzione (handler) che 
verra' eseguita alla ricezione del segnale.

Ver   Date        Comment
1.0   21/03/17    Versione iniziale
"""
import signal
import sys
import time

def gestore_ctrl_c(signum, frame):
    """Handler per il segnale SIGINT"""
    print("\n\n[handler] gestione dell'eccezione CTRL-C")
    print(f"[handler] Ricevuto il segnale SIGINT [{signum}]; SIGINT={signal.SIGINT}")
    print("[handler] Fine del processo.\n")
    sys.exit(123)

if __name__ == "__main__":
    print("\n[INIZIO]")
    
    # al segnale SIGINT (CTRL-C) associamo la funzione (handler) gestore_ctrl_c
    signal.signal(signal.SIGINT, gestore_ctrl_c)
    
    # Ciclo infinito
    while True:
        print("\nCiclo infinito ...")
        time.sleep(1)
    
    # Codice mai eseguito
    print("\n\n[FINE]")
    sys.exit(0)
