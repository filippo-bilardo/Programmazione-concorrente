
/* exec02_nomagicnumbers.c - © FB - 12/03/17  
 * Esempio di utilizzo della istruzione execl
 * con definizione dei parametri da passare all'instruzione 
 * execl prima di richiamare la funzione.
 */
#include <stdlib.h>  // exit
#include <stdio.h>   // printf
#include <unistd.h>  // execl

int main(int argc, char *argv[]) {
  
  char *pathname="/bin/ls";
  char *myarg[3]={"cmdls", "-a", (char *)0};

  execl(pathname, myarg[0], myarg[1], myarg[2]);
  printf("exec fallita!\n");
  exit(112);
}

