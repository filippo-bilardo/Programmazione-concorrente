#!/usr/bin/env python3
# =============================================================================
# File: 18_pool_completo.py
# Argomento: Confronto fra elaborazione sequenziale e parallela
# Scopo: Misura la stessa serie di calcoli nel padre e in un pool di quattro worker.
# Esecuzione: python3 18_pool_completo.py
# Nota: Il rapporto fra i tempi dipende dal carico, dalle CPU e dai costi del pool.
# =============================================================================
"""Esempio completo con Pool"""
from multiprocessing import Pool, cpu_count
import time
import os

def task_pesante(n):
    """Simula task computazionalmente intensivo"""
    pid = os.getpid()
    print(f"Worker {pid}: inizio task {n}")
    
    # Il calcolo usa la CPU; la pausa successiva aggiunge anche tempo di attesa.
    total = sum(i*i for i in range(n * 100000))
    
    time.sleep(0.1)
    print(f"Worker {pid}: completato task {n}")
    return (n, total)

if __name__ == '__main__':
    print(f"=== Pool di Processi ===")
    print(f"CPU disponibili: {cpu_count()}\n")
    
    tasks = [1, 2, 3, 4, 5, 6, 7, 8]
    
    # Sequenziale (per confronto)
    print("Esecuzione SEQUENZIALE:")
    start = time.time()
    results_seq = [task_pesante(t) for t in tasks]
    time_seq = time.time() - start
    print(f"Tempo: {time_seq:.2f}s\n")
    
    # Il tempo parallelo include anche creazione e chiusura dei processi.
    # map() restituisce le coppie (n, total) nello stesso ordine di tasks.
    print("Esecuzione PARALLELA:")
    start = time.time()
    with Pool(processes=4) as pool:
        results_par = pool.map(task_pesante, tasks)
    time_par = time.time() - start
    print(f"Tempo: {time_par:.2f}s\n")
    
    # Un rapporto > 1 indica un vantaggio del parallelo; non è garantito.
    print(f"Speedup: {time_seq/time_par:.2f}x")
