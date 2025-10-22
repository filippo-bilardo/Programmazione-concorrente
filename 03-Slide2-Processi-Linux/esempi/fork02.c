
/**
 * fork02.c - © FB - 7/12/20 
 * e' possibile vedere il pid dei processi in esecuzione tramite il comando ps -u
 * $./a.out & #avvia il programma in background 
 * digitare fg e poi CTRL-C per terminare il processo in background
 */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid

int main(int argc, char **argv) {

  printf("\nAvvio del processo con pid=%d\n", getpid());
  for(;;) {;}
}

/* Possibile output:

fb@rpi2:~/sis2/01-fork $ gcc fork02.c
fb@rpi2:~/sis2/01-fork $ ./a.out &
[1] 4623
fb@rpi2:~/sis2/01-fork $ Avvio del processo con pid=4623
fb@rpi2:~/sis2/01-fork $ ps -u
USER   PID  %CPU %MEM   VSZ  RSS TTY      STAT START   TIME COMMAND
fb    4448  0.0  0.4  6244 4304 pts/3    Ss   18:00   0:00 -bash
fb    4623 96.4  0.0  1676  316 pts/3    R    18:06   0:09 ./a.out
fb    4630  0.0  0.2  4740 2172 pts/3    R+   18:06   0:00 ps -u

*/
