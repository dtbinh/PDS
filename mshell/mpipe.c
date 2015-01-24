#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define BOLD "\033[00;01m"
#define NORM "\033[00;00m"
#define USAGE "./mpipe command1 "BOLD"to"NORM" command2\n"

int main(int argc,char* argv[]) {
  int i,indice_to = 0;
  int fds[2];
  pid_t pid;

  for(i = 0; i < argc; i++) {
    if (strcmp(argv[i],"to") == 0) {indice_to = i; i = argc;}
  }

  if(indice_to == 0) {
    printf(USAGE); 
    return -1;
  } else {
    argv[indice_to] = '\0';
  }

  assert(pipe(fds) != -1);

  pid = fork();
  switch(pid) {
  case -1 : 
    printf("Fork please"); 
    assert(0);
  case 0 :
    setpgid(0,0);
    dup2(fds[0],STDIN_FILENO);
    close(fds[0]);
    close(fds[1]);
    execvp(argv[indice_to+1],argv+indice_to + 1);
  }

  switch(fork()) {
  case -1 :
    printf("Fork please"); 
    assert(0);
  case 0 :
    setpgid(0,pid);
    dup2(fds[1],STDOUT_FILENO);
    close(fds[0]);
    close(fds[1]);  
    execvp(argv[1],argv+1);
  }

  return 0;




}
