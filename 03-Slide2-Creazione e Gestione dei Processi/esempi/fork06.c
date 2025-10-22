//==========================================================================================
// Project: fork06.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Creazione di N_CHILD processi figlio 
//  Attenzione! Tutto il codice dopo l'istruzione fork viene eseguito anche dal processo 
//  Figlio.
//
// Ver  Date      Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0  07/12/20  Versione iniziale
//==========================================================================================
//------------------------------------------------------------------------------------------
//=== Includes =============================================================================
//------------------------------------------------------------------------------------------
#include <stdio.h>   // printf
#include <unistd.h>  // getpid
#include <stdlib.h>  // exit

//------------------------------------------------------------------------------------------
//=== Constants ============================================================================
//------------------------------------------------------------------------------------------
#define N_CHILD 4 //numero di processi da creare

//------------------------------------------------------------------------------------------
//=== Function prototypes ==================================================================
//------------------------------------------------------------------------------------------
void childProcess(int num);

//------------------------------------------------------------------------------------------
//=== Main =================================================================================
//------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

  printf("[INIZIO] - Prima della fork - pid=%d, pid padre=%d\n", getpid(), getppid());
  
  int pid[N_CHILD];
  for(int i=0; i<N_CHILD; i++) {
    pid[i]=fork();
    if (pid[i]==0) {    // codice processo figlio
      childProcess(i);
      exit(10); //break; // Attenzione! ogni child riceve una "copia" del ciclo for
    } else if (pid[i]>0) { // codice processo padre
      printf("[Padre] - pid=%d, pid padre=%d, pid[i]=%d\n", getpid(), getppid(), pid[i]);
    } else { // pid<0 // Errore
      printf("Creazione del processo figlio fallita!\n");
    }    
  }
  
  //Codice eseguito solo dal padre
  printf("[FINE] - pid=%d, pid padre=%d, N_CHILD=%d\n", getpid(), getppid(), N_CHILD);
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(int num) {
  printf("[Figlio] - pid=%d, pid padre=%d\n", getpid(), getppid());
}

/* Possibile output:
fb@rpi2:~/sis2/01-fork $ gcc fork06.c
fb@rpi2:~/sis2/01-fork $ ./a.out

[INIZIO] - Prima della fork - pid=15189, pid padre=4448

[Padre] - pid=15189, pid padre=4448, pid[i]=15190
[Figlio] - pid=15190, pid padre=15189

[Padre] - pid=15189, pid padre=4448, pid[i]=15191
[Padre] - pid=15189, pid padre=4448, pid[i]=15192
[Padre] - pid=15189, pid padre=4448, pid[i]=15193

[FINE] - pid=15189, pid padre=4448, N_CHILD=4

[Figlio] - pid=15191, pid padre=15189
fb@rpi2:~/sis2/01-fork \$ [Figlio] - pid=15192, pid padre=1
[Figlio] - pid=15193, pid padre=1
*/
