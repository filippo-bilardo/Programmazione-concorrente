#!/usr/bin/env python3
"""Esempio completo con Pool"""
from multiprocessing import Pool, cpu_count
import time
import os

def task_pesante(n):
    """Simula task computazionalmente intensivo"""
    pid = os.getpid()
    print(f"Worker {pid}: inizio task {n}")
    
    # Simula calcolo pesante
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
    
    # Parallelo con Pool
    print("Esecuzione PARALLELA:")
    start = time.time()
    with Pool(processes=4) as pool:
        results_par = pool.map(task_pesante, tasks)
    time_par = time.time() - start
    print(f"Tempo: {time_par:.2f}s\n")
    
    print(f"Speedup: {time_seq/time_par:.2f}x")
