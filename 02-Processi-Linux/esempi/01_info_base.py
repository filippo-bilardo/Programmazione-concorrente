# Visualizzare informazioni processo in Python
import os
import psutil

pid = os.getpid()
processo = psutil.Process(pid)

print(f"PID: {processo.pid}")
print(f"Nome: {processo.name()}")
print(f"Stato: {processo.status()}")
print(f"Parent PID: {processo.ppid()}")
print(f"Memoria: {processo.memory_info().rss / 1024**2:.2f} MB")
print(f"CPU: {processo.cpu_percent()}%")
print(f"Thread: {processo.num_threads()}")
print(f"File aperti: {len(processo.open_files())}")