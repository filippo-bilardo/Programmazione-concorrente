import os
import time

def main():
    print(f"Codice padre! (pid: {os.getpid()})")
    pid = os.fork()

    if pid == 0:
        # Codice del figlio
        for i in range(30):
            print(f"{i} - Sono il figlio! (pid: {os.getpid()})")
            time.sleep(1)
    elif pid > 0:
        # Codice del padre
        for i in range(30):
            print(f"{i} - Sono il padre! (pid: {os.getpid()})")
            time.sleep(1)
    else:
        # Errore nella creazione del processo figlio
        print("Creazione del processo figlio fallita!")

    print(f"Codice comune a padre e figlio! (pid: {os.getpid()})")

if __name__ == "__main__":
    main()