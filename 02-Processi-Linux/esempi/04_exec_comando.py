#!/usr/bin/env python3
"""Esempio di exec() per eseguire comandi"""
import os

print("Prima di exec()")
print(f"PID: {os.getpid()}")
print(f"Programma: Python\n")

# Sostituisce Python con ls
os.execl("/bin/ls", "ls", "-lh", "/tmp")

# Questo codice NON viene MAI eseguito
print("Questo non verrà mai stampato!")
