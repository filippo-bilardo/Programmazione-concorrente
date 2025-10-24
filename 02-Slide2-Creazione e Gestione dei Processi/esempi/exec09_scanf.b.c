// 09. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execve
#include <string.h>  // strcat, strcmp


int splitString(char *cmd, char **argv) 
{
  int argNr=0,argStrIdx=0;
  
  while(*cmd != 0) {
    argv[argNr][argStrIdx++]=*cmd;
    if(*cmd=' ') {printf("%s\n",argv[argNr]); argNr++; argStrIdx=0;}
    cmd++; 
  }
  
  //printf("esecuzione del comando: execv(%s ,%s, %s, %s)\n\n",MyArgV[0],MyArgV[1],MyArgV[2]);
}

int main(int argc, char *argv[]) 
{
  int argNr=0,argStrIdx=0;
  char *pathname = "/bin/ls";
  char MyArgV[10][50]; 
  char arg[10][50]; 
  char cmd[50]; 
  char *cm;
  int notexit=1;
  pid_t pid;
  int wstatus, wstatus_high, wstatus_low;
  pid_t wpid;

  printf("\n[PADRE INIZIO] - Prima della fork - pid processo=%d, pid del padre=%d\n\n", getpid(), getppid());

  while(notexit)
  {
    // Lettura del comando da eseguire
    printf("\n>>>"); 
    scanf("%[^\n]s",&cmd);  
    //splitString(cmd, MyArgV);
 
    cm=cmd;
    printf("\ncm=%s, cmd=%s\n\n",cm,cmd);
    do  
    {
      printf("cm=%d, argNr=%d, argStrIdx=%d, *cm=%c\n",cm, argNr,argStrIdx,*cm);
      arg[argNr][argStrIdx]=*cm;
      if(*cm==' ' || *cm==0) {
        arg[argNr][argStrIdx]=0;
        printf("len=%d, argv[%d]='%s'\n\n",argStrIdx, argNr, arg[argNr]); 
        argNr++; 
        argStrIdx=-1;
      }
      argStrIdx++;
    } 
    while(*cm++ != 0);
    
    exit(0);
    
    // Condizione d'uscita
    if (!strcmp("quit",MyArgV[0])) {printf("Fine esecuzione comandi\n"); notexit=0; continue;}
    
    // Avvio del processo child ed esecuzione del comando
    printf("Fork\n"); 
    if ((pid = fork()) < 0) {
      perror("Creazione processo fallita!");
      abort();
    } else if (pid == 0) {
      //DoWorkInChild();
      //printf("\nEsecuzione del comando:'%s'; pathname:'%s'\n\n",cmd, pathname); 
      //execl(pathname, cmd, (char *)0);
      //perror("exec fallita!\n");
      exit(112);
    } else {
      /* Attesa della terminazione del processo figlio. */
      wpid=wait(&wstatus);
      wstatus_high = wstatus >> 8;
      wstatus_low = wstatus & 0xFF;
      printf("\n[PADRE] - Child wpid=%ld, exited with wstatus_high=%d, wstatus_low=%d\n", (long)wpid, wstatus_high, wstatus_low);
    }
  }
  
  printf("[PADRE FINE] - Termine del processo con pid=%d avente pid padre=%d\n",getpid(),getppid());
  printf("\n");
  exit(0);
}

