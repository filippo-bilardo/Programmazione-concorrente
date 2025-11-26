#!/usr/bin/env python3
"""
pyexec03_exec_fork.py - © FB - 12/03/17
Esempio di utilizzo della istruzione execl insieme all'istruzione fork
"""
import os
import sys

if __name__ == "__main__":
    print(f"\n[INIZIO] - Prima della fork - pid={os.getpid()}, pid padre={os.getppid()}\n")
    
    pid = os.fork()
    
    if pid == 0:  # Processo figlio
        print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")
        try:
            os.execl("/bin/ls", "ls", "-a")
        except OSError:
            print("exec fallita!")
            sys.exit(112)
    elif pid > 0:  # Processo padre
        # Attesa della terminazione dei processi figli
        wait_retvalue, wait_status = os.wait()
        wait_status_high = wait_status >> 8
        wait_status_low = wait_status & 0xFF
        print(f"\n[Padre] - wait_retvalue={wait_retvalue}, wait_status_high={wait_status_high}, wait_status_low={wait_status_low}")
    else:  # Errore nella creazione del processo
        print("Creazione fallita!")
    
    print(f"[FINE] - Termine del processo con pid={os.getpid()} avente pid padre={os.getppid()}")
    sys.exit(0)
