//==========================================================================================
//
// Project:  th05-arg_int.c
// Date:     31/03/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  I parametri vengono passati come puntatori a void (void* par)
//  Quindi prima di passarli al thread da creare va fatto l'opportuno casting.
//  gcc -pthread th05-arg_int.c
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   31/03/21     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <pthread.h>  // thread	
 
//------------------------------------------------------------------------------------------
void* threadFunc(void* par);
void* threadFuncAlt(void* par);

//------------------------------------------------------------------------------------------
int main() {
  
  pthread_t tid1, tid2, tid3, tid4; // Thread id

  // Data to pass to the thread function
  int data = 123;

  // Creates the thread
  pthread_create(&tid1, NULL, threadFunc, (void*)&data);
  // Creates the thread
  void* arg = (void*)data;  // Converts to void*
  pthread_create(&tid2, NULL, threadFuncAlt, arg);
  // Creates the thread
  pthread_create(&tid3, NULL, threadFuncAlt, (void*)533);
  // Creates the thread
  pthread_create(&tid4, NULL, threadFuncAlt, (void*)'a');
  
  // Attendo la terminazione dei threads
  //pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);
  pthread_join(tid4, NULL);
}

//------------------------------------------------------------------------------------------
void* threadFunc(void* par) {
  //int indirizzo = (int *)par;
  //int valore = *indirizzo;
  int value = *((int *)par);
  
  // Uses value as an integer
  printf("value = %d\n", value);
  
  // Terminates
  return (void*)0;
}
void* threadFuncAlt(void* par) {
  
  // Recupero l'argomento "int". Bisogna sapere che è un intero. 
  // Questo metodo può essere utilizzato solo per passaggio di argomenti di 
  // tipo intero.
  int value = (int)par;
  
  // Uses value as an integer
  printf("ThreadID=%d value = %d\n",pthread_self(), value);
  
  // Terminates
  return (void*)0;
}
