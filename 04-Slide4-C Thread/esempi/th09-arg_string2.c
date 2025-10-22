//==========================================================================================
//
// Project:  th09-arg_string2.c
// Date:     29/12/16
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Creazione di thread e passaggio di una struct come parametro. 
//  gcc -pthread th09-arg_string2.c
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   29/12/16     Versione iniziale
// 1.1   24/04/19     Aggiunta commenti
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
#define NUM_THREADS 4

struct thread_data
{
  int thread_id;
  int count;
  char *message;
};

//------------------------------------------------------------------------------------------
void *th_countdown(void * arg);

//------------------------------------------------------------------------------------------
int main(int argc , char *argv[])
{
  pthread_t thid[NUM_THREADS];
  struct thread_data td[NUM_THREADS];
  int iret, i;

  /* creazione dei thread */
  for(i=0; i<NUM_THREADS; i++)
  {
    printf("main() : creazione thread #%d\n", i);
    td[i].thread_id = i;
    td[i].count = i*2+1;
    td[i].message = "Hello Word 2";
    if (pthread_create(&thid[i], NULL,th_countdown,(void*)&td[i]))
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
      fprintf (stderr, "join fallito %d\n", iret);
      exit(2);
    }
  }

  exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------------------
void *th_countdown(void * arg)
{
  struct thread_data* p = (struct thread_data*) arg;
  int i;

  for(i=p->count;i>0;i--)
  {
    printf("Thread_id #%d Messagge %s: Countdown %d...\n", p->count, p->message, i);
    sleep(1);
  }
  printf("Thread terminato.\n");
  pthread_exit(0);
}

