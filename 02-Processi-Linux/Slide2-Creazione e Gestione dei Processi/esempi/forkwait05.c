//==========================================================================================
// Project: forkwait05.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Utilizzo istruzione wait con N_CHILD processi figli e terminazione volontaria.
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

  printf("[INIZIO] - Prima della fork - pid=%d, pid padre=%d\n",getpid(),getppid());
  
  int pid[N_CHILD];
  for(int i=0; i<N_CHILD; i++) {
    pid[i]=fork();
    if (pid[i]==0) {    // codice processo figlio
      childProcess(i);
      //exit(10); //break; // Attenzione! ogni child riceve una "copia" del ciclo for
    } else if (pid[i]>0) { // codice processo padre
      printf("[Padre] - pid=%d, pid padre=%d, pid[i]=%d\n", getpid(), getppid(), pid[i]);
    } else { // pid<0 // Errore
      printf("Creazione del processo figlio fallita!\n");
    }    
  }
  
  //Codice eseguito solo dal padre
  printf("[Padre] - pid=%d, pid padre=%d, num_figli=%d\n", getpid(), getppid(), N_CHILD);
  int wait_status; int wait_status_high, wait_status_low;
  int wait_retvalue;
  for(int i=0; i<N_CHILD; i++) {
    wait_retvalue = wait(&wait_status);
    wait_status_high = wait_status >> 8; // wait_status/256
    wait_status_low = wait_status & 0xFF;
    printf("[Padre] - wait_retvalue=%d, wait_status_high=%d, wait_status_low=%d\n", wait_retvalue, wait_status_high, wait_status_low);  
  }

  exit(4);
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(int num) {
  printf("[Figlio] - pid=%d, pid padre=%d\n", getpid(), getppid());
  exit(num*10);
}












