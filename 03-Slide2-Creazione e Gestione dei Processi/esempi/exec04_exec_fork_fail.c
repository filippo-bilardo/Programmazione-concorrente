
/* exec04_exec_fork.c - © FB - 08/12/20  
 * Esempio di utilizzo della istruzione execl insieme all'istruzione fork
 */
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execl

int main(int argc, char *argv[]) 
{
  printf("\n[INIZIO] - Prima della fork - pid=%d, pid padre=%d\n\n", getpid(), getppid());

  int pid=fork();
  if (pid==0) { // Processo figlio 
    printf("[Figlio] - pid=%d, pid padre=%d\n", getpid(), getppid());
    execl("/bin/lsssssssss", "ls", "-a", (char *)0);
    printf("exec fallita!\n");
    exit(112);
  } else if (pid>0) { // Processo padre
    // Attesa della terminazione dei processi figli
    int wait_status, wait_status_high, wait_status_low;
    int wait_retvalue;
    wait_retvalue=wait(&wait_status);
    wait_status_high = wait_status >> 8;
    wait_status_low = wait_status & 0xFF;
    printf("\n[Padre] - wait_retvalue=%d, wait_status_high=%d, wait_status_low=%d\n", wait_retvalue, wait_status_high, wait_status_low);
  }
  else { // (pid<0) Errore nella creazione del processo 
    printf("Creazione fallita!");
  }

  printf("[FINE] - Termine del processo con pid=%d avente pid padre=%d\n", getpid(), getppid());
  exit(0);
}
 