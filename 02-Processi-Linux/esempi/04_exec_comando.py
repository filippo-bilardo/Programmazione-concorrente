#!/usr/bin/env python3
# =============================================================================
# File: 04_exec_comando.py
# Argomento: Sostituzione del programma con exec
# Scopo: Sostituisce il programma Python con /bin/ls, conservando il PID.
# Esecuzione: python3 04_exec_comando.py
# Nota: Richiede Linux/Unix e /bin/ls; elenca il contenuto di /tmp.
# =============================================================================
"""Esempio di exec() per eseguire comandi"""
import os

print("Prima di exec()")
print(f"PID: {os.getpid()}")
print(f"Programma: Python\n")

# execl() riceve il percorso e gli argomenti separati: "ls" diventa argv[0].
# -l richiede il formato dettagliato, -h rende leggibili le dimensioni.
# exec non crea un processo: sostituisce il programma nello stesso PID.
# Le stampe precedenti richiedono flush o python3 -u se stdout è bufferizzato.
os.execl("/bin/ls", "ls", "-lh", "/tmp")

# Se exec riesce non ritorna; se fallisce solleva OSError, qui non gestito.
print("Questo non verrà mai stampato!")
