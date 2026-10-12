#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> // <--- Inclusa per la gestione dei segnali (SIGCHLD)

// Funzione di supporto per simulare il lampeggio (Messaggio a schermo e logica temporale)
void esegui_lampeggio(const char* colore, float intervallo_secondi) {
    int microsecondi = (int)(intervallo_secondi * 1000000);
    for (int i = 0; i < 10; i++) {
        // NOTA PER GLI STUDENTI: Qui inserirete anche il comando fisico per il PIN (es. tramite libreria pigpio o wiringPi)
        printf("[FIGLIO %d] Lampeggio %d: LED %s ACCESO\n", getpid(), i + 1, colore);
        usleep(microsecondi);
        printf("[FIGLIO %d] Lampeggio %d: LED %s SPENTO\n", i + 1, colore);
        usleep(microsecondi);
    }
}

int main() {
    char comando;
    int led = 0; // 1=Verde, 2=Rosso, 3=Giallo

    // EVITA I PROCESSI ZOMBIE: Dice al kernel di ripulire automaticamente i figli al termine,
    // permettendo al padre di NON usare la wait() bloccante e accettare comandi in parallelo.
    signal(SIGCHLD, SIG_IGN); 

    printf("=== Pannello di Controllo LED Multi-Processo (Parallelo) ===\n");
    printf("Comandi disponibili (puoi inserirne molteplici in sequenza):\n");
    printf("  v -> 10 lampeggi LED VERDE (0.3s)\n");
    printf("  r -> 10 lampeggi LED ROSSO (0.7s)\n");
    printf("  g -> 10 lampeggi LED GIALLO (1.2s)\n");
    printf("  q -> Esci dal programma principale\n\n");

    while (1) {
        printf("Inserisci un comando: ");
        // Legge il comando saltando eventuali spazi o invii residui
        if (scanf(" %c", &comando) != 1) continue;

        // Gestione del comando di chiusura prima di toccare i processi
        if (comando == 'q' || comando == 'Q') {
            printf("Chiusura del programma principale. I figli in background continueranno fino al termine. Arrivederci!\n");
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
        // STRUTTURA DELLA FORK ASINCRONA
        // =====================================================================
        pid_t pid = fork();

        if (pid < 0) {
            // Scenario a): Errore di sdoppiamento
            perror("[ERRORE] Generazione del processo figlio fallita");
            continue; 
        } 
        else if (pid == 0) {
            // Scenario b): Codice eseguito esclusivamente dal processo FIGLIO
            // Verifica il valore del led e imposta i parametri corretti
            switch (led) {
                case 1:
                    esegui_lampeggio("VERDE", 0.3f);
                    break;
                case 2:
                    esegui_lampeggio("ROSSO", 0.7f);
                    break;
                case 3:
                    esegui_lampeggio("GIALLO", 1.2f);
                    break;
                default:
                    printf("[FIGLIO %d] Errore: LED non riconosciuto.\n", getpid());
                    exit(1);
            }

            printf("[FIGLIO %d] Sequenza completata. Rilascio risorse.\n", getpid());
            exit(0); // Termina il figlio per evitare che ritorni nel ciclo while del padre
        } 
        else {
            // Scenario c): Codice eseguito esclusivamente dal processo PADRE
            // Il padre NON usa wait(NULL). Stampa il messaggio e torna subito a chiedere l'input.
            printf("[PADRE] Avviato LED %d in background (PID figlio: %d).\n\n", led, pid);
        }
        // =====================================================================
    }

    return 0;
}
/*
La linea di codice signal(SIGCHLD, gestore_sigchld); serve a registrare una funzione di 
 (chiamata "handler") per il segnale SIGCHLD nei sistemi operativi di tipo Unix/Linux. 
 In parole semplici, dice al sistema operativo cosa fare quando un processo figlio cambia stato.

 Il ruolo del segnale SIGCHLD
Quando un processo crea un processo figlio (tramite una chiamata come fork()), 
quest'ultimo lavora in modo indipendente. Nel momento in cui il figlio termina la sua 
esecuzione o viene interrotto, il kernel del sistema operativo invia automaticamente 
un segnale SIGCHLD al processo padre. Questo segnale serve a notificare il padre che 
la situazione del figlio è cambiata.

La funzione della chiamata signal
Di default, se il padre non specifica cosa fare, il segnale SIGCHLD viene ignorato. 
Utilizzando la funzione signal(), il programmatore sovrascrive questo comportamento 
predefinito. Con questa istruzione si dice al sistema: "Non appena arriva un segnale 
SIGCHLD, interrompi momentaneamente quello che stai facendo ed esegui immediatamente la 
funzione gestore_sigchld".

Che cos'è il gestore_sigchld
Il gestore_sigchld è una funzione personalizzata scritta dal programmatore (chiamata 
signal handler). All'interno di questa funzione viene solitamente inserita la chiamata 
a funzioni come wait() o waitpid(). Questo passaggio è fondamentale per raccogliere lo 
stato di terminazione del figlio e liberare le risorse di sistema che occupava.
Evitare i processi "Zombie"

L'obiettivo principale di questa riga di codice è prevenire la creazione di processi 
zombie. Se un processo figlio termina e il padre non ne raccoglie lo stato (tramite 
la wait), il figlio rimane nel sistema in uno stato "defunto" (zombie), occupando 
inutilmente un identificativo di processo (PID) nella tabella dei processi. L'uso di 
questo gestore permette una pulizia automatica e asincrona dei figli non appena muoiono.
 */
