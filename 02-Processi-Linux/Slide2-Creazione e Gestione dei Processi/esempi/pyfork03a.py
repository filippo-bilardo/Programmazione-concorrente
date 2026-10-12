import os

# Calcola l'area e il perimetro di un rettangolo
# il processo padre calcola l'area, il processo figlio calcola il perimetro
base = 5
altezza = 10

print(f"Codice eseguito dal padre! (pid: {os.getpid()})")

# Creazione del processo figlio, la variabile pid conterrà 
# il valore 0 nel processo figlio e il pid del figlio nel processo padre
pid = os.fork()
if pid == 0:
    # Codice del figlio, calcolo del perimetro
    perimetro = 2 * (base + altezza)
    print(f"Il perimetro del rettangolo è: {perimetro}")
    print(f"Sono il figlio! (pid: {os.getpid()})")
elif pid > 0:
    # Codice del padre
    area = base * altezza
    print(f"L'area del rettangolo è: {area}")
    print(f"Sono il padre! (pid: {os.getpid()})")
else: # pid < 0
    # Errore nella creazione del processo figlio
    print("Creazione del processo figlio fallita!")

# Codice comune a padre e figlio
print(f"Codice comune a padre e figlio! (pid: {os.getpid()})")

