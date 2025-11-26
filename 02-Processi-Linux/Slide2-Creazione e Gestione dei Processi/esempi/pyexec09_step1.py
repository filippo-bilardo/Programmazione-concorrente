#!/usr/bin/env python3
"""
pyexec09_step1.py
Esecuzione multipla di comandi tramite input - Step 1
Scomposizione della stringa di comando
"""
import sys

if __name__ == "__main__":
    # Lettura del comando da eseguire
    cmd = input("\nInserire il comando da eseguire: ")
    
    # Scomposizione del comando (in Python è molto più semplice!)
    args = cmd.split()
    
    print(f"\ncmd={cmd}\n")
    for i, arg in enumerate(args):
        print(f"argv[{i}]='{arg}'")
    
    sys.exit(0)
