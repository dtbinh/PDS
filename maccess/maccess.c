#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "fatal.h"

#define ERROR "maccess: illegal option -- t\nusage: maccess [name of file] [-rwx] [-v]\n"

int main(int argc, const char* argv[]) {  
  char option;
  fatal((argc != 3) && (argc != 4),ERROR,2);
  if ((option =  argv[2][1]) == 'r') {
    if (access(argv[1],R_OK) == 0) {
      printf("Oui, on peut le lire !\n"); 
      return 0;
    }
  }

  if ((option =  argv[2][1]) == 'w') {
    if (access(argv[1],W_OK) == 0) {
      printf("Oui, on peut y écrire !\n"); 
      return 0;
    }
  }
  
  if ((option =  argv[2][1]) == 'x') {
    if (access(argv[1],X_OK) == 0) {
      printf("Oui, on peut l'éxecuter!\n"); 
      return 0;
    } 
  }

  if ((argc == 4) && (argv[3][1] == 'v')) {
    printf("%s\n",strerror(errno)); 
    return -1;
  } 

  return 0;
}
