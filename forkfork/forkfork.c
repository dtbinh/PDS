#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "forkfork.h"

void forkfork (func_t f, void *arg) {

  int status;

  switch(fork()) {
  case -1 : printf("Peut pas créer un fils D:");
  case 0 : 
    switch (fork()) {
    case -1 : printf("Peut pas créer un petit-fils D:");
    case 0 : f(arg); printf("Je suis le petit-fils %d, mon père est %d\n",getpid(), getppid()); exit(EXIT_SUCCESS);
    default : printf("Le fils %d est mort\n",getpid()); exit(EXIT_SUCCESS);
    }
  default : sleep(6); printf("Je suis le père %d\n",getpid()); wait(&status); 
  }

}

void fonction_lente (void *arg) {
  sleep(3);
  printf("fonction_lente %d\n",(int) arg);
}
