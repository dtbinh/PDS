#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZ 1048576

void create (int size) {
  int i,lus = 0;
  char tampon[BUF_SIZ];
  
  for(i = 0; i < size; i++) {
    tampon[lus] = (i % 26) + 'a';
    lus++;
    if (lus == BUF_SIZ) {
      write(STDOUT_FILENO, &tampon , lus); 
      lus = 0;
    }
  }
  write(STDOUT_FILENO, &tampon , lus);
}

int main(int argc, char* argv[]) {
  create(atoi(argv[1]));
  return 0;
}
