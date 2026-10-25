//==========================================================================================
//
// Project:  sig02-handler.c
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
void gestoreCtrlC(int signum);

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]");

  // al segnale SIGNINT (CTRL-C) associamo 
  // la funzione (handler) gestoreCtrlC
  signal(SIGINT, gestoreCtrlC);
  
  //Ciclo infinito
  for(;;)
  {
    printf("\nCiclo infinito ...");
    sleep(1);
  }

  // Codice mai eseguito
  printf("\n\n[FINE]");
  exit(0);
}

//------------------------------------------------------------------------------------------
void gestoreCtrlC(int signum) 
{
  printf("\n\n[handler] gestione dell'eccezione CTRL-C \n");
  printf("[handler] Ricevuto il segnale SIGINT [%d]; SIGINT=%d\n", signum, SIGINT); 
  printf("[handler] Fine del processo.\n\n");
  exit(123);
}

