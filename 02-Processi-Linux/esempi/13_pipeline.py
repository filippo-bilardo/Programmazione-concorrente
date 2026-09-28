#!/usr/bin/env python3
# =============================================================================
# File: 13_pipeline.py
# Argomento: Pipeline di tre processi collegati da pipe
# Scopo: Genera i numeri da 1 a 10, li raddoppia e ne calcola la somma attesa: 110.
# Esecuzione: python3 -u 13_pipeline.py
# Nota: Richiede Linux/Unix. Usare -u: os._exit() non scarica i buffer di Python.
# =============================================================================
"""Pipeline di processi comunicanti"""
import os
import sys

# I dati viaggiano su stdout; le spiegazioni vanno su stderr per non entrare
# nelle pipe. Avviare con python3 -u: _exit() non svuota i buffer di stdout.
def stage1():
    """Primo stadio: genera numeri"""
    print("Stage 1: genero numeri 1-10", file=sys.stderr)
    for i in range(1, 11):
        print(i)
    os._exit(0)

def stage2():
    """Secondo stadio: moltiplica per 2"""
    print("Stage 2: moltiplico per 2", file=sys.stderr)
    # La lettura termina a EOF, quando tutti i descrittori di scrittura sono chiusi.
    for line in sys.stdin:
        num = int(line.strip())
        print(num * 2)
    os._exit(0)

def stage3():
    """Terzo stadio: somma totale"""
    print("Stage 3: calcolo somma", file=sys.stderr)
    total = 0
    # La lettura termina a EOF, quando tutti i descrittori di scrittura sono chiusi.
    for line in sys.stdin:
        total += int(line.strip())
    print(f"\nRisultato finale: {total}", file=sys.stderr)
    os._exit(0)

def create_pipeline():
    """Crea pipeline: stage1 | stage2 | stage3"""
    
    # os.pipe() restituisce due descrittori: lettura (r) e scrittura (w).
    # I figli ereditano i descrittori aperti al momento del fork.
    # Pipe 1: stage1 -> stage2
    r1, w1 = os.pipe()
    
    # Pipe 2: stage2 -> stage3
    r2, w2 = os.pipe()
    
    # Fork stage1
    pid1 = os.fork()
    if pid1 == 0:
        # dup2() collega il descrittore standard 1 (stdout) alla pipe.
        # La copia w1 può poi essere chiusa: il descrittore 1 resta valido.
        os.close(r1)
        os.dup2(w1, 1)
        os.close(w1)
        os.close(r2)
        os.close(w2)
        stage1()
    
    # Fork stage2
    pid2 = os.fork()
    if pid2 == 0:
        # Il descrittore 0 (stdin) riceve da stage1; il descrittore 1 invia a stage3.
        os.close(w1)
        os.dup2(r1, 0)
        os.close(r1)
        os.close(r2)
        os.dup2(w2, 1)
        os.close(w2)
        stage2()
    
    # Fork stage3
    pid3 = os.fork()
    if pid3 == 0:
        # Reindirizza stdin da pipe2
        os.close(r1)
        os.close(w1)
        os.close(w2)
        os.dup2(r2, 0)
        os.close(r2)
        stage3()
    
    # Chiudere le copie inutilizzate è essenziale: una scrittura lasciata aperta
    # nel padre impedirebbe ai lettori di ricevere EOF e terminare.
    os.close(r1)
    os.close(w1)
    os.close(r2)
    os.close(w2)
    
    # Attende tutti gli stage
    os.waitpid(pid1, 0)
    os.waitpid(pid2, 0)
    os.waitpid(pid3, 0)
    
    print("\nPipeline completata", file=sys.stderr)

# Main
if __name__ == "__main__":
    print("=== Pipeline di Processi ===\n", file=sys.stderr)
    create_pipeline()
