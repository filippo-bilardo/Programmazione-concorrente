
/* forkexit01.c - © FB - 7/12/20
 * La terminazione volontaria del processo avviene con 
 * l'esecuzione dell’ultima istruzione oppure con la chiamata 
 * alla funzione exit() 
 * echo $? per visualizzare l'exit status dell'ultimo comando
 */
#include <stdlib.h>  // exit

int main(int argc, char **argv) {
  
  exit(253);
}

