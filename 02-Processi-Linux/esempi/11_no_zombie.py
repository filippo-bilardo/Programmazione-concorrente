#!/usr/bin/env python3
"""Prevenzione zombie con signal handler"""
import os
import signal
import time

def sigchld_handler(signum, frame):
    """Handler per SIGCHLD - raccoglie child terminati"""
    while True:
        try:
            # WNOHANG = non blocca
            pid, status = os.waitpid(-1, os.WNOHANG)
            if pid == 0:
                break
            print(f"Handler: raccolto child {pid}")
        except ChildProcessError:
            break

# Installa handler
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
