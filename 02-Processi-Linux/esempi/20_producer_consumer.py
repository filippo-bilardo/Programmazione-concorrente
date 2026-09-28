#!/usr/bin/env python3
# =============================================================================
# File: 20_producer_consumer.py
# Argomento: Produttori e consumatori con Queue
# Scopo: Due produttori inviano dieci elementi a tre consumatori concorrenti.
# Esecuzione: python3 20_producer_consumer.py
# Nota: I consumatori escono dopo un timeout; il padre limita la loro attesa.
# =============================================================================
"""Pattern Producer-Consumer con Queue"""
from multiprocessing import Process, Queue
import time
import random

def producer(queue, producer_id, n_items):
    """Produce items e li mette nella queue"""
    for i in range(n_items):
        item = f"Item-{producer_id}-{i}"
        print(f"Producer {producer_id}: prodotto {item}")
        # La coda trasferisce ogni elemento a uno dei consumatori.
        queue.put(item)
        time.sleep(random.uniform(0.1, 0.5))
    
    print(f"Producer {producer_id}: terminato")

def consumer(queue, consumer_id):
    """Consuma items dalla queue"""
    while True:
        try:
            # Attende un elemento per al massimo due secondi; una coda vuota
            # allo scadere genera queue.Empty. Non prova che i produttori siano finiti.
            item = queue.get(timeout=2)
            print(f"  Consumer {consumer_id}: consumato {item}")
            time.sleep(random.uniform(0.1, 0.3))
        # L’except generico intercetta il timeout ma anche ogni altra eccezione.
        except:
            break
    
    print(f"  Consumer {consumer_id}: terminato")

if __name__ == '__main__':
    print("=== Producer-Consumer ===\n")
    
    queue = Queue()
    
    # Crea producers
    producers = [
        Process(target=producer, args=(queue, i, 5))
        for i in range(2)
    ]
    
    # Crea consumers
    consumers = [
        Process(target=consumer, args=(queue, i))
        for i in range(3)
    ]
    
    # Avvia tutti
    for p in producers + consumers:
        p.start()
    
    # Aspetta producers
    for p in producers:
        p.join()
    
    # Il timeout di join limita l’attesa del padre, senza fermare il consumatore.
    # is_alive() controlla se serve la terminazione forzata prevista dall’esempio.
    for c in consumers:
        c.join(timeout=3)
        if c.is_alive():
            c.terminate()
    
    print("\nFine")
