#!/usr/bin/env python3
"""
pysig04-SIG_IGN.py
Date: 21/03/17
Author: Filippo Bilardo

In questo esempio viene modificato il comportamento di default
del segnale SIGINT (CTRL-C) e il programma non verra' terminato
successivamente vengono ignorati i successivi segnali SIGINT.
 
eseguire il processo in background
$ ./pysig04-SIG_IGN.py &

inviare piu' volte il segnale CTRL-C al processo
$ kill -SIGINT <pid processo>

per terminare il processo 
$ kill -9 <pid processo>
oppure 
$ kill <pid processo>

Ver   Date        Comment
1.0   21/03/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler_signal_ctrlc(signum, frame):
    """Handler che imposta l'ignore del segnale"""
    print("\n\n[handler] gestione del segnale CTRL-C")
    print("[handler] Ignoro i successivi segnali SIGINT\n")
    signal.signal(signal.SIGINT, signal.SIG_IGN)  # SIGINT viene ignorato
    return

if __name__ == "__main__":
    print(f"\n[INIZIO] PID: {os.getpid()}\n")
    
    # associamo la funzione handler_signal_ctrlc al segnale SIGINT
    signal.signal(signal.SIGINT, handler_signal_ctrlc)
    
    # Ciclo infinito
    while True:
        print("\nCiclo infinito ...")
        time.sleep(15)
    
    # Codice mai eseguito
    print("\n\n[FINE]")
    sys.exit(0)
