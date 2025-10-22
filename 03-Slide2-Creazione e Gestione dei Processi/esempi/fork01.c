
/**
 * fork01.c - © FB - 7/12/20
 * pid = identificativo del processo in esecuzione 
 * 
 * fb@rpi2:~/sis2/01-fork $ gcc fork01.c
 * fb@rpi2:~/sis2/01-fork $ ./a.out
 */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid

int main(int argc, char **argv) {

  printf("\nAvvio del processo con pid=%d\n", getpid());
}

//Possibile output:
//Avvio del processo con pid=12345