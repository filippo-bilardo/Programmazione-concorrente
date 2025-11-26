#!/usr/bin/env python3
"""
pyexec06_execve.py - © FB - 12/03/17
Esempio di utilizzo della istruzione execve

os.execve(path, args, env) sostituisce il processo corrente
- path è il percorso (assoluto o relativo) dell'eseguibile da caricare
- args è la lista degli argomenti del programma da eseguire
- env è il dizionario delle variabili di ambiente da sostituire all'ambiente
  del processo
"""
import os
import sys

if __name__ == "__main__":
    pathname = "/bin/ls"
    myenv = {"USER": "anna", "PATH": "/home/anna/d1"}
    myargv = ["ls", "."]
    
    try:
        os.execve(pathname, myargv, myenv)
    except OSError as e:
        print(f"exec fallita! A causa dell'errore: {e}")
        sys.exit(112)
