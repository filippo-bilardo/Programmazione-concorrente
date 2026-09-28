#!/usr/bin/env python3
# =============================================================================
# File: 12_process_manager.py
# Argomento: Gestore di processi worker
# Scopo: Avvia worker, ne registra i PID, invia SIGTERM e raccoglie gli stati finali.
# Esecuzione: python3 -u 12_process_manager.py
# Nota: Richiede Linux/Unix. Gestisce anche Ctrl+C nel processo principale.
# =============================================================================
"""Gestore semplice di processi"""
import os
import sys
import time
import signal

class ProcessManager:
    """Gestisce un pool di processi worker"""
    
    def __init__(self):
        self.workers = {}  # pid -> info
        # Dopo fork ogni processo possiede una copia indipendente di questo flag.
        self.running = True
    
    def worker_func(self, worker_id):
        """Funzione eseguita dai worker"""
        print(f"Worker {worker_id} (PID {os.getpid()}): avviato")
        
        # Simula lavoro
        for i in range(10):
            if not self.running:
                break
            print(f"Worker {worker_id}: iterazione {i+1}/10")
            time.sleep(2)
        
        print(f"Worker {worker_id}: terminato")
        os._exit(0)
    
    def start_worker(self, worker_id):
        """Avvia un worker"""
        pid = os.fork()
        
        if pid == 0:  # Child
            self.worker_func(worker_id)
        else:  # Parent
            # Solo il manager registra PID e istante di avvio nel proprio dizionario.
            self.workers[pid] = {
                'id': worker_id,
                'start_time': time.time()
            }
            print(f"Manager: avviato worker {worker_id} (PID {pid})")
    
    def stop_worker(self, pid):
        """Ferma un worker specifico"""
        if pid in self.workers:
            try:
                # SIGTERM termina il worker con l’azione predefinita del segnale.
                # Inviare il segnale non raccoglie lo stato: servirà anche wait().
                os.kill(pid, signal.SIGTERM)
                print(f"Manager: inviato SIGTERM a {pid}")
            except OSError as e:
                print(f"Manager: errore stop worker: {e}")
    
    def stop_all(self):
        """Ferma tutti i worker"""
        print("\nManager: fermo tutti i worker...")
        # Cambia solo il flag del padre: i worker vengono fermati dai segnali sotto.
        self.running = False
        
        for pid in list(self.workers.keys()):
            self.stop_worker(pid)
    
    def wait_workers(self):
        """Attende terminazione worker"""
        while self.workers:
            try:
                # Attende un figlio qualsiasi e associa il suo stato ai dati salvati.
                pid, status = os.wait()
                
                if pid in self.workers:
                    worker_info = self.workers.pop(pid)
                    duration = time.time() - worker_info['start_time']
                    
                    print(f"Manager: worker {worker_info['id']} "
                          f"(PID {pid}) terminato dopo {duration:.1f}s")
                    
                    if os.WIFEXITED(status):
                        code = os.WEXITSTATUS(status)
                        print(f"  Exit code: {code}")
                    elif os.WIFSIGNALED(status):
                        sig = os.WTERMSIG(status)
                        print(f"  Terminato da segnale: {sig}")
                        
            except ChildProcessError:
                break
    
    def run(self, num_workers=3, duration=10):
        """Esegue il manager"""
        print(f"=== Process Manager ===")
        print(f"Manager PID: {os.getpid()}\n")
        
        # Avvia worker
        for i in range(num_workers):
            self.start_worker(i)
        
        # Lascia lavorare
        print(f"\nManager: lascio lavorare per {duration}s...\n")
        time.sleep(duration)
        
        # Ferma tutti
        self.stop_all()
        
        # Attende terminazione
        self.wait_workers()
        
        print("\nManager: tutti i worker terminati")

# Main
if __name__ == "__main__":
    manager = ProcessManager()
    
    try:
        manager.run(num_workers=3, duration=10)
    except KeyboardInterrupt:
        print("\n\nInterrotto da utente")
        # Anche in caso di interruzione richiede la fine dei figli e li raccoglie.
        manager.stop_all()
        manager.wait_workers()
