import os
import signal
import sys
import time

def esegui_lampeggio(colore, intervallo_secondi):
    """
    Funzione di supporto per simulare il lampeggio (Messaggio a schermo e logica temporale)
    """
    pid_figlio = os.getpid()
    for i in range(10):
        # NOTA PER GLI STUDENTI: Qui inserirete anche il comando fisico per il PIN (es. tramite RPi.GPIO o gpiozero)
        print(f"[FIGLIO {pid_figlio}] Lampeggio {i + 1}: LED {colore} ACCESO")
        time.sleep(intervallo_secondi)
        print(f"[FIGLIO {pid_figlio}] Lampeggio {i + 1}: LED {colore} SPENTO")
        time.sleep(intervallo_secondi)

# =====================================================================
# Main Program
# =====================================================================

# EVITA I PROCESSI ZOMBIE: Dice al kernel di ripulire automaticamente i figli al termine,
# permettendo al padre di NON usare la wait() bloccante e accettare comandi in parallelo.
signal.signal(signal.SIGCHLD, signal.SIG_IGN)

print("=== Pannello di Controllo LED Multi-Processo (Parallelo) ===")
print("Comandi disponibili (puoi inserirne molteplici in sequenza):")
print("  v -> 10 lampeggi LED VERDE (0.3s)")
print("  r -> 10 lampeggi LED ROSSO (0.7s)")
print("  g -> 10 lampeggi LED GIALLO (1.2s)")
print("  q -> Esci dal programma principale\n")

while True:
    try:
        # Legge l'input rimuovendo gli spazi bianchi (simile al comportamento di " %c" in scanf)
        input_utente = input("Inserisci un comando: ").strip()
    except (EOFError, KeyboardInterrupt):
        print("\nChiusura forzata del programma.")
        break

    if not input_utente:
        continue

    # Estrae il primo carattere (per imitare il comportamento dello scanf %c)
    comando = input_utente

    # Gestione del comando di chiusura prima di toccare i processi
    if comando == 'q' or comando == 'Q':
        print("Chiusura del programma principale. I figli in background continueranno fino al termine. Arrivederci!")
        break

    # Impostazione della variabile in base al comando
    if comando == 'v' or comando == 'V':
        led = 1
        colore_led = "VERDE"
        intervallo = 0.3
    elif comando == 'r' or comando == 'R':
        led = 2
        colore_led = "ROSSO"
        intervallo = 0.7
    elif comando == 'g' or comando == 'G':
        led = 3
        colore_led = "GIALLO"
        intervallo = 1.2
    else:
        print("Comando non valido!")
        continue  # Torna all'inizio del ciclo

    # =====================================================================
    # FORK
    # =====================================================================
    try:
        pid = os.fork()
    except OSError:
        # Scenario a): Errore di sdoppiamento
        print("[ERRORE] Generazione del processo figlio fallita")
        continue

    if pid == 0:
        # Scenario b): Codice eseguito esclusivamente dal processo FIGLIO
        if led == 1:
            esegui_lampeggio(colore_led, intervallo)
        elif led == 2:
            esegui_lampeggio(colore_led, intervallo)
        elif led == 3:
            esegui_lampeggio(colore_led, intervallo)
        else:
            print(f"[FIGLIO {os.getpid()}] Errore: LED non riconosciuto.")
            sys.exit(1)

        print(f"[FIGLIO {os.getpid()}] Sequenza completata. Rilascio risorse.")
        sys.exit(0)  # Termina il figlio per evitare che ritorni nel ciclo while del padre
    else:
        # Scenario c): Codice eseguito esclusivamente dal processo PADRE
        # Il padre NON usa wait(). Stampa il messaggio e torna subito a chiedere l'input.
        print(f"[PADRE] Avviato LED {led} in background (PID figlio: {pid}).\n")
    # =====================================================================
