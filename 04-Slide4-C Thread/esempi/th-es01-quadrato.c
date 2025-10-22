//==========================================================================================
//
// Project:  th-es01-quadrato.c
// Date:     01/04/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Al main thread viene passato come parametro un numero, quindi, viene creato un thread 
//  per calcolare il quadrato del numero.
//  gcc -pthread th-es01-quadrato.c -o quadrato
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   01/04/21    Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
void *quadrato(void *param);  // the thread function prototype

//------------------------------------------------------------------------------------------
int main(int argc , char *argv[])
{
  /* variabili */
  //   // the thread identifier
  int par;

  /* Controllo dell'argomento passato */
  if (argc != 2) {
    fprintf(stderr,"Attenzione! Utilizzo: quadrato <integer value>\n");
    exit(1);
  }
  // Converto in intero il parametro stringa passato al main (atoi())
  par=atoi(argv[1]);
  if(par < 0) {
    fprintf(stderr,"Attenzione! L'argomento %d non puo' essere negativo\n", par);
    exit(2);
  }
  printf("mainthread - ");
  printf("argv[1]=%s, (int)argv[1]=%d, atoi(argv[1])=%d\n", argv[1], (int)argv[1], par);
  
  /* Creazione del thread */
  /* Attesa della terminazione del thread creato */
    
  /* Fine */
  printf("main thread terminato.\n");
  exit(0);
}

//------------------------------------------------------------------------------------------
void *quadrato(void *num)
{
  //inserite il vostro codice qui
}
