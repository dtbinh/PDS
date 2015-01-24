#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

void tail_simpliste(const char* file, int lignes) {
  int flux,nb_entree = 0;
  char octet;
  assert((flux = open(file,O_RDONLY)) != -1);
  while(read(flux, &octet, sizeof(char)) == 1) {
    if(octet == '\n') nb_entree++;
  }
  close(flux);
  assert((flux = open(file,O_RDONLY)) != -1);
  while(nb_entree > lignes && (read(flux, &octet, sizeof(char)) == 1)) {
    if(octet == '\n') nb_entree--; 
  }
  while(read(flux, &octet, sizeof(char)) == 1) write(STDIN_FILENO,&octet,sizeof(char));
}



void tail_utile(const char* file, const int lignes) {
  char tampon[LINE_MAX][LINE_MAX]; /* Nombre arbitraire */
  char octet;
  int flux,indice = 0,j = 0,k = 0;

  assert((flux = open(file,O_RDONLY)) != -1);

  /* Ecriture de chaque ligne dans chaque case du tableau tampon.
   Les lignes "inutiles" sont écrasées */
  while(read(flux, &octet, sizeof(char)) == 1) {
    tampon[indice][j++] = octet;
    if(octet == '\n') {
      tampon[indice][j]='\0'; 
      indice = ((indice + 1)%lignes); 
      j=0;
    }
  }

  /* Affichage des lignes, case par case de tampon */
  for(j = 0; j < lignes ; j++) {
    k = 0;
    octet = tampon[indice][k];
    while (octet != '\0' && octet != EOF) {
      octet = tampon[indice][k];
      write(STDIN_FILENO,&octet,sizeof(char));
      k++;
    }
    indice = (indice + 1)%lignes;
  }

  close(flux);
}

int main(int argc,char* argv[]) {
  tail_utile(argv[1],atoi(argv[2]));
  return 0;
}
