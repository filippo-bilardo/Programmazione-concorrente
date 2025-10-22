#!/usr/bin/env python3
from multiprocessing import Process, RLock
import time

def funzione_ricorsiva(rlock, depth, max_depth):
    if depth > max_depth:
        return
    with rlock:
        print(f"  {'  ' * depth}Depth {depth}: lock acquisito")
        time.sleep(0.1)
        funzione_ricorsiva(rlock, depth + 1, max_depth)
        print(f"  {'  ' * depth}Depth {depth}: lock rilasciato")

if __name__ == '__main__':
    print("=== RLock (Reentrant Lock) ===\n")
    rlock = RLock()
    p = Process(target=funzione_ricorsiva, args=(rlock, 0, 3))
    p.start()
    p.join()
