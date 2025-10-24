//==========================================================================================
//
// Project:  sig10.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Esempio di invio segnali tramite la system call kill tra processi.
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
int cont=0;

//------------------------------------------------------------------------------------------
void handler(int signum) 
{
  printf("[handler] Proc. %d: ricevuti n. %d segnali %d\n", getpid(), ++cont, signum);
  if(cont==5) {
    printf("\n[handler] fine processo figlio. Proc. %d\n\n", getpid());
    exit(0);
  }
}

//------------------------------------------------------------------------------------------
void ProcessoPadre(int pidfiglio)
{
    int i;

    //Ciclo infinito
    for(i=10;i>0;i--)
    {
      printf("[PADRE] Proc. %d. iterazione %d ...\n", getpid(), i);
      kill(pidfiglio, SIGUSR1);
      sleep(1);
    }
}
void ProcessoFiglio()
{
    //Ciclo infinito
    for(;;)
    {
      printf("[FIGLIO] Proc. %d. Ciclo infinito ...\n", getpid());
      sleep(2);
    }
}


int main() 
{
  printf("\n[INIZIO] Proc. %d. \n\n", getpid());
  signal(SIGUSR1, handler);
  
  int pid;
  pid=fork();
  if (pid>0)
  { 
    ProcessoPadre(pid);
  }
  else if (pid==0)
  { 
    ProcessoFiglio();
  }
  else
  { 
    //Errore;
  }
  

  // Codice mai eseguito
  printf("\n[FINE] Proc. %d. \n\n", getpid());
  exit(0);
}
