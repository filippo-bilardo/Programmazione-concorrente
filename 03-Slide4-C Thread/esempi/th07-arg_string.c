//==========================================================================================
//
// Project:  th07-arg_string.c
// Date:     24/04/19
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Creazione di thread e passaggio di una stringa come parametro. 
//  I parametri vengono passati come puntatori a void (void* par)
//  Dati di un tipo più grande di un intero devono essere passati per indirizzo. 
//  Il nome del vettore stringa e' un indirizzo.
//  gcc -pthread th07-arg_string.c
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
#include <pthread.h>  // thread	

//------------------------------------------------------------------------------------------
void *print_message_function(void *ptr);

//------------------------------------------------------------------------------------------
int main(int argc , char *argv[])
{
  pthread_t thread1, thread2;
  const char *message1 = "Thread 1 message";
  char message2[30] = "Thread 2 message";
  int  iret;
  
  /* Create independent threads each of which will execute function */
  iret = pthread_create( &thread1, NULL, print_message_function, (void*)message1);
  if(iret<0) {
    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
    exit(EXIT_FAILURE);
  } else {
    //printf("pthread_create() for thread 1 returns: %d\n",iret);
  }
  
  iret = pthread_create( &thread2, NULL, print_message_function, (void*)message2);
  if(iret<0) {
    fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
    exit(EXIT_FAILURE);
  } else {
    //printf("pthread_create() for thread 2 returns: %d\n",iret);
  }
  
  /* Wait till threads are complete before main continues. Unless we  */
  /* wait we run the risk of executing an exit which will terminate   */
  /* the process and all threads before the threads have completed.   */
  iret=pthread_join( thread1, NULL);
  if(iret != 0) {
    fprintf (stderr, "join fallito %d\n", iret);
    exit(4);
  }
  iret=pthread_join( thread2, NULL);
  if(iret != 0) {
    fprintf (stderr, "join fallito %d\n", iret);
    exit(4);
  }
  
  exit(EXIT_SUCCESS);
}

//------------------------------------------------------------------------------------------
void *print_message_function( void *ptr )
{
  char *message;
  message = (char *)ptr;
  
  printf("%s \n", message);
  //printf("%s \n", (char *)ptr);
}
