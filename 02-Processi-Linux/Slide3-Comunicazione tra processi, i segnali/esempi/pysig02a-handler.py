#!/usr/bin/env python3
"""
pysig01-no_handler.py
Date: 21/03/17
Author: Filippo Bilardo

Se non e' definito nessun handler di segnale, quando il Kernel o la shell inviano 
il segnale, verra' eseguita l'operazione di default.
Provare a eseguire il seguente codice.
Cosa succede se nella shell si preme CTRL-C ?

Ver   Date        Comment
1.0   21/03/17    Versione iniziale
"""
import sys
import time

if __name__ == "__main__":
    print("\n[INIZIO]")
    
    # Ciclo infinito
    try:
        while True:
            print("\nCiclo infinito ...")
            time.sleep(1)
    except KeyboardInterrupt:
        # Python gestisce automaticamente CTRL-C con KeyboardInterrupt
        print("\n\nInterrotto da CTRL-C")
        sys.exit(0)
    
    # Codice mai eseguito
    print("\n\n[FINE]")
    sys.exit(0)
