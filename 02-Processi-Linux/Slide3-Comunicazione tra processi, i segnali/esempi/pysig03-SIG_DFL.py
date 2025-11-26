#!/usr/bin/env python3
"""
pysig03-SIG_DFL.py
Date: 21/03/17
Author: Filippo Bilardo

In questo esempio viene modificato il comportamento di default del segnale 
SIGINT (CTRL-C) e il programma non verra' terminato, successivamente viene ripristinato 
il comportamento di default.

Ver   Date        Comment
1.0   21/03/17    Versione iniziale
"""
import signal
import sys
import time

def handler_signal_ctrlc(signum, frame):
    """Handler che ripristina il comportamento di default"""
    print("\n\n[handler] gestione dell'eccezione CTRL-C")
    print("[handler] Ripristino del comportamento di default\n")
    signal.signal(signal.SIGINT, signal.SIG_DFL)  # SIGINT torna a default
    return

if __name__ == "__main__":
    print("\n[INIZIO]\n")
    
    # associamo la funzione handler_signal_ctrlc al segnale SIGINT
    signal.signal(signal.SIGINT, handler_signal_ctrlc)
    
    # Ciclo infinito
    while True:
        print("Ciclo infinito ...")
        time.sleep(1)
    
    # Codice mai eseguito
    print("\n[FINE]\n")
    sys.exit(0)
