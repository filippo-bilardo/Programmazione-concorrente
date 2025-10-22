//==========================================================================================
// Project: fork05.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Creazione di un processo figlio 
//  - un processo ha sempre un padre. Se il padre termina prima del figlio, il padre diventa
//    il padre del padre e cosi via fino ad arrivare al processo "Init" pid=1.
//    Quindi, se il processo padre e il padre del padre (bash) terminano prima del figlio
//    il processo padre diventa "Init" pid=1
//
// Ver  Date      Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0  07/12/20  Versione iniziale
//==========================================================================================
//------------------------------------------------------------------------------------------
//=== Includes =============================================================================
//------------------------------------------------------------------------------------------
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, sleep

//------------------------------------------------------------------------------------------
//=== Function prototypes ==================================================================
//------------------------------------------------------------------------------------------
void childProcess(void);
void fatherProcess(int childpid);

//------------------------------------------------------------------------------------------
//=== Main =================================================================================
//------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

  printf("\n[INIZIO] - Esiste solo il processo padre, il padre del padre e' la shell \n");
  printf("[INIZIO] - Prima della fork - pid processo=%d, pid padre=%d\n",getpid(),getppid());

  // Creazione del processo figlio
  int pid=fork();
  if (pid==0) {
    // Esecuzione codice del processo figlio
    childProcess();
  } else if (pid>0) {
    // Esecuzione codice del processo padre
    fatherProcess(pid);
  } else { // pid<0
    // Errore
    printf("Creazione del processo figlio fallita!\n");
  }

  // Codice comune a processo padre e figlio
  printf("[FINE] - Termine del processo con pid=%d, padre=%d\n", getpid(), getppid());
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(void) {
  // Se aspettiamo 1 secondi il processo padre e il padre del padre (bash) termineranno
  // il processo padre diventera' "Init"
  sleep(1);
  printf("[Figlio] - pid=%d, pid padre=%d\n",getpid(),getppid());
}
void fatherProcess(int childpid) {
  printf("\n[Padre] - pid=%d, pid padre=%d, pid figlio=%d\n",getpid(),getppid(),childpid);
}

/* Possibile output:
fb@rpi2:~/sis2/01-fork $ gcc fork05.c
fb@rpi2:~/sis2/01-fork $ ./a.out

[INIZIO] - Esiste solo il processo padre, il padre del padre è la shell
[INIZIO] - Prima della fork - pid processo=7145, pid padre=4448
[Padre] - pid=7145, pid padre=4448, pid figlio=7146
[FINE] - Termine del processo con pid=7145, padre=4448
fb@rpi2:~/sis2/01-fork\$ [Figlio] - pid=7146, pid padre=1
[FINE] - Termine del processo con pid=7146, padre=1
*/
