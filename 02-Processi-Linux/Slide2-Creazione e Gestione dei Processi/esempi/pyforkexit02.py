#!/usr/bin/env python3
"""
pyforkexit02.py - © FB - 7/12/20 -> 26/11/25
La terminazione involontaria del processo tramite il
tentativo di una azione illegale.
echo $? per visualizzare l'exit status dell'ultimo comando
"""
import sys

if __name__ == "__main__":
    a = 0
    a = 10 / a  # ZeroDivisionError
    
    print("questo codice non verrà eseguito")
    sys.exit(1)
