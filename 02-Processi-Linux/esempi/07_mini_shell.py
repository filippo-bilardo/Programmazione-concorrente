#!/usr/bin/env python3
# =============================================================================
# File: 07_mini_shell.py
# Argomento: Mini shell interattiva
# Scopo: Gestisce cd, pwd ed exit; esegue gli altri comandi in processi figli.
# Esecuzione: python3 -u 07_mini_shell.py
# Nota: Richiede Linux/Unix. La separazione degli argomenti avviene sugli spazi.
# =============================================================================
"""Mini shell interattiva"""
import os
import sys

def esegui_comando(cmd_line):
    """Esegue una linea di comando"""
    # Parser minimo: non interpreta virgolette, pipe, redirezioni o wildcard.
    args = cmd_line.strip().split()
    
    if not args:
        return
    
    comando = args[0]
    
    # I built-in agiscono nel processo della shell: cd deve cambiarne la directory.
    # Un chdir() eseguito in un figlio non cambierebbe la directory del padre.
    if comando == "cd":
        try:
            os.chdir(args[1] if len(args) > 1 else os.environ['HOME'])
        except Exception as e:
            print(f"cd: {e}")
        return
    
    elif comando == "pwd":
        print(os.getcwd())
        return
    
    elif comando == "exit":
        sys.exit(0)
    
    # Il figlio sostituisce il proprio programma; il padre resta la shell.
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            os.execvp(comando, args)
        except OSError:
            print(f"{comando}: comando non trovato")
            os._exit(127)
    else:  # Parent
        # Attesa in primo piano: il prossimo prompt compare dopo la fine del comando.
        os.waitpid(pid, 0)

def main():
    """Loop principale della shell"""
    print("=== Mini Shell ===")
    print("Comandi: cd, pwd, exit, o qualsiasi comando esterno\n")
    
    while True:
        try:
            # Prompt
            cwd = os.getcwd()
            prompt = f"{cwd}$ "
            cmd_line = input(prompt)
            
            # Esegue comando
            esegui_comando(cmd_line)
            
        except KeyboardInterrupt:
            print("\nUsa 'exit' per uscire")
        # Ctrl+D su input vuoto segnala la fine dell’input.
        except EOFError:
            print("\nBye!")
            break

if __name__ == "__main__":
    main()
