#!/usr/bin/env python3
"""
pyexec08_scanf.py
Esecuzione multipla di comandi tramite input
"""
import os
import sys

if __name__ == "__main__":
    print(f"\n[PADRE INIZIO] - Prima della fork - pid processo={os.getpid()}, pid del padre={os.getppid()}\n")
    
    while True:
        # Lettura del comando da eseguire
        try:
            cmd = input("\nInserire il comando da eseguire: ").strip()
        except EOFError:
            break
        
        if not cmd:
            continue
        
        pathname = f"/bin/{cmd}"
        print(f"\nEsecuzione del comando:'{cmd}'; pathname:'{pathname}'\n")
        
        # Condizione d'uscita
        if cmd == "quit":
            print("Fine esecuzione comandi")
            break
        
        # Avvio del processo child ed esecuzione del comando
        print("Fork")
        pid = os.fork()
        
        if pid < 0:
            print("Creazione processo fallita!")
            sys.exit(1)
        elif pid == 0:  # Processo figlio
            print(f"\nEsecuzione del comando:'{cmd}'; pathname:'{pathname}'\n")
            try:
                os.execl(pathname, cmd)
            except OSError:
                print("exec fallita!")
                sys.exit(112)
        else:  # Processo padre
            # Attesa della terminazione del processo figlio
            wpid, wstatus = os.wait()
            wstatus_high = wstatus >> 8
            wstatus_low = wstatus & 0xFF
            print(f"\n[PADRE] - Child wpid={wpid}, exited with wstatus_high={wstatus_high}, wstatus_low={wstatus_low}")
    
    print(f"[PADRE FINE] - Termine del processo con pid={os.getpid()} avente pid padre={os.getppid()}")
    print()
    sys.exit(0)
