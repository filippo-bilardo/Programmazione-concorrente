
/* fork02.c - © FB - 7/12/20 
 * e' possibile vedere il pid dei processi in 
 * esecuzione tramite il comando ps -u
 * $./a.out & #avvia il programma in background 
 * digitare fg e poi CTRL-C per terminare il processo in background 
 */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid

int main(int argc, char **argv) {

  printf("\nAvvio del processo con pid=%d\n", getpid());
  for(;;) {;}
}

