#!/usr/bin/env python3
# =============================================================================
# File: 17_pool_map.py
# Argomento: Distribuzione del lavoro con Pool.map()
# Scopo: Tre worker calcolano i quadrati dei numeri da 1 a 10.
# Esecuzione: python3 17_pool_map.py
# Nota: map() attende tutti i risultati e conserva l’ordine dei dati di ingresso.
# =============================================================================
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
    
    # Il pool riutilizza tre processi per tutti i dieci valori; with ne gestisce
    # l’uscita e libera le risorse quando il blocco termina.
    with Pool(processes=3) as pool:
        print(f"Pool con {pool._processes} worker\n")
        
        # map() distribuisce il lavoro e blocca fino al completamento.
        # Le stampe possono alternarsi, ma la lista segue l’ordine di numeri.
        risultati = pool.map(elabora, numeri)
    
    print(f"\nRisultati: {risultati}")
    print(f"Somma: {sum(risultati)}")
