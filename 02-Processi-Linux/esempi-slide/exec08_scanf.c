// 08. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep
#include <string.h>  // strcat, strcmp

int main(int argc, char *argv[]) 
{
  int esco=0; //0=Falso
  char pathname[30], cmd[30]; 
  pid_t pid;
  int wstatus, wstatus_high, wstatus_low;
  pid_t wpid;

  printf("\n[PADRE INIZIO] - Prima della fork - pid processo=%d, pid del padre=%d\n\n", getpid(), getppid());

  while(!esco)
  {
    // Lettura del comando da eseguire
    printf("\nInserire il comando da eseguire:"); 
    scanf(" %s",&cmd);
    pathname[0]=0; //pathname="";
    strcat(pathname,"/bin/"); 
    strcat(pathname,cmd);
    printf("\nEsecuzione del comando:'%s'; pathname:'%s'\n\n",cmd, pathname); 
    
    // Condizione d'uscita
    if (!strcmp("quit",cmd)) {printf("Fine esecuzione comandi\n"); esco=1; continue;}
    
    // Avvio del processo child ed esecuzione del comando
    printf("Fork\n"); 
    if ((pid = fork()) < 0) {
      perror("Creazione processo fallita!");
      abort();
    } else if (pid == 0) {
      //DoWorkInChild();
      printf("\nEsecuzione del comando:'%s'; pathname:'%s'\n\n",cmd, pathname); 
      execl(pathname, cmd, (char *)0);
      perror("exec fallita!\n");
      exit(112);
    }

    /* Attesa della terminazione del processo figlio. */
    wpid=wait(&wstatus);
    wstatus_high = wstatus >> 8;
    wstatus_low = wstatus & 0xFF;
    printf("\n[PADRE] - Child wpid=%ld, exited with wstatus_high=%d, wstatus_low=%d\n", (long)wpid, wstatus_high, wstatus_low);
  }
  
  printf("[PADRE FINE] - Termine del processo con pid=%d avente pid padre=%d\n",getpid(),getppid());
  printf("\n");
  exit(0);
}

