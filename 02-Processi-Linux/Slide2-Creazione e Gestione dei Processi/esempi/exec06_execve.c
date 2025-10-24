
/* exec06_execve.c - © FB - 12/03/17  
 * Esempio di utilizzo della istruzione execve
 * int execve(char *pathname, char *argV[], , char *env[]);
 * - pathname e' il nome (assoluto o relativo) dell’eseguibile da caricare
 * - argV e' il vettore degli argomenti del programma da eseguire
 * - env e' il vettore delle variabili di ambiente da sostituire all’ambiente
 *   del processo (contiene stringhe del tipo “VARIABILE=valore”)
 */
#include <stdlib.h>  // exit
#include <unistd.h>  // execve

int main(int argc, char *argv[]) {
  char *pathname = "/bin/ls";
  char *myenv[3]={"USER=anna", "PATH=/home/anna/d1", (char *)0};
  char *myargv[]={"ls", ".", (char *)0};
  
  execve(pathname, myargv, myenv);
  perror("exec fallita! A causa dell'errore: ");
  exit(112);
}

