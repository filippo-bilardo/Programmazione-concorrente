import os
import sys
import time

def esegui_lampeggio(colore, intervallo_secondi):
    """
    Funzione di supporto per simulare il lampeggio (Messaggio a schermo e logica temporale)
    """
    for i in range(10):
        # NOTA PER GLI STUDENTI: Qui inserirete anche il comando fisico per il PIN (es. tramite RPi.GPIO o gpiozero)
        print(f"[FIGLIO] Lampeggio {i + 1}: LED {colore} ACCESO")
        time.sleep(intervallo_secondi)
        
        print(f"[FIGLIO] Lampeggio {i + 1}: LED {colore} SPENTO")
        time.sleep(intervallo_secondi)

# =====================================================================
# Main Program
# =====================================================================

print("=== Pannello di Controllo LED tramite Processi ===")
print("Comandi disponibili:")
print("  v -> 10 lampeggi LED VERDE  (0.3s)")
print("  r -> 10 lampeggi LED ROSSO  (0.7s)")
print("  g -> 10 lampeggi LED GIALLO (1.2s)")
print("  q -> Esci dal programma\n")

while True:
    try:
        # Legge il comando saltando eventuali spazi o invii residui
        input_utente = input("Inserisci un comando: ").strip()
    except (EOFError, KeyboardInterrupt):
        print("\nChiusura forzata del programma.")
        break

    if not input_utente:
        continue

    comando = input_utente

    # Gestione del comando di chiusura prima di toccare i processi
    if comando == 'q' or comando == 'Q':
        print("Chiusura del programma principale. Arrivederci!")
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
    # STRUTTURA DELLA FORK - IMPLEMENTATA IN PYTHON
    # =====================================================================
    try:
        # 1 e 2. Chiamata alla funzione fork() per sdoppiare il processo attuale
        pid = os.fork()
    except OSError:
        # a) Errore di sdoppiamento
        print("[ERRORE] Generazione del processo figlio fallita")
        continue

    if pid == 0:
        # b) Codice eseguito esclusivamente dal processo FIGLIO
        if led == 1:
            esegui_lampeggio(colore_led, intervallo)
        elif led == 2:
            esegui_lampeggio(colore_led, intervallo)
        elif led == 3:
            esegui_lampeggio(colore_led, intervallo)
        else:
            print(f"[FIGLIO] Errore: LED non riconosciuto.")
            sys.exit(1)

        # IMPORTANTE: Termina il processo figlio per evitare che ritorni nel ciclo del padre
        sys.exit(0)
    else:
        # c) Codice eseguito esclusivamente dal processo PADRE
        # Per evitare processi "zombie" e forzare la sincronizzazione,
        # il padre attende attivamente che il figlio termini prima di chiedere un nuovo comando.
        print(f"[PADRE] Figlio avviato (PID: {pid}). Attendo il completamento...")
        
        # os.wait() corrisponde a wait(NULL) in C e restituisce una tupla (pid, status)
        pid_archiviato, stato = os.wait()
        
        print(f"[PADRE] Il figlio {pid_archiviato} ha terminato. Ora puoi inserire un nuovo comando.\n")
    # =====================================================================
