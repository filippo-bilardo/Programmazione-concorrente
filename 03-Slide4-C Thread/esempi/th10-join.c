//==========================================================================================
//
// Project:  th10-join.c
// Date:     29/12/16
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Prima di terminare l'intero processo il main thread aspetta 
//  che i singoli thread siano stati completati.
//  c99 -pthread th10-join.c
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   29/12/16     Versione iniziale
// 1.1   24/04/19     Aggiunta commenti
// 1.2   31/03/21     Piccole modifiche al codice
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
void *th_countdown(void *arg);

//------------------------------------------------------------------------------------------
int main() 
{
  int exitstatus, err;
  pthread_t thID1, thID2;
  
  /* creazione dei thread */
  err=pthread_create(&thID1, NULL, th_countdown, "t1");
  if (err<0) {
    perror("pthread_create error for thread thID1\n");
    exit (1);
  } else {
    printf("main thread - creato thread01 tid=%d\n",thID1);
  }
  
  err=pthread_create(&thID2, NULL, th_countdown, "t2");
  if (err<0) {
    perror("pthread_create error for thread th2\n");
    exit (2);
  } else {
    printf("main thread - creato thread02 tid=%d\n",thID2);
  }
  
  /* Wait till threads are complete before main continues.  */
  /* Unless we run the risk of executing an exit which will terminate */
  /* the process and all threads before the threads have completed.   */
  printf("main thread - attendo la terminazione dei thread creati\n");
  err=pthread_join(thID1, NULL);
  if(err!=0) {
    perror("join fallito\n");
    exit (3);
  }
  err=pthread_join(thID2, NULL);
  if(err!=0) {
    perror("join fallito\n");
    exit (3);
  }
  
  printf("main thread terminato.\n");
  exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------------------
void *th_countdown(void *arg)
{
  for(int i=5;i>0;i--)
  {
    printf("Thread %s: Countdown %d...\n", (char*)arg, i);
    sleep(1);
  }
  printf("Thread %s terminato.\n", (char*)arg);
  pthread_exit(0);
}
