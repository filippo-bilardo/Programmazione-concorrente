import os

def main():
    print(f"Codice eseguito dal padre! (pid: {os.getpid()})")
    pid = os.fork()

    if pid == 0:
        # Codice del figlio
        print(f"Sono il figlio! (pid: {os.getpid()})")
    elif pid > 0:
        # Codice del padre
        print(f"Sono il padre! (pid: {os.getpid()})")
    else:
        # Errore nella creazione del processo figlio
        print("Creazione del processo figlio fallita!")

    print(f"Codice comune a padre e figlio! (pid: {os.getpid()})")

if __name__ == "__main__":
    main()