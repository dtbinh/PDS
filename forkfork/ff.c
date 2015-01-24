#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "forkfork.h"

int main(int argc, char* argv[]) {
  void* inutile = malloc(1);
  forkfork(fonction_lente,inutile);
  return 0;
}
