//==========================================================================================
//
// Project:  th11-ret_int.c
// Date:     01/04/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Thread che restituisce un intero al main thread.
//  gcc -pthread th11-ret_int.c
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   01/04/21     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
void *thread01(void *arg);

//------------------------------------------------------------------------------------------
int main() 
{
  int err;
  pthread_t th1;
  //Thread retval
  void* pretval;
  int retval;
  
  /* creazione del thread */
  err=pthread_create(&th1, NULL, thread01, "t1");
  if (err<0) {
    perror("pthread_create error for thread th1\n");
    exit (1);
  } else {
    printf("main thread - creato thread01 tid=%d\n",th1);
  }
  
  /* Waits (joins) the thread */
  printf("main thread - attendo la terminazione del thread creato\n");
  err=pthread_join(th1, &pretval);
  if(err<0) {
    perror("join fallito\n");
    exit (3);
  }
  
  /* Uses the return value */
  retval = (int)(pretval);
  printf("main thread - Thread returned %d\n", retval);  
  
  /* Exit */
  printf("main thread terminato.\n");
  exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------------------
void *thread01(void *arg)
{
  printf("Thread %s terminato.\n", (char*)arg);
  pthread_exit((void*)23);
}
