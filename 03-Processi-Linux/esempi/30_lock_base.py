#!/usr/bin/env python3
"""Esempio base di Lock"""
from multiprocessing import Process, Lock, Value
import time

def increment_unsafe(counter, name):
    """Incrementa senza lock (UNSAFE)"""
    for _ in range(1000):
        # Race condition!
        temp = counter.value
        time.sleep(0.0001)  # Simula operazione
        counter.value = temp + 1

def increment_safe(counter, lock, name):
    """Incrementa con lock (SAFE)"""
    for _ in range(1000):
        with lock:
            temp = counter.value
            time.sleep(0.0001)
            counter.value = temp + 1

if __name__ == '__main__':
    # Test UNSAFE
    print("=== Test UNSAFE (race condition) ===")
    counter = Value('i', 0)
    
    p1 = Process(target=increment_unsafe, args=(counter, "P1"))
    p2 = Process(target=increment_unsafe, args=(counter, "P2"))
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Atteso: 2000\n")
    
    # Test SAFE
    print("=== Test SAFE (con lock) ===")
    counter = Value('i', 0)
    lock = Lock()
    
    p1 = Process(target=increment_safe, args=(counter, lock, "P1"))
    p2 = Process(target=increment_safe, args=(counter, lock, "P2"))
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Atteso: 2000")
