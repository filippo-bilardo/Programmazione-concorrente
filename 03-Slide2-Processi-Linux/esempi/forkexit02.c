
/* forkexit02.c - © FB - 7/12/20
 * La terminazione involontaria del processo tramite il
 * tentativo di una azione illegale.
 * echo $? per visualizzare l'exit status dell'ultimo comando
 */
#include <stdlib.h>  // exit
#include <stdio.h>   // printf

int main(int argc, char **argv) {

  int a=0;
  a=10/a;
  
  printf("questo codice non verrà eseguito");
  exit(1);
}

