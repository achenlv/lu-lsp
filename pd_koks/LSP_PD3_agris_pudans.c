/*
Agris Pudāns
st. apl. nr. ap08426

PD3 -- Direktoriju koka apstaigāšana
*/

#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAX_PATH 256

void meklet(const char *filename, const char *basedir, const char *initdir) {
  DIR *d;
  struct dirent *de;
  char path[MAX_PATH];
  struct stat statbuf;

  /* Atveram direktoriju */
  d = opendir(basedir);
  if (d == NULL) {
    return;
  }

  /* Lasām direktorijas saturu */
  while ( (de = readdir(d)) != NULL ) {
    /* Izlaižam . un .. un softlinks, lai izvairītos no ieciklošanās */
    if (strcmp(de->d_name, ".") == 0 || 
        strcmp(de->d_name, "..") == 0 ||
        S_ISLNK(statbuf.st_mode)) {
      continue;
    }

    /* Veidojam pilno ceļu */
    if (strlen(basedir) + strlen(de->d_name) + 2 > MAX_PATH) {
      continue;
    }
    
    strcpy(path, basedir);
    if (path[strlen(basedir) - 1] != '/') {
      strcat(path, "/");
    }
    strcat(path, de->d_name);

    /* Izvada, ja fails ir meklētais */
    if (strcmp(de->d_name, filename) == 0) {
      const char *rel_path = path + strlen(initdir);
      if (*rel_path == '/') rel_path++;
      printf("%s\n", rel_path);
    }
      /* Izlaižam direktorijas, kas ir vājās saites */
      if (S_ISLNK(statbuf.st_mode)) {
        continue;
      }
    /* Rekursīvi iet tālāk apakšdirektorijās */
    if (lstat(path, &statbuf) == 0) {
      /* Izlaižam direktorijas, kas ir vājās saites */
      if (S_ISLNK(statbuf.st_mode)) {
        continue;
      }
      /* Rekursīvi ejam pa īstajām direktorijām */
      if (S_ISDIR(statbuf.st_mode)) {
        meklet(filename, path, initdir);
      }
    }
  }

  closedir(d);
}


int main(int argc, char *argv[]) {
  struct stat statbuf;

  /* Pārbaudām argumentus */
  if (argc != 3) {
    fprintf(stderr, "Izmantošana: %s <fails> <direktorija>\n", argv[0]);
    return 0;
  }
  if (stat(argv[2], &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "Kļūda: %s nav direktorija!\n", argv[2]);
    return -1;
  }

  /* Sākam meklēšanu */
   meklet(argv[1], argv[2], argv[2]); 

   
  return 0;
}