//==========================================================================================
//
// Project:  sig06.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Esempio di gestione del segnale SIGCHLD.
//  
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   02/04/17     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <signal.h>   // signal, costanti

//------------------------------------------------------------------------------------------
void handler(int signum);
void ProcessoPadre();
void ProcessoFiglio();

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO]\n");
  signal(SIGINT,handler);

  int PID;
  PID=fork();
  if (PID>0)
  { 
    ProcessoPadre();
  }
  else if (PID==0)
  { 
    ProcessoFiglio();
  }
  else
  { 
    //Errore;
  }

  // Codice mai eseguito
  printf("\n\n[FINE]");
  exit(0);
}

//------------------------------------------------------------------------------------------
void handler(int signum) 
{
  printf("\n[handler] Ricevuto il segnale. Processo %d %d\n",signum,getpid());
  
  int status;
  wait(&status);
  printf("[handler] Stato figlio:%d. Processo %d\n", status>>8, getpid());
  
  printf("[handler] Terminazione del processo %d.\n\n",getpid());
  exit(3);
}

//------------------------------------------------------------------------------------------
void ProcessoPadre()
{
    //Ciclo infinito
    for(;;)
    {
      printf("[PADRE] Ciclo infinito ... Processo %d\n",getpid());
      sleep(3);
    }
}

//------------------------------------------------------------------------------------------
void ProcessoFiglio()
{
    //Ciclo infinito
    for(;;)
    {
      printf("[FIGLIO] Ciclo infinito ... Processo %d\n",getpid());
      sleep(1);
    }
}


