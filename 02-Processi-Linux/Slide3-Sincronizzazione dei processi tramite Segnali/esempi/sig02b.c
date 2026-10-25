//==========================================================================================
//
// Project:  sig02.c
// Date:     21/03/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  In questo esempio al segnale SIGINT (CTRL-C), associamo una funzione (handler) che 
//  verra' eseguita alla ricezione del segnale.
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
void handlerCtrlC(int signum);
void handler(int signum);
 
//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]");

  // al segnale SIGNINT (CTRL-C) associamo 
  // la funzione (handler) handlerCtrlC
  signal(SIGINT, handlerCtrlC);
  signal(SIGUSR1, handler);
  signal(SIGUSR2, handler);
  
  //Ciclo infinito
  for(;;)
  {
    printf("\nCiclo infinito ...");
    sleep(20);
  }

  // Codice mai eseguito
  printf("\n\n[FINE]");
  exit(0);
}

//------------------------------------------------------------------------------------------
void handlerCtrlC(int signum) 
{
  printf("\n\n[handler] gestione dell'eccezione CTRL-C \n");
  printf("[handler] Ricevuto il segnale SIGINT [%d]; SIGINT=%d\n", signum, SIGINT); 
  printf("[handler] Fine del processo.\n\n");
  exit(1);
}
void handler(int signum) 
{
  printf("[handler] Ricevuto il segnale %d", signum); 
  if (signum == SIGUSR1) 
  {
    printf("[handler] Ricevuto il segnale SIGUSR1\n");
    printf("[handler] Accendi led ROSSO\n");
  }
  else if (signum == SIGUSR2) 
  {
    printf("[handler] Ricevuto il segnale SIGUSR2\n");
    printf("[handler] Accendi led VERDE\n");
  }
}

