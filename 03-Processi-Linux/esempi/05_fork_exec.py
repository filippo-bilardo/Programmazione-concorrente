#!/usr/bin/env python3
"""Pattern fork() + exec()"""
import os
import sys

def esegui_comando(comando, args):
    """Esegue un comando in un processo separato"""
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            # execvp cerca il comando in $PATH
            os.execvp(comando, [comando] + args)
        except OSError as e:
            print(f"Errore exec: {e}", file=sys.stderr)
            os._exit(1)
    else:  # Parent
        # Aspetta terminazione child
        _, status = os.waitpid(pid, 0)
        
        if os.WIFEXITED(status):
            exit_code = os.WEXITSTATUS(status)
            return exit_code
        else:
            return -1

# Test
print("=== Esecuzione comandi ===\n")

print("1. Comando: ls -l")
esegui_comando("ls", ["-l"])

print("\n2. Comando: echo Hello World")
esegui_comando("echo", ["Hello", "World"])

print("\n3. Comando: python3 --version")
esegui_comando("python3", ["--version"])

print("\n=== Fine ===")
