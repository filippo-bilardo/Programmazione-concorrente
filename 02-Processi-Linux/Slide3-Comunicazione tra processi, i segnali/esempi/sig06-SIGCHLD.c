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
  printf("[handler] Ricevuto il segnale %d\n",signum);
  
  int status;
  wait(&status);
  printf("[handler] Stato figlio:%d\n", status>>8);
  
  exit(0);
}

//------------------------------------------------------------------------------------------
void ProcessoPadre()
{
    signal(SIGCHLD,handler);

    //Ciclo infinito
    for(;;)
    {
      printf("[PADRE] Ciclo infinito ...\n");
      sleep(1);
    }
}

//------------------------------------------------------------------------------------------
void ProcessoFiglio()
{
    printf("[FIGLIO] Attesa 5 sec ...\n");
    sleep(5);
    exit(1); 
}


