#!/usr/bin/env python3
# =============================================================================
# File: 02_fork_multipli.py
# Argomento: Creazione e raccolta di più processi figli
# Scopo: Avvia tre figli e recupera PID e codice di uscita di ciascuno con wait().
# Esecuzione: python3 -u 02_fork_multipli.py
# Nota: Richiede Linux/Unix. I figli lavorano contemporaneamente.
# =============================================================================
"""Creazione di multipli processi figli"""
import os
import time

def crea_child(child_id):
    """Crea un processo figlio"""
    # Solo il padre torna al ciclo chiamante: il figlio termina con _exit().
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

# wait() raccoglie un figlio alla volta, non necessariamente in ordine di creazione.
while children:
    pid, status = os.wait()
    # status è un valore codificato, non il codice di uscita diretto.
    # Qui si assume uscita normale; in generale va prima verificato WIFEXITED.
    exit_code = os.WEXITSTATUS(status)
    print(f"Parent: child {pid} terminato con exit code {exit_code}")
    # Rimuove il PID raccolto fino a esaurire l’elenco dei figli da attendere.
    children.remove(pid)

print("Parent: tutti i children terminati")
