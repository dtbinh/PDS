#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "makeargv.h"

#define USAGE "do: illegal option\nusage: do ["AND"|"OR"] ["CC"] ["KILL"] [cmd]\n"

#define AND "--and" 
#define OR "--or"
#define CC "--cc"
#define KILL "--kill"

/* Dans le tableau de processus, remplace le pid du processus qui vient de finir par une valeur neutre, -1. */
void neutralize(pid_t tableau[], int size, pid_t pid){
  int i = 0;
  while (i < size) {
    if (tableau[i] == pid) {tableau[i] = -1; i = size;}
    else i++;
  }
}

/* Tue tous les processus qui n'ont pas encore finis, càd ceux dont le pid est contenu dans le tableau de processus. */
void killall(pid_t tableau[],int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (tableau[i] != -1) {
      printf("Kill %d\n",tableau[i]); 
      assert(kill(tableau[i],SIGKILL) == 0);
    }
  }  
}


int main(int argc, char* argv[]) {
  pid_t tableau[64];
  int i,cc=0,status=0,cpt;
  pid_t pid;
  char** argvp;

  if ((strcmp(argv[1],AND) != 0) && (strcmp(argv[1],OR) != 0)) {
    write(STDOUT_FILENO, USAGE, strlen(USAGE)); 
    return -1;
  }
  
  /* L'indice de départ des commandes dépend de la présence ou non de "--cc" et de "--kill".
     La variable cc va donc valoir 0, 1 en cas de présence de -cc, 2 en cas de --cc puis --kill. */
  if (strcmp(argv[2],CC) == 0) {
    if (strcmp(argv[3],KILL) == 0) {
	cc = 2;
    } else {
      cc = 1;
    }
  }

  /* Crée n processus, un pour chaque commande, puis ajoute les pid dans un tableau */
  for (i = 2+cc ; i < argc ; i++) {
    switch(pid = fork()) {
    case -1 : printf("Erreur dans le fork.");
    case 0 :
      makeargv(argv[i]," ",&argvp);
      printf("Le fils %d fait %s.\n",getpid(),*argvp);
      
      /* En cas de succès de execvp, EXIT_SUCCESS ou EXIT_FAILURE en focntion du programmee. */
      execvp(*argvp,argvp);

      /* Cette ligne est atteinte si execvp échoue, et renvoie EXIT_FAILURE. 
	 execvp peut réussir mais le programme lu peut échouer : 
	 - execvp("cp","cp -BONJOUR- -WELLMET-")
	 - execvp("ps","ps oops") 
	 - execvp("ls","ls -Goobypls") */
      exit(EXIT_FAILURE);
    
    default : tableau[i-(2+cc)] = pid; break; /*Ajoute le pid du fils dans le tableau de processus. */
    }
  }

  /* Imprime la liste des processus */
  for (i = 0 ; i < (argc - (2+cc)) ; i++) {
    if (i == 0) printf("Liste des processus fils : [");
    printf(" %d ",tableau[i]);
    if (i == (argc - (3+cc))) printf("]\n");
  }


  for (i = 2+cc ; i < argc ; i++) {
    pid = wait(&status);
    assert(pid != -1);
    /* Initialisation du calcul à partir de l'état du premier processus fini (succès = 0, échec = non nul) */
    if (i == 2+cc) cpt = WEXITSTATUS(status); 

    /* Quelle que soit l'issue (succès, échec), le processus est terminé. */
    neutralize(tableau,(argc-2-cc),pid); 

	printf("Le fils %d a fini, status : %d.\n",pid,WEXITSTATUS(status));

    if (strcmp(argv[1],AND) == 0) {
      cpt = cpt && WEXITSTATUS(status);
      /* Arret de tous les processus si un seul a échoué dans le cas du ET */
      if (cc == 2 && (WEXITSTATUS(status) != EXIT_SUCCESS)) {
	printf("Le fils %d s'est planté. Conjonction coupe-circuit.\n",pid);
	killall(tableau,argc-2-cc); 
	i=argc;
      }
    } 


    if (strcmp(argv[1],OR) == 0) {
      cpt = cpt || WEXITSTATUS(status);
      /* Arret de tous les processus si un seul a réussi dans le cas du OU */
      if (cc == 2 && (WEXITSTATUS(status) == EXIT_SUCCESS)) {
	printf("Le fils %d a réussi. Disjonction coupe-circuit.\n",pid);
	killall(tableau,argc-2-cc); 
	i=argc;
      }
    } 

  }

  /* Imprime le résultat de la conjonction/disjonction. */
  if (cc != 2) {
    if (strcmp(argv[1],AND) == 0)  { 
      printf("ET : %d\n",cpt);
    } else { 
      printf("OU : %d\n",cpt);
    } 
  }
 
 return 0;
}
