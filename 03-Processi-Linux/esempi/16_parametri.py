#!/usr/bin/env python3
"""Passaggio parametri ai processi"""
from multiprocessing import Process
import os

def calcola(operazione, a, b, risultato_queue=None):
    """Esegue operazione matematica"""
    pid = os.getpid()
    
    if operazione == 'add':
        res = a + b
    elif operazione == 'mul':
        res = a * b
    elif operazione == 'pow':
        res = a ** b
    else:
        res = None
    
    print(f"Processo {pid}: {operazione}({a}, {b}) = {res}")
    
    if risultato_queue:
        risultato_queue.put((operazione, res))

if __name__ == '__main__':
    from multiprocessing import Queue
    
    # Queue per risultati
    q = Queue()
    
    # Operazioni
    operazioni = [
        ('add', 10, 5),
        ('mul', 10, 5),
        ('pow', 2, 10)
    ]
    
    processes = []
    for op, a, b in operazioni:
        p = Process(target=calcola, args=(op, a, b, q))
        processes.append(p)
        p.start()
    
    # Attende
    for p in processes:
        p.join()
    
    # Legge risultati
    print("\nRisultati:")
    while not q.empty():
        op, res = q.get()
        print(f"  {op}: {res}")
