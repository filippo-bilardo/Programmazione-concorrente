#!/usr/bin/env python3
"""
pyexec02_nomagicnumber.py - © FB - 12/03/17
Esempio di utilizzo della istruzione execl
con definizione dei parametri da passare all'instruzione 
execl prima di richiamare la funzione.
"""
import os
import sys

if __name__ == "__main__":
    pathname = "/bin/ls"
    myarg = ["cmdls", "-a"]
    
    try:
        os.execl(pathname, myarg[0], myarg[1])
    except OSError:
        print("exec fallita!")
        sys.exit(112)
