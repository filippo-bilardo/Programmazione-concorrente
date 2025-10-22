//==========================================================================================
//
// Project:  th04-memoriaprocesso.c
// Date:     31/03/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Tutti i thread di un processo condividono la stessa memoria dati.
//  In questo esempio vedremo la variabile a modificata da tutti i thread
//  c99 -pthread th04-memoriaprocesso.c 
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   29/12/16     Versione iniziale
// 1.1   31/03/21     Piccole modifiche al codice
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <pthread.h>  // thread	
 
//------------------------------------------------------------------------------------------
void *th_countdown(void *arg);

//------------------------------------------------------------------------------------------
int a;  //Variabile visibile da tutti i thread

//------------------------------------------------------------------------------------------
int main() 
{
  int exitstatus,err;
  pthread_t th1, th2;
  a=10; // Modifica della variabile dal mainThread
  
  err=pthread_create(&th1, NULL, th_countdown, "t1");
  if (err!=0) {
    perror("pthread_create error for thread th1\n");
    exit (1);
  } else {
    printf("main thread - creato thread01 tid=%d\n",(int)th1);
  }
  
  err=pthread_create(&th2, NULL, th_countdown, "t2");
  if (err!=0) {
    perror("pthread_create error for thread th2\n");
    exit (2);
  } else {
    printf("main thread - creato thread02 tid=%d\n",(int)th2);
  }
  
  sleep(6);
  printf("Main thread terminato.\n");
  exit(0);
}

//------------------------------------------------------------------------------------------
void *th_countdown(void *arg)
{
  int i;
  
  for(i=5;i>0;i--)
  {
    a++; // Modifica della variabile dal singolo Thread
    printf("Thread %s: Countdown %d..., il valore a e'=%d\n", (char*)arg, i, a);
    sleep(1);
  }
  printf("Thread %s terminato.\n", (char*)arg);
  pthread_exit(0);
}
