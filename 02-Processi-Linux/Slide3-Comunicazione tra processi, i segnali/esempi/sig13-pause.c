//==========================================================================================
//
// Project:  sig12.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Esempio risveglio dopo l'esecuzione di una system call pause
// mediante la ricezione di un qualsiasi segnale.
//
// Ver   Date        Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0   02/04/17     Versione iniziale
// 
//==========================================================================================
#include <stdio.h>    // printf
#include <stdlib.h>   // exit
#include <unistd.h>   // sleep 
#include <signal.h>   // signal, costanti

//------------------------------------------------------------------------------------------
void handler(int signum) 
{
  printf("\n[handler] Ricevuto il segnale %d\n",signum);  
}

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO] Avvio del processo.\n");
  
  // Associazione della funzione handler ai vari segnali
  printf("Associo la funzione handler a vari segnali.\n");
  signal(SIGINT, handler);     // 2  - Interrupt (ANSI). Action: exit
  signal(SIGUSR1, handler);    // 10 - User-defined signal 1 (POSIX). Action: exit
  signal(SIGUSR2, handler);    // 12 - User-defined signal 2 (POSIX). Action: exit

  // imposto l'invio del segnale di allarme dopo 5 sec
  printf("Metto in pausa il processo nell'attesa dei un segnale.\n");
  pause();

  // Fine
  printf("\n[FINE] Terminazione del processo.\n");
  exit(0);
}
