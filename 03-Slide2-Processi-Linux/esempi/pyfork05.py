#==========================================================================================
# Project: fork05.py
# Date: 2025
# Author: Tradotto da fork05.c
#==========================================================================================
# Creazione di un processo figlio
# - un processo ha sempre un padre. Se il padre termina prima del figlio, il padre diventa
#   il padre del padre e così via fino ad arrivare al processo "Init" pid=1.
#   Quindi, se il processo padre e il padre del padre (bash) terminano prima del figlio
#   il processo padre diventa "Init" pid=1
#
#==========================================================================================

import os
import time

#=== Local Functions ======================================================================

def child_process():
    # Se aspettiamo 1 secondo il processo padre e il padre del padre (bash) potrebbero terminare
    # il processo padre diventerà "Init"
    time.sleep(1)
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")

def father_process(childpid):
    print(f"\n[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid figlio={childpid}")

#=== Main =================================================================================

def main():
    print("\n[INIZIO] - Esiste solo il processo padre, il padre del padre è la shell")
    print(f"[INIZIO] - Prima della fork - pid processo={os.getpid()}, pid padre={os.getppid()}")

    # Creazione del processo figlio
    pid = os.fork()

    if pid == 0:
        # Esecuzione codice del processo figlio
        child_process()
    elif pid > 0:
        # Esecuzione codice del processo padre
        father_process(pid)
    else:  # pid < 0
        # Errore
        print("Creazione del processo figlio fallita!")

    # Codice comune a processo padre e figlio
    print(f"[FINE] - Termine del processo con pid={os.getpid()}, padre={os.getppid()}")

if __name__ == "__main__":
    main()
