#!/usr/bin/env python3
"""Esempio di wait() con analisi status"""
import os
import signal
import time

def test_exit_normale():
    """Test terminazione normale"""
    print("\n=== Test 1: Terminazione normale ===")
    
    pid = os.fork()
    if pid == 0:
        print("Child: termino con exit code 42")
        os._exit(42)
    else:
        _, status = os.wait()
        print(f"Parent: child terminato")
        
        if os.WIFEXITED(status):
            code = os.WEXITSTATUS(status)
            print(f"  Exit code: {code}")

def test_segnale():
    """Test terminazione da segnale"""
    print("\n=== Test 2: Terminazione da segnale ===")
    
    pid = os.fork()
    if pid == 0:
        print("Child: aspetto segnale...")
        time.sleep(10)  # Non arriverà mai qui
        os._exit(0)
    else:
        time.sleep(1)
        print(f"Parent: invio SIGTERM a {pid}")
        os.kill(pid, signal.SIGTERM)
        
        _, status = os.wait()
        print(f"Parent: child terminato")
        
        if os.WIFSIGNALED(status):
            sig = os.WTERMSIG(status)
            print(f"  Terminato da segnale: {sig}")

def test_wait_non_bloccante():
    """Test wait non bloccante"""
    print("\n=== Test 3: Wait non bloccante ===")
    
    pid = os.fork()
    if pid == 0:
        time.sleep(3)
        os._exit(0)
    else:
        # Prova subito (child ancora in esecuzione)
        result, status = os.waitpid(pid, os.WNOHANG)
        
        if result == 0:
            print("Parent: child ancora in esecuzione")
            
        # Ora aspetta bloccando
        print("Parent: aspetto bloccante...")
        os.waitpid(pid, 0)
        print("Parent: child terminato")

# Esegue i test
test_exit_normale()
test_segnale()
test_wait_non_bloccante()
