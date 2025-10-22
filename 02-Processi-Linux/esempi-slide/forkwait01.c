//==========================================================================================
// Project: forkwait01.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Lo stato di terminazione del processo figlio puo' essere rilevato dal processo padre, 
//  mediante la system call wait()
//  int wait(int *status);
//  - parametro status e' l'indirizzo della variabile in cui viene memorizzato lo stato 
//    di terminazione del figlio
//  - risultato prodotto dalla wait() e' il pid del processo terminato, oppure un codice 
//    di errore (<0)
//
//  Se non esiste neanche un figlio, wait() NON e' sospensiva e ritorna un codice di errore
//  (valore restituito < 0)
//
// Ver  Date      Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0  07/12/20  Versione iniziale
//==========================================================================================
//------------------------------------------------------------------------------------------
//=== Includes =============================================================================
//------------------------------------------------------------------------------------------
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep
#include <stdlib.h>  // exit
#include <sys/wait.h>// wait

//------------------------------------------------------------------------------------------
//=== Main =================================================================================
//------------------------------------------------------------------------------------------
int main(int argc, char **argv) {

  int wait_status;
  int wait_retvalue;
  
  wait_retvalue = wait(&wait_status);
  
  printf("[FINE] - wait_retvalue=%d, wait_status=%d\n", wait_retvalue, wait_status);
  printf("[FINE] - Termine del processo con pid=%d avente pid padre=%d\n",getpid(),getppid());
  printf("\n");
  
  exit(0);
}

