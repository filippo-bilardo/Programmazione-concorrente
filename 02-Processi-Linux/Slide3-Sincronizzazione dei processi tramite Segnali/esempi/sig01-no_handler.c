//==========================================================================================
//
// Project:  sig01-no_handler.c
// Date:     21/03/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Se non e' definito nessun handler di segnale, quando il Kernel o la shell inviano 
//  il segnale, verra' eseguita l'operazione di default.
//  Provare a compilare ed eseguire il seguente codice.
//  Cosa succede se nella shell si preme CTRL-C ?
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   21/03/17     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 

int main()
{
  printf("\n[INIZIO]");

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
