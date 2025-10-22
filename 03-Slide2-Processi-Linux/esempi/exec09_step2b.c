// 09. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execve
#include <string.h>  // strcat, strcmp

int splitString(char *string, const char *delim) {

  int argNr=0,argStrIdx=0;
  char arg[10][50];  

  printf("string='%s', delim='%c'\n\n",string, *delim);
  do  
  {
    printf("string=%d, argNr=%d, argStrIdx=%d, *string='%c'\n",string, argNr,argStrIdx,*string);
    arg[argNr][argStrIdx]=*string;
    if(*string==*delim || *string==0) {
      arg[argNr][argStrIdx]=0;
      printf("len=%d, argv[%d]='%s'\n\n",argStrIdx, argNr, arg[argNr]); 
      argNr++; 
      argStrIdx=-1;
    }
    argStrIdx++;
  } 
  while(*string++ != 0);
  return 0;
}

int splitExample2(char *st, const char *delim) {
  char *ch;
  ch = strtok(st, " ");
  while (ch != NULL) {
    printf("%s\n", ch);
    ch = strtok(NULL, " ,");
  }
  return 0;
}

int main(int argc, char *argv[]) 
{
  char cmd[50]; 

  // Lettura del comando da eseguire
  printf("\n>>>"); 
  scanf("%[^\n]s",&cmd);  

  // Scomposizione del comando
  splitString(cmd, " ");
  splitExample2(cmd, " ");

  exit(0);
}

