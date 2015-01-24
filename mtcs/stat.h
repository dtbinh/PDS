struct stats_t { 
  int clients;
  int recus;
  int envoyes;
  int max_clients;
};

struct stats_t stats;

int lignes_envoyees[MAX_CONNECTION]; 

int lignes_recues[MAX_CONNECTION]; 

extern void printstats(int i);

