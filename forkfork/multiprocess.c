#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) { 
  char commande[32];
  int i;
  for (i = 0; i < 10 ; i++) {
    switch(fork()) {
    case -1 : printf("Please why?");
    case 0 :
      while(1) { 
	write(0,commande,snprintf(commande,31,"Le fils %d est toujours là\n",getpid()));
	sleep(15);
      }    
    default: break;
    }
    
  }
  snprintf(commande,31,"ps -a -f");
  system(commande);
   for (i = 0; i < 10 ; i++) { 
    scanf("%s",commande);
    printf("%s",commande); 
    system(commande);
    }
  return 0;
}
