#include <stdio.h>
#include "fatal.h"
#include <stdlib.h>

void fatal(int assert, const char *message,  int status) {
  if (assert) {
    int i;
    i=0;
    while(message[i]!='\0') {
      putc(message[i], stderr);
      i++;
    }
    exit(status);
  }
}
