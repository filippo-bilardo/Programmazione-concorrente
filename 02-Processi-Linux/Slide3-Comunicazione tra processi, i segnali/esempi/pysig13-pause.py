#!/usr/bin/env python3
"""
pysig13-pause.py
Date: 02/04/17
Author: Filippo Bilardo

Esempio risveglio dopo l'esecuzione di una system call pause
mediante la ricezione di un qualsiasi segnale.

Ver   Date        Comment
1.0   02/04/17    Versione iniziale
"""
import signal
import sys
import os

def handler(signum, frame):
    """Handler generico per i segnali"""
    print(f"\n[handler] Ricevuto il segnale {signum}")

if __name__ == "__main__":
    print("\n[INIZIO] Avvio del processo.\n")
    
    # Associazione della funzione handler ai vari segnali
    print("Associo la funzione handler a vari segnali.")
    signal.signal(signal.SIGINT, handler)   # 2  - Interrupt (ANSI). Action: exit
    signal.signal(signal.SIGUSR1, handler)  # 10 - User-defined signal 1 (POSIX)
    signal.signal(signal.SIGUSR2, handler)  # 12 - User-defined signal 2 (POSIX)
    
    # metto in pausa il processo nell'attesa di un segnale
    print("Metto in pausa il processo nell'attesa dei un segnale.")
    signal.pause()
    
    # Fine
    print("\n[FINE] Terminazione del processo.\n")
    sys.exit(0)
