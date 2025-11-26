#!/usr/bin/env python3
"""
pyexec09_step2.py
Esecuzione multipla di comandi tramite input - Step 2
Utilizzo di split per scomporre la stringa
"""
import sys

def split_example():
    """Esempio di split in Python"""
    st = "Where there is will, there is a way."
    words = st.split()
    for word in words:
        print(word)

if __name__ == "__main__":
    split_example()
    
    # Lettura del comando da eseguire
    cmd = input("\nInserire il comando da eseguire: ")
    
    # Scomposizione del comando
    args = cmd.split()
    
    print(f"cmd={cmd}\n")
    for i, arg in enumerate(args):
        print(f"argv[{i}]='{arg}'")
    
    sys.exit(0)
