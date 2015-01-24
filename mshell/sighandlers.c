/* ------------------------------
   $Id: sighandlers.c,v 1.1 2005/03/17 13:00:46 marquet Exp $
   ------------------------------------------------------------

   mshell - a job manager
   
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <assert.h>


#include "jobs.h"
#include "common.h"
#include "sighandlers.h"


/*
 * Signal - wrapper for the sigaction function
 */
int
signal_wrapper(int signum, handler_t *handler) 
{
  struct sigaction action;
  action.sa_flags = SA_RESTART;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  assert(sigaction(signum, &action, NULL) != -1);
  return 0;  
}


/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children
 */
void
sigchld_handler(int sig) 
{

  int status;
  pid_t pid;
  
  if (verbose)
    printf("sigchld_handler: entering\n");

  while((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
    struct job_t* job = jobs_getjobpid(pid);
    if (WIFEXITED(status)) {
      if (job != NULL) printf("[%d] Terminated \t %s\n",job->jb_jid,job->jb_cmdline);
      jobs_deletejob(pid);
    }
    else if (WIFSIGNALED(status)) {
      if (job != NULL) printf("[%d] Terminated (signal %d) \t %s\n",job->jb_jid,WTERMSIG(status),job->jb_cmdline);
      jobs_deletejob(pid);      
    } 
    else if (WIFSTOPPED(status)) {
      if (job != NULL) printf("[%d] Suspended (signal %d) \t %s\n",job->jb_jid,WSTOPSIG(status),job->jb_cmdline);
	job->jb_state = ST;
    } 
  } 
  
  if (verbose)
    printf("sigchld_handler: exiting\n");
  
  return;
}

/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void
sigint_handler(int sig) 
{

  pid_t processus;
   
  if (verbose)
    printf("sigint_handler: entering\n");
  
  processus = jobs_fgpid();
  if (processus != 0){    
    kill(processus,sig);  
  }
    
  if (verbose)
    printf("sigint_handler: exiting\n");
    
  return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void
sigtstp_handler(int sig) 
{
  
  pid_t processus;
  
  if (verbose)
    printf("sigtstp_handler: entering\n");
  
  processus = jobs_fgpid();
  if (processus != 0) {
    kill(processus,sig);
  }

  if (verbose)
    printf("sigtstp_handler: exiting\n");
    
  return;
}
