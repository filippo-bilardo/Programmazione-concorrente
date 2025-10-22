#!/usr/bin/env python3
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
    # Parent NON fa wait()
    print(f"Parent: child {pid} diventerà zombie")
    print(f"Parent: verifica con 'ps aux | grep {pid}'")
    
    # Dorme lasciando il child in stato zombie
    print("Parent: aspetto 10 secondi...")
    time.sleep(10)
    
    # Ora rimuove lo zombie
    print("Parent: rimuovo zombie con wait()")
    os.wait()
    print("Parent: zombie rimosso")
