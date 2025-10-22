// 09. Esecuzione multipla di comandi tramite scanf
//
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // getpid, getppid, fork, sleep, execve
#include <string.h>  // strcat, strcmp

char *strsep(char **stringp, const char *delim) {
  if (*stringp == NULL) { return NULL; }
  char *token_start = *stringp;
  *stringp = strpbrk(token_start, delim);
  if (*stringp) {
    **stringp = '\0';
    (*stringp)++;
  }
  return token_start;
}

int splitExample(void) {
  char st[] ="Where there is will, there is a way.";
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
  int argNr=0,argStrIdx=0;
  char arg[10][50]; 
  char cmd[50]; 
  char *cm;

  splitExample();
  
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
}

