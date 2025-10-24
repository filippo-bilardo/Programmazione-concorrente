//==========================================================================================
//
// Project: th02-creation.c
// Date: 31/03/21
// Author: Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Ogni thread puo' creare altri thread.
// gcc -pthread th02-creation.c
//
// Ver Date     Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0 31/03/21 Versione iniziale
//
//==========================================================================================
#include <stdio.h>   // printf
#include <stdlib.h>  // exit
#include <unistd.h>  // sleep
#include <pthread.h> // thread
//------------------------------------------------------------------------------------------
void *threadSimple(void *arg);
void *threadCreator(void *vargp);
//------------------------------------------------------------------------------------------

int main() {
  int exitstatus;
  pthread_t th1, th2;

  pthread_create(&th1, NULL, threadSimple, "thread01");
  printf("main thread - creato thread01 tid=%d\n",(int)th1);

  pthread_create(&th2, NULL, threadCreator, "tcreator");
  printf("main thread - creato threadCreator tid=%d\n",(int)th2);

  sleep(1); //Aspetto che tutti i thread vengano eseguiti
  printf("Main thread terminato.\n");
  exit(0);
}
//------------------------------------------------------------------------------------------
void *threadSimple(void *arg) {
  printf("Thread %s Creato!\n", (char*)arg);
}
void *threadCreator(void *vargp) {
  printf("Thread threadCreator Creato!...\n");
  pthread_t tid;
  pthread_create(&tid, NULL, threadSimple, "thread02");
  printf("threadCreator - creato thread02 tid=%d\n",(int)tid);
}
