
/* fork01.c - © FB - 7/12/20
 * pid = identificativo del processo in esecuzione */
#include <stdio.h>   // printf
#include <unistd.h>  // getpid

int main(int argc, char **argv) {

  printf("\nAvvio del processo con pid=%d\n", getpid());
}

