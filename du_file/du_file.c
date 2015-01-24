#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <search.h>

#define ERROR "du_file: illegal option\nusage: du_file [-bL] [pathname]\n"

#define MAX_KEY 1048576


static int opt_follow_links = 0;
static int opt_apparent_size = 0;

int valid_name(const char* name) {
  return (strcmp(name,".") && strcmp(name,".."));
}


/*
int du_file(const char* pathname) {
  DIR* d;
  struct stat st;
  struct dirent* dt;
  char chemin[PATH_MAX+1];
  int taille = 0;
  if(opt_follow_links) {
    assert(stat(pathname,&st) != -1);}
  else 
    {assert(lstat(pathname,&st) != -1);}
  if(S_ISDIR(st.st_mode)) {
    assert((d = opendir(pathname)) != NULL);
    while((dt = readdir(d)) != NULL) {
      if(valid_name(dt->d_name)) {
	snprintf(chemin,PATH_MAX,"%s/%s",pathname,dt->d_name);
	printf("%s\n",chemin);
	taille += du_file(chemin);
      }
    }
    closedir(d);
  }
  taille +=opt_apparent_size?st.st_size:st.st_blocks;
  return taille;
} */

int 
du_liens_physiques
(const char* pathname)
{
  DIR* d;
  struct stat st;
  struct dirent* dt;
  char chemin[PATH_MAX+1];
  int taille;
  char key[MAX_KEY];
  ENTRY couple;
  taille = 0;
  

  if(opt_follow_links) {
    assert(stat(pathname,&st) != -1);}
  else 
    {assert(lstat(pathname,&st) != -1);}

  if(S_ISDIR(st.st_mode)) {
    assert((d = opendir(pathname)) != NULL);

    while((dt = readdir(d)) != NULL) {
      if(valid_name(dt->d_name)) {
	snprintf(chemin,PATH_MAX,"%s/%s",pathname,dt->d_name);
	printf("%s\n",chemin);
	taille += du_liens_physiques(chemin);
      }
    } /* end while */

    closedir(d);
  }/* endif S_ISDIR */


  snprintf(key, MAX_KEY, "%d%d",(int) (st.st_ino),(int) (st.st_dev));
  couple.key = key;
  couple.data = &pathname;
 
  if (!hsearch(couple,FIND)) {
    hsearch(couple, ENTER);
    taille +=opt_apparent_size?st.st_size:st.st_blocks;
  }
  return taille;

}

int main(int argc, char* argv[]) {
  int opt;
  assert(hcreate(256)!=0);
  while((opt = getopt(argc, argv, "bL")) != -1){
    switch(opt) {
    case 'b' :opt_apparent_size = 1; break;
    case 'L' :opt_follow_links = 1; break;
    default: printf(ERROR); return 255;}
  }
  printf("%d\n",du_liens_physiques(argv[argc - 1]));
  /*printf("%d\n",du_file(argv[argc - 1]));*/
  hdestroy();
  return 0;
}
