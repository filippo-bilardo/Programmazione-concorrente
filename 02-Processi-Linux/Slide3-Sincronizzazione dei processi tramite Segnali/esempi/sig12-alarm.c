//==========================================================================================
//
// Project:  sig12.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Esempio di recezione del segnale SIGALRM impostato tramite la system call alarm.
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
  printf("[handler] Ricevuto il segnale %d\n",signum);  
  printf("[handler] Terminazione processo.\n\n");
  exit(1);
}

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]\n");
  
  // Associazione della funzione handler al segnale SIGALRM
  printf("Associo la funzione handler al segnale SIGALRM.\n");
  signal(SIGALRM, handler);

  // imposto l'invio del segnale di allarme dopo 5 sec
  printf("Imposto l'invio del segnale di allarme dopo 5 sec.\n");
  alarm(5);

  //Ciclo infinito
  int i=1;
  for(;;)
  {
    printf("Ciclo infinito (%d) ...\n",i++);
    sleep(1);
  }

  // Codice mai eseguito
  printf("\n[FINE]\n");
  exit(0);
}
