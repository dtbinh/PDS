#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "config.h"
#include "stat.h"
#include "tools.h"



void printtime() {
    time_t current_time;
    char* c_time_string;
 
    current_time = time(NULL);
 
    if (current_time == ((time_t)-1)) {
        (void) fprintf(stderr, "Failure to compute the current time.");
    }
 

    c_time_string = ctime(&current_time);
 
    if (c_time_string == NULL)
    {
        (void) fprintf(stderr, "Failure to convert the current time.");
    }
 
    (void) printf("STATS\t%s", c_time_string);
}

int spammeur() {
  int max = 0;
  int i;
  for(i = 1; i < MAX_CONNECTION; i++) if(lignes_envoyees[i] > lignes_envoyees[max]) max = i;
  return max;
}


int lecteur() {
  int max = 0;
  int i;
  for(i = 1; i < MAX_CONNECTION; i++) if(lignes_recues[i] > lignes_recues[max]) max = i;
  return max;
}


void printstats(int i) {
  printf("----\n");
  printtime();
  printf("Clients connectés\t%d\n",stats.clients);
  printf("Lignes envoyées  \t%d\n",stats.envoyes);
  printf("Lignes reçues    \t%d\n",stats.recus);
  printf("Record de connexions\t%d\n",stats.max_clients);
  printf("Plus gros spammeur\t%d\n",spammeur());
  printf("Le meilleur lecteur\t%d\n",lecteur());
  printf("----\n");
}

