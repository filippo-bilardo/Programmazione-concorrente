#!/usr/bin/env python3
"""Estendere la classe Process"""
from multiprocessing import Process
import time
import os

class WorkerProcess(Process):
    """Custom Process class"""
    
    def __init__(self, task_id, iterations):
        super().__init__()
        self.task_id = task_id
        self.iterations = iterations
    
    def run(self):
        """Override del metodo run()"""
        print(f"Worker {self.task_id}: avviato (PID={os.getpid()})")
        
        for i in range(self.iterations):
            print(f"Worker {self.task_id}: iterazione {i+1}/{self.iterations}")
            time.sleep(1)
        
        print(f"Worker {self.task_id}: terminato")

if __name__ == '__main__':
    print("=== Custom Process Class ===\n")
    
    # Crea workers
    workers = [WorkerProcess(i, 3) for i in range(3)]
    
    # Avvia tutti
    for w in workers:
        w.start()
    
    # Aspetta tutti
    for w in workers:
        w.join()
    
    print("\nMain: fine")
