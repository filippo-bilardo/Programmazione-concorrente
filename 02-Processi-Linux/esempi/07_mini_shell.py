#!/usr/bin/env python3
"""Mini shell interattiva"""
import os
import sys

def esegui_comando(cmd_line):
    """Esegue una linea di comando"""
    args = cmd_line.strip().split()
    
    if not args:
        return
    
    comando = args[0]
    
    # Comandi built-in
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
    
    # Comandi esterni
    pid = os.fork()
    
    if pid == 0:  # Child
        try:
            os.execvp(comando, args)
        except OSError:
            print(f"{comando}: comando non trovato")
            os._exit(127)
    else:  # Parent
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
        except EOFError:
            print("\nBye!")
            break

if __name__ == "__main__":
    main()
