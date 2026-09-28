#!/usr/bin/env python3
# =============================================================================
# File: 09_zombie.py
# Argomento: Osservazione di un processo zombie
# Scopo: Ritarda wait() per lasciare osservabile lo stato del figlio già terminato.
# Esecuzione: python3 -u 09_zombie.py
# Nota: Richiede Linux/Unix; durante la pausa si può controllare il figlio con ps.
# =============================================================================
"""Dimostrazione processo zombie"""
import os
import time

print("=== Creazione Zombie ===\n")

pid = os.fork()

if pid == 0:
    # Child termina subito
    print(f"Child {os.getpid()}: termino")
    os._exit(0)
else:
    # Il figlio terminato non esegue più codice: il kernel conserva PID e stato
    # finché il padre non li raccoglie. Durante questa attesa è uno zombie.
    print(f"Parent: child {pid} diventerà zombie")
    print(f"Parent: verifica con 'ps aux | grep {pid}'")
    
    # Dorme lasciando il child in stato zombie
    print("Parent: aspetto 10 secondi...")
    time.sleep(10)
    
    # wait() raccoglie lo stato già disponibile e libera la voce residua del figlio.
    print("Parent: rimuovo zombie con wait()")
    os.wait()
    print("Parent: zombie rimosso")
