#!/usr/bin/env python3
"""Memoria condivisa con Value e Array"""
from multiprocessing import Process, Value, Array
import time

def incrementa(counter, arr, worker_id):
    """Incrementa counter e array condivisi"""
    for i in range(100):
        # Counter condiviso
        with counter.get_lock():
            counter.value += 1
        
        # Array condiviso
        arr[worker_id] += 1
        
        time.sleep(0.01)

if __name__ == '__main__':
    # Counter condiviso
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
    
    # Aspetta
    for p in processes:
        p.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Array finale: {list(arr)}")
    print(f"Somma array: {sum(arr)}")
