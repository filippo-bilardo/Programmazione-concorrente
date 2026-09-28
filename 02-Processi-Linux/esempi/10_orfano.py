#!/usr/bin/env python3
# =============================================================================
# File: 10_orfano.py
# Argomento: Osservazione di un processo orfano
# Scopo: Il padre termina subito e il figlio mostra il proprio PPID dopo una pausa.
# Esecuzione: python3 -u 10_orfano.py
# Nota: Richiede Linux/Unix. Il nuovo padre può essere PID 1 o un subreaper.
# =============================================================================
"""Dimostrazione processo orfano"""
import os
import time

print("=== Creazione Orfano ===\n")

pid = os.fork()

if pid == 0:
    # Il figlio continua indipendentemente dal padre. Non c’è sincronizzazione:
    # il padre potrebbe essere già terminato anche prima di questa prima lettura.
    print(f"Child {os.getpid()}: PPID iniziale = {os.getppid()}")
    print("Child: aspetto che parent termini...")
    
    time.sleep(3)
    
    # Dopo la fine del padre il kernel riassegna il figlio a PID 1 oppure a un
    # subreaper (un processo incaricato di adottare discendenti orfani).
    # La stampa su init/systemd descrive il caso comune, non tutti gli ambienti.
    print(f"Child {os.getpid()}: nuovo PPID = {os.getppid()}")
    print("Child: sono stato adottato da init/systemd!")
    
    time.sleep(2)
    os._exit(0)
else:
    # Parent termina subito
    print(f"Parent {os.getpid()}: termino lasciando child {pid} orfano")
    os._exit(0)
