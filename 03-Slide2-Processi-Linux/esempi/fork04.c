//==========================================================================================
// Project: fork04.c
// Date: 07/12/2020
// Author: Filippo Bilardo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Creazione di un processo figlio. 
//  Organizzazione migliore del codice rispetto al esempio precedente
//  - ogni variabile del figlio è inizializzata con il valore assegnatole dal padre 
//    prima della fork()
//  - il figlio nasce con lo stesso program counter del padre: la prima istruzione eseguita 
//    dal figlio è quella che segue immediatamente fork()
//    + il codice prima della fork viene eseguito solo dal processo padre
//    + il codice dopo della fork viene eseguito sia dal processo padre e dal processo figlio
//  - un processo ha sempre un padre. Se il padre termina prima del figlio, il padre diventa
//    il padre del padre e cosi via fino ad arrivare al processo "Init" pid=1
//
// Ver  Date      Comment
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 1.0  07/12/20  Versione iniziale
//
//==========================================================================================
//------------------------------------------------------------------------------------------
//=== Includes =============================================================================
//------------------------------------------------------------------------------------------
#include <stdio.h>   // printf
#include <unistd.h>  // getpid

//------------------------------------------------------------------------------------------
//=== Function prototypes ==================================================================
//------------------------------------------------------------------------------------------
void childProcess(void);
void fatherProcess(int childpid);

//------------------------------------------------------------------------------------------
//=== Local variables ======================================================================
//------------------------------------------------------------------------------------------
int op1=10, op2=20, ris;

//------------------------------------------------------------------------------------------
//=== Main =================================================================================
//------------------------------------------------------------------------------------------
int main() {

  printf("\n[INIZIO] - Esiste solo il processo padre, il padre del padre e' la shell \n");
  printf("[INIZIO] - Prima della fork - pid processo=%d, pid padre=%d\n",getpid(),getppid());

  // Creazione del processo figlio
  int pid=fork();
  if (pid==0) {
    // Esecuzione codice del processo figlio
    childProcess();
  } else if (pid>0) {
    // Esecuzione codice del processo padre
    fatherProcess(pid);
  } else { // pid<0
    // Errore
    printf("Creazione del processo figlio fallita!\n");
  }

  // Codice comune a processo padre e figlio
  printf("[FINE] - Termine del processo con pid=%d, ris=%d, padre=%d\n", getpid(), ris, getppid());
}
//------------------------------------------------------------------------------------------
//=== Local Functions ======================================================================
//------------------------------------------------------------------------------------------
void childProcess(void)
{
  // op1 e op2 inizializzate con il valore assegnato dal padre prima della fork()
  // ris ha valore diverso rispetto al processo padre
  ris=op1+op2; 
  printf("\n[Figlio] - Il risultato della somma di op1+op2=%d\n", ris);
  printf("[Figlio] - pid=%d, pid padre=%d\n", getpid(), getppid());
}

void fatherProcess(int childpid)
{
  ris=op1*op2;
  printf("\n[Padre] - pid=%d, pid padre=%d, pid figlio=%d\n", getpid(), getppid(), childpid);
  printf("[Padre] - Il risultato della moltiplicazione di op1*op2=%d\n", ris);
}

/* Possibile output:

fb@rpi2:~/sis2/01-fork $ gcc fork04.c
fb@rpi2:~/sis2/01-fork $ ./a.out

[INIZIO] - Esiste solo il processo padre, il padre del padre è la shell
[INIZIO] - Prima della fork - pid processo=5248, pid padre=4448
[Padre] - pid=5248, pid padre=4448, pid figlio=5249
[Padre] - Il risultato della moltiplicazione di op1*op2=200
[FINE] - Termine del processo con pid=5248, ris=200, padre=4448
[Figlio] - Il risultato della somma di op1+op2=30
[Figlio] - pid=5249, pid del processo padre=5248
[FINE] - Termine del processo con pid=5249, ris=30, padre=5248

fb@rpi2:~/sis2/01-fork $
*/