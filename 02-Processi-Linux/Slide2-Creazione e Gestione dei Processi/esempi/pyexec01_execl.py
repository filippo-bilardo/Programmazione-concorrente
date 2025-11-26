#!/usr/bin/env python3
"""
pyexec01_execl.py - © FB - 12/03/17
Esempio di utilizzo della istruzione execl

In Python, os.execl(path, arg0, arg1, ...) sostituisce il processo corrente
- path è il percorso (assoluto o relativo) dell'eseguibile da caricare
- arg0 è il nome del programma (argv[0])
- arg1, ..., argN sono gli argomenti da passare al programma
"""
import os
import sys

if __name__ == "__main__":
    try:
        os.execl("/bin/ls", "cmd_ls", "-a")
    except OSError:
        print("exec fallita!")
        sys.exit(112)
