#!/usr/bin/env python3
# =============================================================================
# File: 15_process_class.py
# Argomento: Definizione di una sottoclasse di Process
# Scopo: Incapsula parametri e lavoro di un processo nella classe WorkerProcess.
# Esecuzione: python3 15_process_class.py
# Nota: start() avvia il processo separato che esegue il metodo run().
# =============================================================================
"""Estendere la classe Process"""
from multiprocessing import Process
import time
import os

class WorkerProcess(Process):
    """Custom Process class"""
    
    def __init__(self, task_id, iterations):
        # Inizializza la parte Process prima di aggiungere i parametri del worker.
        super().__init__()
        self.task_id = task_id
        self.iterations = iterations
    
    # start() esegue questo metodo nel figlio; chiamare run() direttamente
    # sarebbe una normale chiamata nel processo corrente.
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
    
    # Avvia ogni worker prima delle attese per consentire il lavoro concorrente.
    for w in workers:
        w.start()
    
    # join() attende la fine del processo associato senza restituire un risultato.
    for w in workers:
        w.join()
    
    print("\nMain: fine")
