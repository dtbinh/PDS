/* ------------------------------
   $Id: pipe.c,v 1.2 2005/03/29 09:46:52 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "pipe.h"


void do_pipe(char *cmds[MAXCMDS][MAXARGS], int nbcmd, int bg) {
  int tube[MAXCMDS][2];
  int status,i=0,j=0;
  pid_t pid; 

  for(i = 0 ; i < nbcmd-1; i++){
    assert(pipe(tube[i]) != -1);
    switch(pid = fork()) {
    case -1 : printf("Mais oui c'est clair\n"); assert(0);
    case 0 :
      if (verbose) {
	printf("[pipe-%d] %s",i,cmds[i][0]); 
	j = 1; 
	while(cmds[i][j] != NULL) {
	  printf(" %s ",cmds[i][j]); j++;
	}
	printf("\n");
	fflush(stdout);
      } 
      dup2(tube[i][1],STDOUT_FILENO);
      close(tube[i][1]);
      close(tube[i][0]);
      execvp(cmds[i][0],cmds[i]+1);
      assert(0);
    default :
      waitpid(pid,&status,0);
    }
  }



  switch(pid = fork()) {
  case -1 : printf("Well met\n"); assert(0);
  case 0 : 
    assert(pipe(tube[i]) != -1);
    if (verbose) {
      printf("[pipe-%d] %s",i,cmds[i][0]); 
      j = 1; 
      while(cmds[i][j] != NULL) {
	printf(" %s ",cmds[i][j]); j++;
      }
      printf("\n");
      fflush(stdout);
    } 
    dup2(tube[i][0],STDIN_FILENO);
    close(tube[i][0]);
    close(tube[i][1]);
    execvp(cmds[i][0],cmds[i]+1);
    assert(0);
  default:
    waitpid(pid,&status,0);
  }

}
