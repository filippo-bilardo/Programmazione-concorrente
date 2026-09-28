#!/usr/bin/env python3
# =============================================================================
# File: 03_gerarchia.py
# Argomento: Gerarchia padre, figli e nipote
# Scopo: Crea due figli; il primo genera a sua volta un figlio, formando un albero.
# Esecuzione: python3 -u 03_gerarchia.py
# Nota: Richiede Linux/Unix. Ogni padre attende i propri figli diretti.
# =============================================================================
"""Creazione di una gerarchia di processi"""
import os
import time

def stampa_info(label):
    """Stampa informazioni sul processo"""
    print(f"{label}: PID={os.getpid()}, PPID={os.getppid()}")

# Processo originale
stampa_info("Originale")

# Albero risultante: Originale -> Child 1 -> Grandchild
#                            -> Child 2
# Prima fork - crea Child 1
pid1 = os.fork()

if pid1 == 0:
    # Child 1
    stampa_info("  Child 1")
    
    # Child 1 fa fork - crea Grandchild
    pid_gc = os.fork()
    
    if pid_gc == 0:
        # Grandchild
        stampa_info("    Grandchild")
        time.sleep(1)
        os._exit(0)
    else:
        # Child 1 aspetta Grandchild
        os.wait()
        print("  Child 1: grandchild terminato")
        os._exit(0)

# Qui arriva solo il processo originale: gli altri rami terminano con _exit().
# Originale fa seconda fork - crea Child 2
pid2 = os.fork()

if pid2 == 0:
    # Child 2
    stampa_info("  Child 2")
    time.sleep(1)
    os._exit(0)

# wait() può raccogliere solo figli diretti: il nipote è raccolto da Child 1.
# Originale aspetta entrambi i children
if pid1 > 0 and pid2 > 0:
    os.wait()
    os.wait()
    print("Originale: tutti i children terminati")
