#!/usr/bin/env python3
"""
pysig14-sleep.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio di risveglio durante l'esecuzione di una system call sleep,
mediante la ricezione di un segnale.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time

def handler(signum, frame):
    """Handler per il segnale"""
    print(f"\nRicevuto il segnale {signum}")
    print("Sono stato risvegliato!")

if __name__ == "__main__":
    print("\n[INIZIO]\n")
    signal.signal(signal.SIGINT, handler)
    
    # Sleep 1000 secondi
    print("Sleep(1000). premere CTRL-C per risvegliare il processo.")
    try:
        time.sleep(1000)
        k = 0
    except:
        # In Python, quando sleep viene interrotto, solleva un'eccezione
        print("Sleep interrotto dal segnale.")
    
    print("[FINE]\n")
    sys.exit(0)
