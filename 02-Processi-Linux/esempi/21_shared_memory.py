#!/usr/bin/env python3
# =============================================================================
# File: 21_shared_memory.py
# Argomento: Dati condivisi con Value e Array
# Scopo: Tre processi aggiornano un contatore comune e tre celle distinte.
# Esecuzione: python3 21_shared_memory.py
# Nota: Risultati attesi: contatore 300, array [100, 100, 100], somma 300.
# =============================================================================
"""Memoria condivisa con Value e Array"""
from multiprocessing import Process, Value, Array
import time

def incrementa(counter, arr, worker_id):
    """Incrementa counter e array condivisi"""
    for i in range(100):
        # += comprende lettura, somma e scrittura: serve un lock sull’intera
        # operazione, oltre alla protezione dei singoli accessi offerta da Value.
        with counter.get_lock():
            counter.value += 1
        
        # Ogni worker aggiorna un indice diverso, quindi non perde incrementi.
        # Se più worker scrivessero la stessa cella servirebbe un lock sul +=.
        arr[worker_id] += 1
        
        time.sleep(0.01)

if __name__ == '__main__':
    # Il codice di tipo i indica un intero C in memoria condivisa.
    counter = Value('i', 0)
    
    # Array condiviso
    arr = Array('i', [0] * 3)
    
    # Crea processi
    processes = [
        Process(target=incrementa, args=(counter, arr, i))
        for i in range(3)
    ]
    
    # Avvia
    for p in processes:
        p.start()
    
    # Legge i valori finali solo dopo che tutti i processi hanno finito.
    for p in processes:
        p.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Array finale: {list(arr)}")
    print(f"Somma array: {sum(arr)}")
