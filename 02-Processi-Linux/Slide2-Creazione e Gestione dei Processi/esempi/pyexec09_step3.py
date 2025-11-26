#!/usr/bin/env python3
"""
pyexec09_step3.py
Esecuzione multipla di comandi tramite input - Step 3
Con funzione per eseguire il comando
"""
import sys

def esegui_cmd(cmd):
    """Scomposizione e visualizzazione del comando"""
    args = cmd.split()
    print(f"string='{cmd}'\n")
    
    for i, arg in enumerate(args):
        print(f"argv[{i}]='{arg}'")

if __name__ == "__main__":
    # Lettura del comando da eseguire
    cmd = input("\n>>> ")
    
    # Eseguo il comando
    esegui_cmd(cmd)
    
    sys.exit(0)
