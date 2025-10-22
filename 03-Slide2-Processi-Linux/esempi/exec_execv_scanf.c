// 09. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execve
#include <string.h>  // strcat, strcmp


int main(int argc, char *argv[]) 
{
  char *pathname = "/bin/ls";
  char *MyArgV[10];
  MyArgV[0]="ls";
  MyArgV[1]="-l";
  MyArgV[2]="..";
  MyArgV[3]=(char *)0;
  
  printf("esecuzione del comando: execv(%s ,%s, %s, %s)\n\n",pathname, MyArgV[0],MyArgV[1],MyArgV[2]);

  execv(pathname, MyArgV);
  perror("exec fallita! A causa dell'errore: ");
  exit(112);
}