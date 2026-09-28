#!/usr/bin/env python3
# =============================================================================
# File: 08_wait_status.py
# Argomento: Interpretazione dello stato di terminazione
# Scopo: Confronta uscita normale, terminazione tramite segnale e attesa non bloccante.
# Esecuzione: python3 -u 08_wait_status.py
# Nota: Richiede Linux/Unix. I tre casi dimostrativi vengono eseguiti in sequenza.
# =============================================================================
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
        
        # Prima riconosce il tipo di terminazione, poi estrae il codice 42.
        if os.WIFEXITED(status):
            code = os.WEXITSTATUS(status)
            print(f"  Exit code: {code}")

def test_segnale():
    """Test terminazione da segnale"""
    print("\n=== Test 2: Terminazione da segnale ===")
    
    pid = os.fork()
    if pid == 0:
        print("Child: aspetto segnale...")
        time.sleep(10)  # Il SIGTERM del padre interrompe normalmente questa pausa.
        os._exit(0)
    else:
        time.sleep(1)
        print(f"Parent: invio SIGTERM a {pid}")
        os.kill(pid, signal.SIGTERM)
        
        _, status = os.wait()
        print(f"Parent: child terminato")
        
        # Per una fine causata da segnale si legge WTERMSIG, non WEXITSTATUS.
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
        # WNOHANG ritorna subito: PID 0 significa che non c’è uno stato da raccogliere.
        # Qui si presume che il figlio sia ancora nella pausa di tre secondi.
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
