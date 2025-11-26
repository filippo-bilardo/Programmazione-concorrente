#!/usr/bin/env python3
"""
pyexec07_stringhe_exev.py
Esercizi con le stringhe ed istruzione execv
$ ./pyexec07_stringhe_exev.py ps ps -au
$ ./pyexec07_stringhe_exev.py ls ls -ah
"""
import os
import sys

if __name__ == "__main__":
    # Controllo dei parametri in ingresso
    if len(sys.argv) < 3:
        print(f"Numero di argomenti [{len(sys.argv)}] non valido!")
        sys.exit(1)
    
    print(f"Numero di argomenti passati, argc={len(sys.argv)}")
    
    # Costruzione del vettore myarg
    myarg = []
    for i in range(len(sys.argv)):
        if i < 2:
            print(f"argv[{i}]:={sys.argv[i]}")
        else:
            myarg.append(sys.argv[i])
            print(f"argv[{i}]:={sys.argv[i]} - myarg[{i-2}]:={myarg[i-2]}")
    
    # Costruzione del pathname
    pathbase = "/bin/"
    print(f"\npathbase={pathbase}")
    pathname = pathbase + sys.argv[1]
    print(f"pathname={pathname}")
    print(f"argv[1]={sys.argv[1]}")
    print(f"pathname={pathname}\n")
    
    # Esecuzione dell'istruzione execv
    try:
        os.execv(pathname, myarg)
    except OSError as e:
        print(f"exec fallita! Errore: {e}")
        sys.exit(1)
