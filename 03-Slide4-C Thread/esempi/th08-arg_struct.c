//==========================================================================================
//
// Project:  th08-arg_struct.c
// Date:     29/12/16
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Creazione di thread e passaggio di una struct come parametro. 
//  Dati di un tipo più grande di un intero devono essere passati per indirizzo. 
//  gcc -pthread th08-arg_struct.c
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   29/12/16     Versione iniziale
// 1.1   24/04/19     Aggiunta commenti
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
#define NUM_THREADS 4

struct thread_data {
  int thread_id;
  char *message;
};

//------------------------------------------------------------------------------------------
void *PrintHello(void *threadarg);

//------------------------------------------------------------------------------------------
int main(int argc , char *argv[])
{
  pthread_t thid[NUM_THREADS];
  struct thread_data td[NUM_THREADS];
  int iret, i;

  /* creazione dei thread */
  for(i=0; i<NUM_THREADS; i++)
  {
    printf("mainThread: creazione thread #%d\n", i);
    td[i].thread_id = i;
    td[i].message = "Hello Word";
    if (pthread_create(&thid[i], NULL,PrintHello,(void*)&td[i]))
    {
      printf("ERROR; from pthread_create()");
      exit(1);
    }
  }

  /* Wait till threads are complete before main continues. */
  for(i=0; i<NUM_THREADS; i++)
  {
    iret=pthread_join(thid[i], NULL);
    if(iret != 0) {
      fprintf(stderr, "join fallito %d\n", iret);
      exit(2);
    }
  }

  exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------------------
void *PrintHello(void *threadarg)
{
  struct thread_data *my_data;
  my_data = (struct thread_data *)threadarg;
  printf("PrintHello() : thread_id #%d Messagge %s\n", my_data->thread_id, my_data->message);
  pthread_exit(0);
}
