
/* fork03.c - © FB - 7/12/20  
 * Creazione di un processo figlio 
 * All'interno del blocco if i processi padre e figlio
 * eseguone codice differente. 
 * Perchè la fork restitusce 0 al codice del processo figlio e 
 * un numero positvo al processo padre (corrisponde al PID del figlio)
 */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, fork

int main() {
  
  printf("Codice eseguito padre! (pid: %d)\n", getpid());
  for(;;) {;}
  
  
  int pid = fork(); // viene creato il processo figlio
  if (pid==0) {
    /* codice figlio */
    printf("Sono il figlio! (pid: %d)\n", getpid());
  } else if (pid>0) {
    /* codice padre */
    printf("Sono il padre! (pid: %d)\n", getpid());
  } else { // pid<0
    printf("Creazione del processo figlio fallita!\n");
  }
  //Se vogliamo differenziare il comportamento dei processi padre e figlio
  //dobbiamo sfruttare questo blocco if e la variabile pid
  
  printf("Codice comune a padre e figlio! (pid: %d)\n", getpid());

  for(;;) {;}
}

