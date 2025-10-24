#!/usr/bin/env python3
"""Sincronizzazione con Lock"""
from multiprocessing import Process, Lock
import time

def conta_senza_lock(worker_id):
    """Senza protezione (può avere race condition)"""
    for i in range(3):
        print(f"Worker {worker_id}: {i}")
        time.sleep(0.1)

def conta_con_lock(worker_id, lock):
    """Con Lock - accesso mutuamente esclusivo"""
    with lock:
        for i in range(3):
            print(f"Worker {worker_id}: {i}")
            time.sleep(0.1)

if __name__ == '__main__':
    # Test senza Lock
    print("=== SENZA Lock ===")
    p1 = Process(target=conta_senza_lock, args=(1,))
    p2 = Process(target=conta_senza_lock, args=(2,))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    time.sleep(1)
    
    # Test con Lock
    print("\n=== CON Lock ===")
    lock = Lock()
    p1 = Process(target=conta_con_lock, args=(1, lock))
    p2 = Process(target=conta_con_lock, args=(2, lock))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
