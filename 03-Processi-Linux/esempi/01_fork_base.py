#!/usr/bin/env python3
"""Esempio base di fork()"""
import os
import time

print("=== Inizio programma ===")
print(f"PID iniziale: {os.getpid()}")

# Fork del processo
pid = os.fork()

if pid == 0:
    # CHILD PROCESS
    print(f"\nCHILD:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID di mio padre: {os.getppid()}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Eseguo lavoro child...")
    time.sleep(2)
    print("  - Child terminato")
    os._exit(0)  # Importante: usa _exit() nel child!
    
else:
    # PARENT PROCESS
    print(f"\nPARENT:")
    print(f"  - Il mio PID: {os.getpid()}")
    print(f"  - PID del mio child: {pid}")
    print(f"  - fork() ha ritornato: {pid}")
    print("  - Aspetto il child...")
    os.wait()  # Attende terminazione child
    print("  - Parent terminato")
