
/* perror01.c - © FB - 08/12/20
 * Gestione degli errori con la system call perror()
 * - in caso di fallimento, ogni system call ritorna un
 *   valore negativo (tipicamente, -1)
 * - in aggiunta, UNIX prevede la variabile globale di sistema errno, 
 *   alla quale il kernel assegna il codice di errore generato dall’ultima 
 *   system call eseguita. 
 *   Per interpretarne il valore è possibile usare la funzione perror():
 *   + perror(“stringa”) stampa “stringa” seguita dalla descrizione del 
 *     codice di errore contenuto in errno
 */
#include <stdlib.h>  // exit
#include <stdio.h>   // printf, perror
#include <unistd.h>  // execl

int main(int argc, char *argv[]) 
{
  execl("/bin/prova", "prova", (char *)0);
  perror("exec fallita! A causa dell'errore: ");
  exit(1);
}
 