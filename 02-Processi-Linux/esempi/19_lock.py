#!/usr/bin/env python3
# =============================================================================
# File: 19_lock.py
# Argomento: Mutua esclusione sulle stampe
# Scopo: Confronta stampe libere e gruppi di stampe protetti da un Lock condiviso.
# Esecuzione: python3 19_lock.py
# Nota: Il lock protegge tutto il ciclo di un worker, comprese le sue pause.
# =============================================================================
"""Sincronizzazione con Lock"""
from multiprocessing import Process, Lock
import time

def conta_senza_lock(worker_id):
    """Senza protezione (può avere race condition)"""
    # I contatori i sono locali: qui si osserva l’alternanza delle stampe,
    # non la perdita di aggiornamenti di un contatore condiviso.
    for i in range(3):
        print(f"Worker {worker_id}: {i}")
        time.sleep(0.1)

def conta_con_lock(worker_id, lock):
    """Con Lock - accesso mutuamente esclusivo"""
    # Il context manager acquisisce il lock e lo rilascia anche se avviene
    # un’eccezione. Solo un worker alla volta esegue l’intero blocco.
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
    # Entrambi i figli ricevono lo stesso lock per coordinare le stampe.
    lock = Lock()
    p1 = Process(target=conta_con_lock, args=(1, lock))
    p2 = Process(target=conta_con_lock, args=(2, lock))
    p1.start()
    p2.start()
    p1.join()
    p2.join()
