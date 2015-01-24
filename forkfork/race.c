#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(void) {
  int i,j,status;
  for (i = 0 ; i < 10 ; i++) {
    switch(fork()) {
    case -1 : printf("Peut pas créer un fils D:"); break;
    case 0 :
      for (j = 0 ; j < 100000000 ; j++) ;
      printf("100M du %d (père : %d)\n",getpid(),getppid());
      for (j = 0 ; j < 100000000 ; j++) ;
      printf("200M du %d (père : %d)\n",getpid(),getppid());
      exit(i);
    default: break;
    } 
  }
  for (i = 0 ; i < 10 ; i++) {
    wait(&status); 
  }
  return 0;
}
