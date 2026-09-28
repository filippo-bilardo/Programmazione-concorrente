# =============================================================================
# File: 01_info_base.py
# Argomento: Informazioni sul processo corrente
# Scopo: Legge PID, stato, memoria, CPU, thread e file aperti tramite psutil.
# Esecuzione: python3 01_info_base.py
# Nota: Dipendenza esterna: psutil (installazione: python3 -m pip install psutil).
# =============================================================================
# Visualizzare informazioni processo in Python
import os
import psutil

# getpid() restituisce l’identificatore del processo che esegue questo script.
pid = os.getpid()
# psutil offre un oggetto con cui interrogare il sistema operativo su quel PID.
processo = psutil.Process(pid)

print(f"PID: {processo.pid}")
print(f"Nome: {processo.name()}")
print(f"Stato: {processo.status()}")
print(f"Parent PID: {processo.ppid()}")
# RSS è la memoria residente in RAM, convertita da byte in MiB (1024**2).
print(f"Memoria: {processo.memory_info().rss / 1024**2:.2f} MB")
# La prima misura non bloccante della CPU è un valore iniziale, normalmente 0.0;
# per misurare un intervallo servirebbe una seconda lettura dopo una pausa.
print(f"CPU: {processo.cpu_percent()}%")
print(f"Thread: {processo.num_threads()}")
# open_files() elenca file regolari aperti, non tutti i descrittori (es. socket).
print(f"File aperti: {len(processo.open_files())}")