//==========================================================================================
//
// Project:  sig04.c
// Date:     21/03/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  In questo esempio viene modificato il comportamento di default
//  del segnale SIGINT (CTRL-C) e il programma non verra' terminato
//  successivamente vengono ignorati i successivi segnali SIGINT.
//   
//  eseguire il processo in background
//  $./a.out &
//  
//  inviare piu' volte il segnale CTRL-C al processo
//  $kill -SIGINT <pid processo>
//  
//  per terminare il processo 
//  $kill -9 <pid processo>
//  oppure 
//  $kill <pid processo>
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
void handler_signal_ctrlc(int signum) 
{
  printf("\n\n[handler] gestione del segnale CTRL-C \n");
  printf("[handler] Ripristino del comportamento di default\n\n");
  signal(SIGINT, SIG_IGN); /*USR1 torna a default */
  return;
}

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]\n");

  // associamo  la funzione handler_signal_ctrlc al segnale SIGNINT
  signal(SIGINT, handler_signal_ctrlc);
  
  //Ciclo infinito
  for(;;)
  {
    printf("\nCiclo infinito ...");
    sleep(15);
  }

  // Codice mai eseguito
  printf("\n\n[FINE]");
  exit(0);
}

