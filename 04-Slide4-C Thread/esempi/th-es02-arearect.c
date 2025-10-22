//==========================================================================================
//
// Project:  th-es02-arearect.c
// Date:     01/04/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Al main thread vengono passati come parametri base e altezza
//  Creare un thread a cui passare una struct con i valori base e altezza 
//  Il thread deve calcolare l'area e restituirla al mainthread come intero.
//  gcc -pthread th-es02-arearect.c -o arearect
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
//------------------------------------------------------------------------------------------
void *arearect(void *param);  // the thread function prototype

//------------------------------------------------------------------------------------------
int main(int argc , char *argv[])
{
  /* variabili */
  //   // the thread identifier
  int retcode;
  int par1,par2;

  /* Controllo dell'argomento passato */
  if (argc != 3) {
    fprintf(stderr,"Attenzione! Utilizzo: arearect <base> <altezza>\n");
    exit(1);
  }
  // Converto in intero il parametro stringa passato al main (atoi())
  par1=atoi(argv[1]);
  if(par1 <= 0) {
    fprintf(stderr,"Attenzione! La base (%d) non puo' essere negativa o nulla\n", par1);
    exit(2);
  }
  par2=atoi(argv[2]);
  if(par2 <= 0) {
    fprintf(stderr,"Attenzione! L'altezza (%d) non puo' essere negativa o nulla\n", par2);
    exit(3);
  }
  printf("mainthread - base=%d, altezza=%d\n", par1, par2);
  
  /* Creazione del thread */
  /* Attesa della terminazione del thread creato 
   * recupero e stampadel risultato restituito */
    
  /* Fine */
  printf("main thread terminato.\n");
  exit(0);
}

//------------------------------------------------------------------------------------------
void *arearect(void *num)
{
  //inserite il vostro codice qui
}
