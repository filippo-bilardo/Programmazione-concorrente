//==========================================================================================
//
// Project:  sig07.c
// Date:     02/04/17
// Author:   Filippo Bilardo
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  Esempio di gestione del segnale SIGCHLD.
//  
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
void handler(int signum);
void child1(void);
void child2(void);
void father(int childpid1,int childpid2);

//------------------------------------------------------------------------------------------
int main() 
{
  printf("\n[INIZIO] Processo %d\n", getpid());
  
  int pid1,pid2;
  pid1=fork();
  if(pid1>0)
  { 
    // Attività del padre
    pid2=fork();
    if(pid2>0)
    { 
      father(pid1,pid2);
    }
    else if(pid2==0) 
    {
      child2();
    }
    else // (pid2<0)
    {
      printf("Si e' verificato un'errore nella crazione del processo figlio2.\n");
    }
  }
  else if(pid1==0) 
  {
    child1();
  }
  else // (pid1<0)
  {
    printf("Si e' verificato un'errore nella crazione del processo figlio1.\n");
  }

  // Fine
  printf("[FINE] Processo %d\n\n", getpid());
  exit(0);
}

//------------------------------------------------------------------------------------------
void father(int childpid1,int childpid2)
{
    signal(SIGCHLD,handler);
    printf("[PADRE] Processo padre=%d. Figio1=%d, Figlio2=%d - AVVIO\n",getpid(),childpid1,childpid2);
    int i; for (i=0; i<10; i++) {sleep(1);} 
    printf("[PADRE] Processo padre=%d - TERMINAZIONE\n",getpid());
}

//------------------------------------------------------------------------------------------
void child1()
{
    printf("[FIGLIO1] Processo %d - AVVIO\n",getpid());
    int i; for (i=0; i<4; i++) {sleep(1);} 
    printf("[FIGLIO1] Processo %d - TERMINAZIONE\n",getpid());
    exit(1); 
}

//------------------------------------------------------------------------------------------
void child2()
{
    printf("[FIGLIO2] Processo %d - AVVIO\n",getpid());
    int i; for (i=0; i<7; i++) {sleep(1);} 
    printf("[FIGLIO2] Processo %d TERMINAZIONE\n",getpid());
    exit(2); 
}

//------------------------------------------------------------------------------------------
void handler(int signum) 
{ 
  int status;
  int pid=wait(&status);
  printf("[handler] Terminazione del processo figlio=%d stato=%d\n", pid, status>>8);
}
