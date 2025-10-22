// 07. Esercizi con le stinghe ed istruzione execv
// $./a.out
// $./a.out ps ps "-au"
// $./a.out ls ls "-ah"
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc,char *argv[])
{
  // Controllo dei parametri in ingresso
  if(argc<3) {
    printf("Numero di argomenti [%d] non valido!\n",argc);
    exit(1);
  }
  printf("Numero di argomenti passati, argc=%d\n",argc);
  // Definizioni del vettore *myarg
  char *myarg[10];    // vettore di puntatori a strighe, vettore di vettori di char
  //char **myarg;     // vettore di strighe, vettore di vettori di char
  //char myarg[10][30];
  int i;
  for(i=0; i<argc; ++i) {
    //strcpy(myarg[i], argv[i]);  //Errore myarg è un vettore di puntatori
    if(i<2) {
      printf("argv[%d]:=%s\n",i,argv[i]);
    } else {
      myarg[i-2] = argv[i];
      printf("argv[%d]:=%s - myarg[%d]:=%s\n",i,argv[i],i-2,myarg[i-2]);
    }
  }
  myarg[i-1]=0;  // le istruzioni exec richiedono che l'ultimo elemento sia 0
  
  // Definizioni delle variabili
  // Riservare in memoria uno spazio
  // In C le srtinghe sono vettori di char che terminano con uno 0;
  char pathname[50];  // una stringa, vettore di char
  // Definizione e Inizializzazione del vettore di char 
  char pathbase[]="/bin/";
  printf("\npathbase=%s\n",pathbase);
  // Inizializziamo la variabile pathname come stringa vuota
  // pathname = ""  
  pathname[0]=0;  
  printf("pathname=%s\n",pathname);
  // Concatenzione di stringhe
  // pathname = pathname+pathbase
  strcat(pathname,pathbase);
  printf("pathname=%s\n",pathname);
  printf("argv[1]=%s\n",argv[1]);
  // pathname = pathname+argv[1]
  strcat(pathname,argv[1]);
  printf("pathname=%s\n",pathname);

  // Esecuzione dell'istruzione execv
  printf("\n");
  execv(pathname,myarg);
  
  return 0;
}  
  