/**
 * @file segmenti_memorie.c
 * @brief Esempio di utilizzo dei diversi segmenti di memoria in un processo Linux.
 * 
 * Scopo: confrontare gli indirizzi di codice, dati globali/statici,
 * variabili automatiche e memoria allocata dinamicamente.
 * Compilazione: gcc -Wall -Wextra segmenti_memorie.c -o segmenti_memorie
 * Esecuzione: ./segmenti_memorie
 * Contesto: disposizione tipica di un processo Linux; gli indirizzi sono
 * virtuali e possono cambiare a ogni avvio per effetto dell'ASLR.
 * 
 * @author Filippo Bilardo
 * versione 1.0 26/09/26 - versione iniziale
 * 
 * 
 */
#include <stdio.h>
#include <stdlib.h>

// 1. SEGMENTO .data (Dati globali inizializzati)
int globale_inizializzata = 42; 

// 2. SEGMENTO .bss (Dati globali NON inizializzati - azzerati all'avvio)
int globale_non_inizializzata; 

void funzione(int parametro_stack) {
    // Le indicazioni sui segmenti descrivono la disposizione tipica generata
    // dal compilatore: il linguaggio C non impone questa organizzazione fisica.
    // 3. STACK (Variabili locali e parametri)
    int locale_stack = 10;
    
    // 4. SEGMENTO .data (Le variabili statiche inizializzate mantengono lo stato)
    // Lo scope resta locale alla funzione, ma la durata copre tutto il programma.
    static int statica_inizializzata = 100; 
    
    // 5. SEGMENTO .bss (Le variabili statiche non inizializzate vanno qui)
    /*
    Il segmento .bss (acronimo storico di Block Started by Symbol) è una delle ottimizzazioni 
    più eleganti della gestione della memoria nei sistemi operativi.La sua regola d'oro è: 
    contiene le variabili globali e statiche che NON sono state inizializzate esplicitamente 
    dal programmatore (o che sono state inizializzate a zero, ad esempio int x = 0;).
    */
    static int statica_non_inizializzata; 

    // 6. HEAP (Memoria dinamica allocata a runtime)
    // Il puntatore è una variabile locale; il blocco a cui punta è sull'heap.
    // malloc riserva sizeof(int) byte senza inizializzarli e può restituire NULL.
    // Qui si stampa solo il puntatore: il blocco non viene dereferenziato.
    int *puntatore_heap = (int *)malloc(sizeof(int)); 

    // Stampa degli indirizzi per vedere la disposizione in memoria
    // %p stampa un puntatore convertito a void*. La conversione dell'indirizzo
    // di funzione è supportata nel contesto Linux, ma non è portabile in ISO C.
    printf("--- SEGMENTO TEXT (Codice) ---\n");
    printf("Indirizzo funzione:                  %p\n\n", (void*)&funzione);

    printf("--- SEGMENTO DATA (.data & .bss) ---\n");
    printf("Indirizzo globale_inizializzata:     %p (.data)\n", (void*)&globale_inizializzata);
    printf("Indirizzo statica_inizializzata:     %p (.data)\n", (void*)&statica_inizializzata);
    printf("Indirizzo globale_non_inizializzata: %p (.bss)\n", (void*)&globale_non_inizializzata);
    printf("Indirizzo statica_non_inizializzata: %p (.bss)\n", (void*)&statica_non_inizializzata);
    
    // Le direzioni indicate nelle etichette sono uno schema didattico comune:
    // allocatore, architettura e mappature possono produrre disposizioni diverse.
    printf("\n--- SEGMENTO HEAP (Cresce verso l'alto) ---\n");
    printf("Indirizzo memoria su Heap:           %p\n\n", (void*)puntatore_heap);

    printf("--- SEGMENTO STACK (Cresce verso il basso) ---\n");
    printf("Indirizzo parametro_stack:           %p\n", (void*)&parametro_stack);
    printf("Indirizzo locale_stack:              %p\n", (void*)&locale_stack);
    printf("Indirizzo puntatore_heap (la var):   %p\n", (void*)&puntatore_heap);

    // free libera il blocco dinamico; non la variabile locale puntatore_heap.
    // Dopo free il blocco non è più utilizzabile; free(NULL) è consentito.
    free(puntatore_heap); 
}

int main() {
    // Passa un valore al parametro e osserva gli indirizzi durante la chiamata.
    funzione(5);
    // Codice di uscita 0: esecuzione completata con successo.
    return 0;
}
