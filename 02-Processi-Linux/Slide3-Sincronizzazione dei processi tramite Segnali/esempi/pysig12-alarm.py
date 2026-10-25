#!/usr/bin/env python3
"""
pysig12-alarm.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio di recezione del segnale SIGALRM impostato tramite la system call alarm.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time

def handler(signum, frame):
    """Handler per il segnale SIGALRM"""
    print(f"[handler] Ricevuto il segnale {signum}")
    print("[handler] Terminazione processo.\n")
    sys.exit(1)

if __name__ == "__main__":
    print("\n[INIZIO]\n")
    
    # Associazione della funzione handler al segnale SIGALRM
    print("Associo la funzione handler al segnale SIGALRM.")
    signal.signal(signal.SIGALRM, handler)
    
    # imposto l'invio del segnale di allarme dopo 5 sec
    print("Imposto l'invio del segnale di allarme dopo 5 sec.")
    signal.alarm(5)
    
    # Ciclo infinito
    i = 1
    while True:
        print(f"Ciclo infinito ({i}) ...")
        i += 1
        time.sleep(1)
    
    # Codice mai eseguito
    print("\n[FINE]\n")
    sys.exit(0)
