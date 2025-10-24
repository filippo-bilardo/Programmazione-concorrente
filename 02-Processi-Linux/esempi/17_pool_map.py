#!/usr/bin/env python3
"""Pool di processi con map()"""
from multiprocessing import Pool
import time
import os

def elabora(n):
    """Elabora un numero"""
    pid = os.getpid()
    print(f"Worker {pid}: elaboro {n}")
    time.sleep(0.5)
    return n * n

if __name__ == '__main__':
    print("=== Pool.map() ===\n")
    
    # Dati da elaborare
    numeri = list(range(1, 11))
    
    # Crea pool con 3 worker
    with Pool(processes=3) as pool:
        print(f"Pool con {pool._processes} worker\n")
        
        # Map distribuisce automaticamente il lavoro
        risultati = pool.map(elabora, numeri)
    
    print(f"\nRisultati: {risultati}")
    print(f"Somma: {sum(risultati)}")
