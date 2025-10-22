
/* fork03b.c - © FB - 7/12/20  
 * Creazione di un processo figlio 
 * i processi padre e figlio procedono in modo concorrente
 */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, fork

int main() {
  
  printf("Codice padre! (pid: %d)\n", getpid());
  int pid = fork();
  if (pid==0) {
    /* codice figlio */
	for(int i=0; i<30; i++) {
		printf("%d - Sono il figlio! (pid: %d)\n", i, getpid());
		sleep(1);
	}
  } else if (pid>0) {
    /* codice padre */
	for(int i=0; i<30; i++) {
		printf("%d - Sono il padre! (pid: %d)\n", i, getpid());
		sleep(1);
	}
  } else { // pid<0
    printf("Creazione del processo figlio fallita!\n");
  }
  printf("Codice comune a padre e figlio! (pid: %d)\n", getpid());
}

