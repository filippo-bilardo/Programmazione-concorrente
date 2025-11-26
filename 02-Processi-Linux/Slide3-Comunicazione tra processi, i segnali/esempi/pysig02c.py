#!/usr/bin/env python3
"""
pysig02b.py
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
import os

def handler_ctrl_c(signum, frame):
    """Handler per il segnale SIGINT"""
    print("\n\n[handler] gestione dell'eccezione CTRL-C")
    print(f"[handler] Ricevuto il segnale SIGINT [{signum}]; SIGINT={signal.SIGINT}")
    print("[handler] Fine del processo.\n")
    sys.exit(1)

def handler(signum, frame):
    """Handler generico per SIGUSR1 e SIGUSR2"""
    print(f"[handler] Ricevuto il segnale {signum}")
    if signum == signal.SIGUSR1:
        print("[handler] Ricevuto il segnale SIGUSR1")
        print("[handler] Accendi led ROSSO")
    elif signum == signal.SIGUSR2:
        print("[handler] Ricevuto il segnale SIGUSR2")
        print("[handler] Accendi led VERDE")

if __name__ == "__main__":
    print("\n[INIZIO]")
    
    # al segnale SIGINT (CTRL-C) associamo la funzione (handler) handler_ctrl_c
    signal.signal(signal.SIGINT, handler_ctrl_c)
    signal.signal(signal.SIGUSR1, handler)
    signal.signal(signal.SIGUSR2, handler)
    
    # Ciclo infinito
    while True:
        print("\nCiclo infinito ...")
        time.sleep(20)
    
    # Codice mai eseguito
    print("\n\n[FINE]")
    sys.exit(0)
