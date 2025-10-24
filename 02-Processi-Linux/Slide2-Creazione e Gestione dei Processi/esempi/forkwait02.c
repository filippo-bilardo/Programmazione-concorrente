//==========================================================================================
// Project: forkwait02.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Esempio di utilizzo istruzione wait con un processo figlio 
//  - se tutti i figli non sono ancora terminati, il processo si sospende in attesa della 
//    terminazione del primo di essi
//  - se almeno un figlio F e' gia' terminato ed il suo stato non e' stato ancora rilevato 
//    (cioè F e' in statozombie), wait() ritorna immediatamente con il suo stato di 
//    terminazione (nella variabile status)
//  - se non esiste neanche un figlio, wait() NON e' sospensiva e ritorna un codice di 
//    errore (valore restituito < 0)
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
int main(int argc, char **argv) {
  
  printf("\n[INIZIO] - Prima della fork - pid processo=%d, pid padre=%d\n",getpid(),getppid());

  int pid=fork();
  if (pid==0) { // Processo figlio 
    childProcess();
  } else if (pid>0) { // Processo padre
    fatherProcess(pid);
  } else {// (pid<0) Errore nella creazione del processo 
    printf("Creazione del processo figlio fallita!\n");
  }

  printf("[FINE] - Termine del processo con pid=%d avente pid padre=%d\n",getpid(),getppid());
  exit(3);
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(void) {
  sleep(3);
  printf("[Figlio] - pid=%d, pid padre=%d\n", getpid(), getppid());
  exit(100);
}
void fatherProcess(int childpid) {
  
  printf("\n[Padre] - pid=%d, pid padre=%d, pid figlio=%d\n",getpid(),getppid(),childpid);
  int wait_status;
  int wait_retvalue;
  wait_retvalue = wait(&wait_status);
  printf("[Padre] - wait_retvalue=%d, wait_status=%d\n", wait_retvalue, wait_status);
}












