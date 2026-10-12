#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

// 1. Variabile globale inizializzata -> Sezione .data
int globale_inizializzata = 42;

// 2. Variabile globale NON inizializzata -> Sezione .bss
int globale_non_inizializzata;

// 3. Costante globale -> Sezione .rodata
const char* stringa_costante = "Sono in .rodata";

void funzione_esempio() {
    // Questa funzione serve solo per mostrare l'indirizzo del codice
}

int main() {
    // 4. Variabile locale -> [stack]
    int variabile_locale = 10;

    // 5. Allocazione dinamica piccola -> [heap] (tramite brk/sbrk internamente)
    int* ptr_heap = (int*)malloc(sizeof(int));

    // 6. Allocazione dinamica grande -> Memory Mapping Segment (tramite mmap)
    // 200 KB superano la soglia standard di malloc per l'uso di mmap
    void* ptr_mmap = malloc(200 * 1024);

    printf("=== MAPPA DELLO SPAZIO DI INDIRIZZAMENTO VIRTUALE ===\n\n");

    // Indirizzi Bassi (Codice e Costanti)
    printf("[BASSO] .text (Codice)      : %p\n", (void*)&funzione_esempio);
    printf("[BASSO] .rodata (Costanti)  : %p\n", (void*)stringa_costante);
    
    // Dati Modificabili Statici
    printf("[MEDIO] .data (Glob. Iniz.) : %p\n", (void*)&globale_inizializzata);
    printf("[MEDIO] .bss (Glob. Non In.): %p\n", (void*)&globale_non_inizializzata);
    
    // Aree Dinamiche
    printf("[CRESCE UPG] [heap]         : %p\n", (void*)ptr_heap);
    printf("[CENTRO] mmap / Librerie    : %p\n", ptr_mmap);
    printf("[ALTO]  [stack] (Locali)    : %p\n\n", (void*)&variabile_locale);

    printf("Il PID del processo e': %d\n", getpid());
    printf("Premi INVIO per terminare il programma...");
    
    // Mantiene il processo attivo per permetterti di controllare /proc/PID/maps
    getchar(); 

    // Pulizia
    free(ptr_heap);
    free(ptr_mmap);
    return 0;
}
