#!/usr/bin/env python3
# =============================================================================
# File: 30_lock_base.py
# Argomento: Race condition e protezione di un contatore
# Scopo: Confronta duemila incrementi concorrenti senza e con un Lock esplicito.
# Esecuzione: python3 30_lock_base.py
# Nota: Senza protezione alcuni incrementi possono perdersi; con il lock sono 2000.
# =============================================================================
"""Esempio base di Lock"""
from multiprocessing import Process, Lock, Value
import time

def increment_unsafe(counter, name):
    """Incrementa senza lock (UNSAFE)"""
    for _ in range(1000):
        # Due processi possono leggere lo stesso valore e poi sovrascriversi:
        # uno degli incrementi si perde. La pausa rende più probabile il conflitto.
        temp = counter.value
        time.sleep(0.0001)  # Simula operazione
        counter.value = temp + 1

def increment_safe(counter, lock, name):
    """Incrementa con lock (SAFE)"""
    for _ in range(1000):
        # Protegge l’intera sequenza lettura-modifica-scrittura, non solo
        # il singolo accesso; il lock viene rilasciato all’uscita dal with.
        with lock:
            temp = counter.value
            time.sleep(0.0001)
            counter.value = temp + 1

if __name__ == '__main__':
    # Test UNSAFE
    print("=== Test UNSAFE (race condition) ===")
    # Value condivide il dato e sincronizza i singoli accessi; da solo non
    # rende atomica la sequenza che legge e poi scrive il valore aggiornato.
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
    # Value condivide il dato e sincronizza i singoli accessi; da solo non
    # rende atomica la sequenza che legge e poi scrive il valore aggiornato.
    counter = Value('i', 0)
    # Il medesimo lock deve essere passato a tutti i processi che aggiornano il dato.
    lock = Lock()
    
    p1 = Process(target=increment_safe, args=(counter, lock, "P1"))
    p2 = Process(target=increment_safe, args=(counter, lock, "P2"))
    
    p1.start()
    p2.start()
    p1.join()
    p2.join()
    
    print(f"Counter finale: {counter.value}")
    print(f"Atteso: 2000")
