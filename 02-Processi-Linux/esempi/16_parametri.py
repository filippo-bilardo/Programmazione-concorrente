#!/usr/bin/env python3
# =============================================================================
# File: 16_parametri.py
# Argomento: Parametri e risultati dei processi
# Scopo: Passa gli argomenti con args e raccoglie i risultati tramite una Queue.
# Esecuzione: python3 16_parametri.py
# Nota: I messaggi arrivano in un ordine che dipende dall’esecuzione dei figli.
# =============================================================================
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
    
    # Un return non porterebbe il risultato al padre: la coda trasferisce la tupla
    # fra processi, serializzandola e ricostruendola nel destinatario.
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
    
    # Con questi pochi messaggi l’esempio attende prima di leggere. Per grandi
    # quantità di dati si deve consumare la coda prima dei join, evitando blocchi.
    for p in processes:
        p.join()
    
    # empty() non è un criterio generale affidabile con produttori ancora attivi.
    # Qui i figli sono già terminati; in generale si usano conteggi o sentinelle.
    print("\nRisultati:")
    while not q.empty():
        op, res = q.get()
        print(f"  {op}: {res}")
