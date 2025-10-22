//==========================================================================================
//
// Project:  sig05.c
// Date:     21/03/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  In questo esempio si utilizza una sola funzione di handler per gestire vari segnali.
//   
//  eseguire il processo in background
//  $./a.out &
//  
//  inviare il segnale SIGUSR1 al processo
//  $kill -10 <pid processo>
//
//  inviare il segnale SIGUSR2 al processo
//  $kill -12 <pid processo>
//  
//  inviare il segnale CTRL-C al processo
//  $kill -SIGINT <pid processo>
//
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   21/03/17     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <signal.h>   // signal, costanti

//------------------------------------------------------------------------------------------
void handler_signal(int signum) 
{
  if (signum == SIGINT) {printf("[handler] Ricevuto il segnale SIGINT\n"); exit(1);} 
  else if (signum == SIGUSR1) {printf("[handler] Ricevuto il segnale SIGUSR1\n");}
  else if (signum == SIGUSR2) {printf("[handler] Ricevuto il segnale SIGUSR2\n");}
  else {printf("[handler] Ricevuto il segnale %d\n",signum);}
}

//------------------------------------------------------------------------------------------
int main() 
{
  signal(SIGINT, handler_signal);     // 2  - Interrupt (ANSI). Action: exit
  signal(SIGUSR1, handler_signal);    // 10 - User-defined signal 1 (POSIX). Action: exit
  //signal(SIGUSR2, handler_signal);  // 12 - User-defined signal 2 (POSIX). Action: exit
  
  printf("\n[INIZIO]\n");
  //Ciclo infinito
  for(;;)
  {
    printf("Ciclo infinito ...\n");
    sleep(5);
  }

  // Codice mai eseguito
  printf("\n[FINE]\n");
  exit(0);
}
