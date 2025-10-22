#!/usr/bin/env python3
"""Creazione di multipli processi figli"""
import os
import time

def crea_child(child_id):
    """Crea un processo figlio"""
    pid = os.fork()
    
    if pid == 0:  # Child
        print(f"Child {child_id}: PID={os.getpid()}, PPID={os.getppid()}")
        time.sleep(child_id)  # Dorme per child_id secondi
        print(f"Child {child_id}: terminato dopo {child_id}s")
        os._exit(child_id)  # Exit con codice = child_id
    
    return pid  # Parent ritorna PID del child

# Main
print("Parent: creo 3 processi figli")
children = []

for i in range(1, 4):
    pid = crea_child(i)
    children.append(pid)
    print(f"Parent: creato child {i} con PID {pid}")

print(f"\nParent: aspetto {len(children)} children...")

# Attende tutti i children
while children:
    pid, status = os.wait()
    exit_code = os.WEXITSTATUS(status)
    print(f"Parent: child {pid} terminato con exit code {exit_code}")
    children.remove(pid)

print("Parent: tutti i children terminati")
