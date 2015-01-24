/*
   mtcs - a multithreaded chat serveur
   Philippe Marquet, Apr 2005

   Gestion de la connexion d'un client par un thread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

#include "config.h"
#include "tools.h"
#include "stat.h"

/* Gestion des sockets */
static int sockets[MAX_CONNECTION]; /* tableau initialisé a zero */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void
add_socket(int fd)
{
  int i;
  
  pgrs_in();
  for (i=0; i<MAX_CONNECTION; i++) {
    if (sockets[i] == 0) {
      sockets[i] = fd;
      break;
    }
  }
  assert(i!=MAX_CONNECTION);
  assert(pthread_mutex_lock(&mutex) == 0);
  if(stats.clients == stats.max_clients) {stats.max_clients++;}
  stats.clients++;
  assert(pthread_mutex_unlock(&mutex) == 0);
  pgrs_out();
}

static void
del_socket(int fd)
{
    int i;

    pgrs_in();
    for (i=0; i<MAX_CONNECTION; i++) {
        if (sockets[i] == fd) {
            sockets[i] = 0;
	    lignes_envoyees[i] = 0;
	    lignes_recues[i] = 0;
            break;
        }
    }
    assert(i!=MAX_CONNECTION);
    assert(pthread_mutex_lock(&mutex) == 0);
    stats.clients--;
    assert(pthread_mutex_unlock(&mutex) == 0);
    pgrs_out();
}

/* Un client */
static void
repeater(int sckt)
{
    char buf[MAX_BUFFER];
    int nbc, i;
    const char WELCOME[] = "mtcs : bienvenu\n";

    pgrs_in();
    write(sckt, WELCOME, strlen(WELCOME));

    pgrs("enregistrement d'une socket");
    add_socket(sckt);

    while (1) {
      pgrs("attente read");
      nbc = read(sckt, buf, MAX_BUFFER);
      if (nbc <= 0) {
	pgrs("fin lecture client");
	pgrs("desenregistrement d'une socket");
	del_socket(sckt);
	close(sckt);
	pgrs_out();
	return;
      }
      lignes_envoyees[sckt]++;
      stats.envoyes++;
      pgrs("boucle ecriture");
      assert(pthread_mutex_lock(&mutex) == 0);
      for(i=0; i<MAX_CONNECTION; i++)
	if (sockets[i] != 0 && sockets[i] != sckt){
	  write(sockets[i], buf, nbc);
	  lignes_recues[i]++;
	  stats.recus++;}
      assert(pthread_mutex_unlock(&mutex) == 0);
      pgrs("fin boucle ecriture");
    }
}

void* annexe(void* tmp) {
  repeater((int) tmp);
  return NULL;
}


/* Création d'un client */
int
manage_cnct(int fd)
{
    pthread_t thread;

    pgrs_in();
    assert(pthread_create(&thread,NULL,&annexe,(void *) fd) == 0);
    assert(pthread_detach(thread) == 0);
    pgrs_out();
    return 0;
}
