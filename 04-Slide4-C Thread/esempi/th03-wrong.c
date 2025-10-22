//==========================================================================================
//
// Project:  th03-wrong.c
// Date:     29/12/16
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Creazione di due thread in aggiunta al mainthread quando però termina il mainthread  
//  termina l'intero programma, quindi l'esecuzione dei thread viene interrotta. 
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
void *th_countdown(void *arg);

//------------------------------------------------------------------------------------------
int main() 
{
  int exitstatus, err;
  pthread_t th1, th2;
  
  err=pthread_create(&th1, NULL, th_countdown, "t1");
  if (err!=0) {
    perror("pthread_create error for thread th1\n");
    exit (1);
  } else {
    printf("main thread - creato thread01 tid=%d\n",th1);
  }
  
  err=pthread_create(&th2, NULL, th_countdown, "t2");
  if (err!=0) {
    perror("pthread_create error for thread th2\n");
    exit (2);
  } else {
    printf("main thread - creato thread02 tid=%d\n",th2);
  }
  
  sleep(3);
  printf("Main thread terminato.\n");
  exit(0);
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
