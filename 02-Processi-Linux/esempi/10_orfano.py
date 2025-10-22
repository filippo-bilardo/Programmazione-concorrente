#!/usr/bin/env python3
"""Dimostrazione processo orfano"""
import os
import time

print("=== Creazione Orfano ===\n")

pid = os.fork()

if pid == 0:
    # Child
    print(f"Child {os.getpid()}: PPID iniziale = {os.getppid()}")
    print("Child: aspetto che parent termini...")
    
    time.sleep(3)
    
    # Ora il parent è terminato
    print(f"Child {os.getpid()}: nuovo PPID = {os.getppid()}")
    print("Child: sono stato adottato da init/systemd!")
    
    time.sleep(2)
    os._exit(0)
else:
    # Parent termina subito
    print(f"Parent {os.getpid()}: termino lasciando child {pid} orfano")
    os._exit(0)
