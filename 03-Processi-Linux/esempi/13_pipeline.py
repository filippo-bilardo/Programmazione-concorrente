#!/usr/bin/env python3
"""Pipeline di processi comunicanti"""
import os
import sys

def stage1():
    """Primo stadio: genera numeri"""
    print("Stage 1: genero numeri 1-10", file=sys.stderr)
    for i in range(1, 11):
        print(i)
    os._exit(0)

def stage2():
    """Secondo stadio: moltiplica per 2"""
    print("Stage 2: moltiplico per 2", file=sys.stderr)
    for line in sys.stdin:
        num = int(line.strip())
        print(num * 2)
    os._exit(0)

def stage3():
    """Terzo stadio: somma totale"""
    print("Stage 3: calcolo somma", file=sys.stderr)
    total = 0
    for line in sys.stdin:
        total += int(line.strip())
    print(f"\nRisultato finale: {total}", file=sys.stderr)
    os._exit(0)

def create_pipeline():
    """Crea pipeline: stage1 | stage2 | stage3"""
    
    # Pipe 1: stage1 -> stage2
    r1, w1 = os.pipe()
    
    # Pipe 2: stage2 -> stage3
    r2, w2 = os.pipe()
    
    # Fork stage1
    pid1 = os.fork()
    if pid1 == 0:
        # Reindirizza stdout a pipe1
        os.close(r1)
        os.dup2(w1, 1)
        os.close(w1)
        os.close(r2)
        os.close(w2)
        stage1()
    
    # Fork stage2
    pid2 = os.fork()
    if pid2 == 0:
        # Reindirizza stdin da pipe1, stdout a pipe2
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
    
    # Parent chiude tutte le pipe e aspetta
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
