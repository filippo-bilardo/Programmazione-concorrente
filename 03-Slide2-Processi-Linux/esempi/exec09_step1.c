// 09. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execve
#include <string.h>  // strcat, strcmp

int main(int argc, char *argv[]) 
{
  int argNr=0,argStrIdx=0;
  char arg[10][50]; 
  char cmd[50]; 
  char *cm;

  // Lettura del comando da eseguire
  printf("\nInserire il comando da eseguire:"); 
  scanf("%[^\n]s",&cmd);  

  // Scomposizione del comando SplitString
  cm=cmd;
  printf("cm=%s\n\n",cm);
  do  
  {
    printf("cm=%d, argNr=%d, argStrIdx=%d, *cm=%c\n",cm, argNr,argStrIdx,*cm);
    arg[argNr][argStrIdx]=*cm;
    if(*cm==' ' || *cm==NULL) {
      arg[argNr][argStrIdx]=NULL;
      printf("len=%d, argv[%d]='%s'\n\n",argStrIdx, argNr, arg[argNr]); 
      argNr++; 
      argStrIdx=-1;
    }
    argStrIdx++;
  } 
  while(*cm++ != NULL);

  exit(0);
}

