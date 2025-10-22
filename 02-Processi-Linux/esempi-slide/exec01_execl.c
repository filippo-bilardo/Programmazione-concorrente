
/* exec01_execl.c - © FB - 12/03/17  
 * Esempio di utilizzo della istruzione execl
 * int execl(char *pathname, char *arg0, ..char *argN, (char*)0);
 * - pathname è il nome (assoluto o relativo) dell’eseguibile da caricare
 * - arg0 è il nome del programma (argv[0]) 
 * - arg1, …, argN sono gli argomenti da passare al programma
 * - (char *)0 è il puntatore nullo che termina la lista
 */
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // execl

int main(int argc, char **argv) {

  execl("/bin/ls", "cmd_ls", "-a", (char *)0);
  printf("exec fallita!\n");
  exit(112);
}
 
