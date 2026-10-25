#!/usr/bin/env python3
"""
pysig05-single_handler.py
Date: 21/03/17
Author: Filippo Bilardo

In questo esempio si utilizza una sola funzione di handler per gestire vari segnali.
 
eseguire il processo in background
$ ./pysig05-single_handler.py &

inviare il segnale SIGUSR1 al processo
$ kill -10 <pid processo>

inviare il segnale SIGUSR2 al processo
$ kill -12 <pid processo>

inviare il segnale CTRL-C al processo
$ kill -SIGINT <pid processo>

Ver   Date        Comment
1.0   21/03/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler_signal(signum, frame):
    """Handler unico per diversi segnali"""
    if signum == signal.SIGINT:
        print("[handler] Ricevuto il segnale SIGINT")
        sys.exit(1)
    elif signum == signal.SIGUSR1:
        print("[handler] Ricevuto il segnale SIGUSR1")
    elif signum == signal.SIGUSR2:
        print("[handler] Ricevuto il segnale SIGUSR2")
    else:
        print(f"[handler] Ricevuto il segnale {signum}")

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler_signal)   # 2  - Interrupt (ANSI). Action: exit
    signal.signal(signal.SIGUSR1, handler_signal)  # 10 - User-defined signal 1 (POSIX)
    # signal.signal(signal.SIGUSR2, handler_signal) # 12 - User-defined signal 2 (POSIX)
    
    print(f"\n[INIZIO] PID: {os.getpid()}\n")
    
    # Ciclo infinito
    while True:
        print("Ciclo infinito ...")
        time.sleep(5)
    
    # Codice mai eseguito
    print("\n[FINE]\n")
    sys.exit(0)
