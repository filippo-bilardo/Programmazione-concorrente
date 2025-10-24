#==========================================================================================
# Project: fork06.py
# Date: 2025
# Author: Tradotto da fork06.c
#==========================================================================================
# Creazione di N_CHILD processi figlio
# Attenzione! Tutto il codice dopo l'istruzione fork viene eseguito anche dal processo figlio.
#
#==========================================================================================

import os

#=== Constants ============================================================================
N_CHILD = 4  # Numero di processi da creare

#=== Local Functions ======================================================================

def child_process(num):
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")

#=== Main =================================================================================

def main():
    print(f"[INIZIO] - Prima della fork - pid={os.getpid()}, pid padre={os.getppid()}")

    pid = [0] * N_CHILD     # Array per memorizzare i PID dei figli
    for i in range(N_CHILD):
        pid[i] = os.fork()
        if pid[i] == 0:  # Codice processo figlio
            child_process(i)
            os._exit(10)  # Attenzione! Ogni child riceve una "copia" del ciclo for
        elif pid[i] > 0:  # Codice processo padre
            print(f"[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid[{i}]={pid[i]}")
        else:  # pid < 0, Errore
            print("Creazione del processo figlio fallita!")

    # Codice eseguito solo dal padre
    print(f"[FINE] - pid={os.getpid()}, pid padre={os.getppid()}, N_CHILD={N_CHILD}")

if __name__ == "__main__":
    main()
