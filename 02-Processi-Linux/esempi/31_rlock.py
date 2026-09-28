#!/usr/bin/env python3
# =============================================================================
# File: 31_rlock.py
# Argomento: Lock rientrante nelle chiamate ricorsive
# Scopo: Lo stesso processo acquisisce più volte un RLock nei livelli da 0 a 3.
# Esecuzione: python3 31_rlock.py
# Nota: Ogni acquisizione richiede un rilascio, gestito dall’uscita dal relativo with.
# =============================================================================
from multiprocessing import Process, RLock
import time

def funzione_ricorsiva(rlock, depth, max_depth):
    # Caso base: interrompe la ricorsione prima di acquisire un altro livello.
    if depth > max_depth:
        return
    # Il proprietario può riacquisire RLock; un Lock normale si bloccherebbe
    # alla chiamata ricorsiva, perché il livello esterno lo detiene ancora.
    with rlock:
        print(f"  {'  ' * depth}Depth {depth}: lock acquisito")
        time.sleep(0.1)
        # Il lock resta acquisito mentre si entra nel livello successivo.
        funzione_ricorsiva(rlock, depth + 1, max_depth)
        # Questa stampa precede il rilascio effettivo, eseguito uscendo dal with.
        # Il lock torna disponibile ad altri solo dopo l’uscita dal livello esterno.
        print(f"  {'  ' * depth}Depth {depth}: lock rilasciato")

if __name__ == '__main__':
    print("=== RLock (Reentrant Lock) ===\n")
    rlock = RLock()
    p = Process(target=funzione_ricorsiva, args=(rlock, 0, 3))
    p.start()
    p.join()
