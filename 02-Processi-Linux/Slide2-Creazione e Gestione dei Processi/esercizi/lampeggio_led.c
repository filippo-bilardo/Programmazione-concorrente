#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Funzione di supporto per simulare il lampeggio (Messaggio a schermo e logica temporale)
void esegui_lampeggio(const char* colore, float intervallo_secondi) {
    int microsecondi = (int)(intervallo_secondi * 1000000);
    
    for (int i = 0; i < 10; i++) {
        // NOTA PER GLI STUDENTI: Qui inserirete anche il comando fisico per il PIN (es. tramite libreria pigpio o wiringPi)
        printf("[FIGLIO] Lampeggio %d: LED %s ACCESO\n", i + 1, colore);
        usleep(microsecondi);
        
        printf("[FIGLIO] Lampeggio %d: LED %s SPENTO\n", i + 1, colore);
        usleep(microsecondi);
    }
}

int main() {
    char comando;
    int led = 0; // 1=Verde, 2=Rosso, 3=Giallo

    printf("=== Pannello di Controllo LED tramite Processi ===\n");
    printf("Comandi disponibili:\n");
    printf("  v -> 10 lampeggi LED VERDE  (0.3s)\n");
    printf("  r -> 10 lampeggi LED ROSSO  (0.7s)\n");
    printf("  g -> 10 lampeggi LED GIALLO (1.2s)\n");
    printf("  q -> Esci dal programma\n\n");

    while (1) {
        printf("Inserisci un comando: ");
        // Legge il comando saltando eventuali spazi o invii residui
        if (scanf(" %c", &comando) != 1) continue;

        // Gestione del comando di chiusura prima di toccare i processi
        if (comando == 'q' || comando == 'Q') {
            printf("Chiusura del programma principale. Arrivederci!\n");
            break;
        }

        // Impostazione della variabile in base al comando
        if (comando == 'v' || comando == 'V') {
            led = 1;
        } else if (comando == 'r' || comando == 'R') {
            led = 2;
        } else if (comando == 'g' || comando == 'G') {
            led = 3;
        } else {
            printf("Comando non valido!\n");
            continue; // Torna all'inizio del ciclo
        }

        // =====================================================================
        // STRUTTURA DELLA FORK - DA COMPLETARE A CURA DELLO STUDENTE
        // =====================================================================
        
        /* 
         * ESERCIZIO PER LO STUDENTE:
         * 1. Dichiarare una variabile di tipo pid_t per memorizzare l'esito della fork().
         * 2. Chiamare la funzione fork() per sdoppiare il processo attuale.
         * 3. Gestire i tre possibili scenari dell'output della fork con una struttura condizionale (if/else):
         *    a) Errore di sdoppiamento (PID < 0)
         *    b) Codice eseguito esclusivamente dal processo FIGLIO (PID == 0)
         *    c) Codice eseguito esclusivamente dal processo PADRE (PID > 0)
         * 
         * Nel blocco del FIGLIO:
         *    - Leggere il valore della variabile 'led'.
         *    - In base al valore (1, 2 o 3), invocare la funzione 'esegui_lampeggio' 
         *      passando i parametri corretti richiesti dalla consegna.
         *    - IMPORTANTE: Ricordarsi di terminare il processo figlio con exit(0) 
         *      per evitare che ritorni nel ciclo while del padre!
         * 
         * Nel blocco del PADRE:
         *    - Per evitare la creazione di processi "zombie", il padre deve attendere 
         *      che il figlio termini la sua sequenza di lampeggi prima di chiedere un nuovo comando.
         *      Utilizzare la chiamata di sistema wait(NULL).
         */

        // --- INIZIO AREA DA IMPLEMENTARE ---
        
        // Rimuovere o commentare la riga sottostante dopo aver implementato la fork
        printf("[SISTEMA] Fork non implementata. Valore attuale di 'led': %d\n", led);

        /* Suggerimento traccia:
        pid_t pid = fork();
        if (pid < 0) {
            // Gestione errore
        } else if (pid == 0) {
            // Logica del FIGLIO (switch o if su variabile 'led')
            // ...
            // esegui_lampeggio("VERDE", 0.3);
            // ...
            exit(0); 
        } else {
            // Logica del PADRE
            // ...
        }
        */

        // --- FINE AREA DA IMPLEMENTARE ---
        // =====================================================================
    }

    return 0;
}
