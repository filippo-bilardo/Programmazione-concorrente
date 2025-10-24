//==========================================================================================
//
// Project:  th06-arg_double.c
// Date:     31/03/21
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  I parametri vengono passati come puntatori a void (void* par)
//  Un puntatore a void è un intero. 
//  Dati di un tipo più grande di un intero devono essere passati per indirizzo. 
//  Si deve eseguire una conversione tra puntatori.
//  gcc -pthread th06-arg_double.c
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

//------------------------------------------------------------------------------------------
int main() {
  
  pthread_t tid1, tid2; // Thread id

  // Data to pass to the thread function
  double data = 1.23;
  // Takes the address of data and converts it to void*
  void* arg = (void*)&data;
  // Data to pass to the thread function
  double data2 = 3.3;

  // Creates the thread
  pthread_create(&tid1, NULL, threadFunc, arg);
  // Creates the thread
  pthread_create(&tid2, NULL, threadFunc, (void*)&data2);
  
  // Attendo la terminazione dei threads
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
}

//------------------------------------------------------------------------------------------
void* threadFunc(void* par) {
  
  // Catches the "double" argument.
  // Must know that it is a double. Type checking is broken
  double* pvalue = (double*)par;
  double value = *pvalue;
  
  // Uses value as an integer
  printf("value = %f\n", value);
  printf("value = %f\n", *(double*)par);
  
  // Terminates
  return (void*)0;
}
