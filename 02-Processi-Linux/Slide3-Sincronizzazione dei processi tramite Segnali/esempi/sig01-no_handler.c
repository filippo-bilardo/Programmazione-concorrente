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
//  gcc sig01-no_handler.c  # compilazione
//  ./a.out                 # esecuzione
//  # cercare il PID del processo in esecuzione con il comando "ps" 
//  # oppure con il comando "pidof a.out"
//  # adesso possiamo inviare un segnale al processo in esecuzione 
//  # aprendo un altra shell, ad esempio:
//  kill -SIGINT <pid>      # invio del segnale SIGINT al processo
//  # oppure possiamo terminare il processo in esecuzione semplicemente premendo CTRL-C 
//  # nella shell, in questo modo inviamo il segnale SIGINT al processo in foreground
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
