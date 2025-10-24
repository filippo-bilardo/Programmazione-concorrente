#!/usr/bin/env python3
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

if __name__ == '__main__':
    print("=== multiprocessing.Process ===\n")
    
    # Crea processi
    processes = []
    for i in range(3):
        p = Process(target=task, args=(i, i+1))
        processes.append(p)
        p.start()
        print(f"Main: avviato processo {i} (PID={p.pid})")
    
    # Attende tutti
    print("\nMain: aspetto terminazione...\n")
    for p in processes:
        p.join()
    
    print("\nMain: tutti i processi terminati")
