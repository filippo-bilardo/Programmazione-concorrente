#==========================================================================================
# Project: fork04.py
# Date: 2025
# Author: Tradotto da fork04.c
#==========================================================================================
# Creazione di un processo figlio.
# Organizzazione migliore del codice rispetto all'esempio precedente
# - ogni variabile del figlio è inizializzata con il valore assegnatole dal padre
#   prima della fork()
# - il figlio nasce con lo stesso program counter del padre: la prima istruzione eseguita
#   dal figlio è quella che segue immediatamente fork()
#   + il codice prima della fork viene eseguito solo dal processo padre
#   + il codice dopo la fork viene eseguito sia dal processo padre che dal processo figlio
# - un processo ha sempre un padre. Se il padre termina prima del figlio, il padre diventa
#   il padre del padre e così via fino ad arrivare al processo "Init" pid=1
#
#==========================================================================================

import os

#=== Local variables ======================================================================
op1 = 10
op2 = 20
ris = 0

#=== Local Functions ======================================================================

def child_process():
    global ris, op1, op2
    # op1 e op2 inizializzate con il valore assegnato dal padre prima della fork()
    # ris ha valore diverso rispetto al processo padre
    ris = op1 + op2
    print(f"\n[Figlio] - Il risultato della somma di op1+op2={ris}")
    print(f"[Figlio] - pid={os.getpid()}, pid padre={os.getppid()}")

def father_process(childpid):
    global ris, op1, op2
    ris = op1 * op2
    print(f"\n[Padre] - pid={os.getpid()}, pid padre={os.getppid()}, pid figlio={childpid}")
    print(f"[Padre] - Il risultato della moltiplicazione di op1*op2={ris}")

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
    print(f"[FINE] - Termine del processo con pid={os.getpid()}, ris={ris}, padre={os.getppid()}")

if __name__ == "__main__":
    main()
