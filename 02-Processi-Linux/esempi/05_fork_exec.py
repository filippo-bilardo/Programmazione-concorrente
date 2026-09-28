#!/usr/bin/env python3
# =============================================================================
# File: 05_fork_exec.py
# Argomento: Esecuzione di comandi con fork() ed execvp()
# Scopo: Il figlio esegue un comando esterno; il padre ne attende la terminazione.
# Esecuzione: python3 -u 05_fork_exec.py
# Nota: Richiede Linux/Unix e i comandi ls, echo e python3 disponibili nel PATH.
# =============================================================================
"""Pattern fork() + exec()"""
import os
import sys

def esegui_comando(comando, args):
    """Esegue un comando in un processo separato"""
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            # execvp cerca il comando nel PATH e riceve gli argomenti in una lista.
            # Il primo elemento è argv[0]; non si invoca una shell intermedia.
            os.execvp(comando, [comando] + args)
        except OSError as e:
            print(f"Errore exec: {e}", file=sys.stderr)
            os._exit(1)
    else:  # Parent
        # waitpid(pid, 0) attende proprio questo figlio e ne raccoglie lo stato.
        _, status = os.waitpid(pid, 0)
        
        # WEXITSTATUS è significativo solo dopo una terminazione normale.
        if os.WIFEXITED(status):
            exit_code = os.WEXITSTATUS(status)
            return exit_code
        else:
            return -1  # Valore scelto dalla funzione per una fine non normale.

# Test
print("=== Esecuzione comandi ===\n")

print("1. Comando: ls -l")
esegui_comando("ls", ["-l"])

print("\n2. Comando: echo Hello World")
esegui_comando("echo", ["Hello", "World"])

print("\n3. Comando: python3 --version")
esegui_comando("python3", ["--version"])

print("\n=== Fine ===")
