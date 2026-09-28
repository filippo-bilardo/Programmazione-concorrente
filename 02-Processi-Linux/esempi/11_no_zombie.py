#!/usr/bin/env python3
# =============================================================================
# File: 11_no_zombie.py
# Argomento: Raccolta dei figli tramite SIGCHLD
# Scopo: Un gestore di segnale raccoglie i figli terminati mentre il padre prosegue.
# Esecuzione: python3 -u 11_no_zombie.py
# Nota: Richiede Linux/Unix. waitpid() usa WNOHANG per evitare attese nel gestore.
# =============================================================================
"""Prevenzione zombie con signal handler"""
import os
import signal
import time

def sigchld_handler(signum, frame):
    """Handler per SIGCHLD - raccoglie child terminati"""
    # Più terminazioni possono produrre una sola notifica SIGCHLD:
    # il ciclo raccoglie tutti gli stati disponibili, non un solo figlio.
    while True:
        try:
            # -1 seleziona qualsiasi figlio; WNOHANG evita di bloccare il padre.
            pid, status = os.waitpid(-1, os.WNOHANG)
            if pid == 0:  # Esistono figli, ma nessuno ha uno stato pronto.
                break
            print(f"Handler: raccolto child {pid}")
        except ChildProcessError:  # Non restano figli da attendere.
            break

# Installa il gestore prima dei fork, così intercetta anche terminazioni rapide.
signal.signal(signal.SIGCHLD, sigchld_handler)

print("=== Prevenzione Zombie con SIGCHLD ===\n")

# Crea multipli children
for i in range(5):
    pid = os.fork()
    
    if pid == 0:
        # Child dorme e termina
        time.sleep(i + 1)
        print(f"Child {os.getpid()}: termino")
        os._exit(0)
    else:
        print(f"Parent: creato child {pid}")

# Parent continua a lavorare
print("\nParent: continuo a lavorare...")
print("Parent: i child verranno raccolti automaticamente")

time.sleep(10)
print("\nParent: fine")
