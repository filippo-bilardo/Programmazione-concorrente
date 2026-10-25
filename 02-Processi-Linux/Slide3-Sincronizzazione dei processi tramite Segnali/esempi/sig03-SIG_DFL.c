//==========================================================================================
//
// Project:  sig03-SIG_DFL.c
// Date:     21/03/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  In questo esempio viene modificato il comportamento di default del segnale 
//  SIGINT (CTRL-C) e il programma non verra' terminato, successivamente viene ripristinato 
//  il comportamento di default.
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
  printf("\n\n[handler] gestione dell'eccezione CTRL-C \n");
  printf("[handler] Ripristino del comportamento di default\n\n");
  signal(SIGINT, SIG_DFL); /*USR1 torna a default */
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
    printf("Ciclo infinito ...\n");
    sleep(1);
  }

  // Codice mai eseguito
  printf("\n[FINE]\n");
  exit(0);
}

