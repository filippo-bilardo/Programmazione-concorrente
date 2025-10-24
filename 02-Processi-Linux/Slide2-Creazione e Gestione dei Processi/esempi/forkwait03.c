//==========================================================================================
// Project: forkwait03.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Esempio di utilizzo istruzione wait con un processo figlio e terminazione volontaria.
//  In caso di terminazione di un figlio, la variabile status raccoglie stato di 
//  terminazione; nell’ipotesi che lo stato sia un intero a 16 bit:
//  + se il byte meno significativo di status e' zero, il più significativo rappresenta
//    lo stato di terminazione (terminazione volontaria, ad esempio con exit)
//  + in caso contrario, il byte meno significativo di status descrive il segnale
//    che ha terminato il figlio (terminazione involontaria)
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
//=== Function prototypes ==================================================================
//------------------------------------------------------------------------------------------
void childProcess(void);
void fatherProcess(int childpid);

//------------------------------------------------------------------------------------------
//=== Main =================================================================================
//------------------------------------------------------------------------------------------
int main(int argc, char **argv) 
{
  printf("\n[INIZIO] - Prima della fork - pid processo=%d, pid padre=%d\n",getpid(),getppid());

  int pid=fork();
  if (pid==0) { // Processo figlio 
    childProcess();
  } else if (pid>0) { // Processo padre
    fatherProcess(pid);
  } else {// (pid<0) Errore nella creazione del processo 
    printf("Creazione del processo figlio fallita!\n");
  }
  
  exit(4);
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(void) {
  sleep(2);
  printf("[Figlio] - pid=%d, pid padre=%d\n\n", getpid(), getppid());
  exit(100);
}
void fatherProcess(int childpid) {

  printf("[Padre] - pid=%d, pid padre=%d, pid figlio=%d\n", getpid(), getppid(), childpid);
  int wait_status; int wait_status_high, wait_status_low;
  int wait_retvalue;
  wait_retvalue = wait(&wait_status);
  wait_status_high = wait_status >> 8; // wait_status/256
  wait_status_low = wait_status & 0xFF;
  printf("[Padre] - wait_retvalue=%d, wait_status_high=%d, wait_status_low=%d\n\n", wait_retvalue, wait_status_high, wait_status_low);  
}












