#!/usr/bin/env python3
# =============================================================================
# File: 01_fork_base.py
# Argomento: Creazione di un processo con fork()
# Scopo: Distingue padre e figlio dal valore restituito da fork() e attende il figlio.
# Esecuzione: python3 -u 01_fork_base.py
# Nota: Richiede Linux/Unix. Le stampe dei due processi possono alternarsi.
# =============================================================================
"""Esempio base di fork()"""
import os
import time

print("=== Inizio programma ===")
print(f"PID iniziale: {os.getpid()}")

# fork() duplica il processo: entrambi riprendono dalla prossima istruzione.
# Il figlio riceve 0, il padre riceve il PID del figlio; la memoria è separata.
pid = os.fork()

if pid == 0:
    # Ramo del figlio: getpid() identifica sé stesso, getppid() il padre.
    print(f"\nCHILD:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID di mio padre: {os.getppid()}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Eseguo lavoro child...")
    time.sleep(2)
    print("  - Child terminato")
    # _exit() termina subito senza cleanup Python né flush delle stampe.
    # L’opzione -u rende visibile l’output anche con questa uscita immediata.
    os._exit(0)  # Codice 0: terminazione riuscita.
    
else:
    # Ramo del padre: l’ordine delle stampe dipende dallo scheduler.
    print(f"\nPARENT:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID del mio child: {pid}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Aspetto il child...")
    os.wait()  # Blocca il padre e raccoglie lo stato del figlio terminato.
    print("  - Parent terminato")
