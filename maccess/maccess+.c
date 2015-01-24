#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char* argv[]) {
  int opt;  
  int result;
  char* file;
  result = 1;
  file = argv[1];
  while ((opt = getopt(argc, argv, "rxwv")) != -1) {
    switch(opt) {
    case 'r' : 
      errno = 0;
      if ((result = access(file,R_OK)) == 0) 
	printf("Can be read\n"); 
      else  
	printf("Can't read : "); 
      break;
    case 'w' : 
      errno = 0;
      if ((result = access(file,W_OK)) == 0) 
	printf("Can write\n"); 
      else  
	printf("Can't write : "); 
      break;
    case 'x' : 
      errno = 0;
      if ((result = access(file,X_OK)) == 0) 
	printf("Can be executed\n"); 
      else  
	printf("Can't execute : "); 
      break;
    case 'v' : if (result == -1) printf("%s\n",strerror(errno)); break;
    default : break;
    }
}

  return 0;

}
