//==========================================================================================
//
// Project:  sig08.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Segnali & exec 
//  
//  dopo un’exec, un processo:
//  • ignora gli stessi segnali ignorati prima di exec
//  • i segnali a default rimangono a default
//  ma
//  • i segnali che prima erano gestiti, vengono riportati a default
//  
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   02/04/17     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <signal.h>   // signal, costanti
#include <unistd.h>   // sleep 

//------------------------------------------------------------------------------------------
void handler(int signum) 
{
  printf("\n[handler] - Gestione del segnale CTRL-C \n");
}

//------------------------------------------------------------------------------------------
int main() 
{
  signal(SIGINT, handler);
  printf("Esecuzione della istruzione sleep(10).\n");
  sleep(10);
  
  signal(SIGINT, SIG_IGN);
  printf("Esecuzione della istruzione exex:sleep 10.\n");
  execl("/bin/sleep","sleep","10", (char *)0);
  
  // Codice mai eseguito
  printf("\n[FINE]\n");
  exit(0);
}
