#!/usr/bin/env python3
"""
pyexec09_scanf.py
Esecuzione multipla di comandi tramite input
Versione completa con fork ed exec
"""
import os
import sys

def esegui_cmd(args):
    """Esegue il comando specificato negli argomenti"""
    if not args:
        return
    
    # Costruzione del pathname
    pathname = f"/bin/{args[0]}"
    
    print("Fork")
    pid = os.fork()
    
    if pid < 0:
        print("Creazione processo fallita!")
        sys.exit(1)
    elif pid == 0:  # Processo figlio
        try:
            os.execv(pathname, args)
        except OSError as e:
            print(f"exec fallita! Errore: {e}")
            sys.exit(112)
    else:  # Processo padre
        # Attesa della terminazione del processo figlio
        wpid, wstatus = os.wait()
        wstatus_high = wstatus >> 8
        wstatus_low = wstatus & 0xFF
        print(f"\n[PADRE] - Child wpid={wpid}, exited with wstatus_high={wstatus_high}, wstatus_low={wstatus_low}")

if __name__ == "__main__":
    print(f"\n[PADRE INIZIO] - Prima della fork - pid processo={os.getpid()}, pid del padre={os.getppid()}\n")
    
    while True:
        # Lettura del comando da eseguire
        try:
            cmd = input("\n>>> ").strip()
        except EOFError:
            break
        
        if not cmd:
            continue
        
        # Scomposizione del comando
        args = cmd.split()
        
        print(f"\ncmd={cmd}\n")
        for i, arg in enumerate(args):
            print(f"argv[{i}]='{arg}'")
        
        # Condizione d'uscita
        if args[0] == "quit":
            print("Fine esecuzione comandi")
            break
        
        # Esegui il comando
        esegui_cmd(args)
    
    print(f"\n[PADRE FINE] - Termine del processo con pid={os.getpid()} avente pid padre={os.getppid()}")
    print()
    sys.exit(0)
