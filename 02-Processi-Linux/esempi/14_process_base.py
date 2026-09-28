#!/usr/bin/env python3
# =============================================================================
# File: 14_process_base.py
# Argomento: Processi con multiprocessing.Process
# Scopo: Avvia tre attività indipendenti e ne attende il completamento con join().
# Esecuzione: python3 14_process_base.py
# Nota: La protezione __main__ evita nuovi avvii quando il modulo è importato.
# =============================================================================
"""Esempio base con multiprocessing.Process"""
import os
import time
from multiprocessing import Process

def task(task_id, duration):
    """Task eseguito dal processo"""
    pid = os.getpid()
    print(f"Task {task_id}: avviato (PID={pid})")
    time.sleep(duration)
    print(f"Task {task_id}: completato dopo {duration}s")

# Con avvio tramite spawn il figlio importa il modulo: questa guardia impedisce
# che l’importazione avvii ricorsivamente altri processi.
if __name__ == '__main__':
    print("=== multiprocessing.Process ===\n")
    
    # Crea processi
    processes = []
    for i in range(3):
        # target è la funzione da eseguire; args contiene i suoi argomenti.
        # Costruire Process non avvia ancora il lavoro.
        p = Process(target=task, args=(i, i+1))
        processes.append(p)
        p.start()
        print(f"Main: avviato processo {i} (PID={p.pid})")
    
    # Tutti i processi sono già partiti: i join non rendono sequenziali i task.
    print("\nMain: aspetto terminazione...\n")
    for p in processes:
        p.join()
    
    print("\nMain: tutti i processi terminati")
