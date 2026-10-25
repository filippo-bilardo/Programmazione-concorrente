#!/usr/bin/env python3
"""
pysig08-exec.py
Date: 02/04/17
Author: Filippo Bilardo

Segnali & exec

dopo un exec, un processo:
- ignora gli stessi segnali ignorati prima di exec
- i segnali a default rimangono a default
ma
- i segnali che prima erano gestiti, vengono riportati a default

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import time
import os

def handler(signum, frame):
    """Handler per il segnale SIGINT"""
    print("\n[handler] - Gestione del segnale CTRL-C")

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    print("Esecuzione della istruzione sleep(10).")
    time.sleep(10)
    
    print("Esecuzione della istruzione exec: sleep 10.")
    os.execl("/bin/sleep", "sleep", "10")
    
    # Codice mai eseguito
    print("\n[FINE]\n")
    sys.exit(0)
