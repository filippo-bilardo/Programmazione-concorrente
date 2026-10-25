//==========================================================================================
//
// Project:  sig11.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Esempio di risveglio durante l'esecuzione di una system call sleep,
//  mediante la ricezione di un segnale.
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
  printf("\nRicevuto il segnale %d\n",signum);  
  printf("Sono stato risvegliato!\n");
}

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]\n");
  signal(SIGINT, handler); 
  
  //Sleep 1000 secondi
  printf("Sleep(1000). premere CTRL-C per risvegliare il processo.\n");
  int k;
  k=sleep(1000);
  printf("Mancavano ancora %d sec.\n", k);

  printf("[FINE]\n\n");
  exit(0);
}
