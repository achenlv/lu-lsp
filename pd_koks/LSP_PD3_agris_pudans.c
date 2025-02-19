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

void search_files(const char *filename, const char *basedir, const char *curdir) {
  DIR *dir;
  struct dirent *entry;
  char path[MAX_PATH];
  struct stat statbuf;
  size_t baselen, curlen;

  /* Atveram direktoriju */
  dir = opendir(curdir);
  if (dir == NULL) {
    return;
  }

  /* Iegūstam ceļu garumus */
  baselen = strlen(basedir);
  curlen = strlen(curdir);

  /* Lasām direktorijas saturu */
  while ((entry = readdir(dir)) != NULL) {
    /* Izlaižam . un .. */
    if (strcmp(entry->d_name, ".") == 0 || 
        strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    /* Veidojam pilno ceļu */
    if (curlen + strlen(entry->d_name) + 2 > MAX_PATH) {
      continue;
    }
    strcpy(path, curdir);
    if (path[curlen - 1] != '/') {
      strcat(path, "/");
    }
    strcat(path, entry->d_name);

    /* Pārbaudām vai šis ir meklētais fails */
    if (strcmp(entry->d_name, filename) == 0) {
      /* Izvadām relatīvo ceļu */
      if (strcmp(curdir, basedir) == 0) {
        printf("./%s\n", entry->d_name);
      } else {
        printf("%s/%s\n", curdir + baselen + 1, entry->d_name);
      }
    }

    /* Ja šī ir direktorija, turpinām meklēšanu tajā */
    if (lstat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
      search_files(filename, basedir, path);
    }
  }

  closedir(dir);
}

void meklet(const char *filename, const char *basedir) {
  DIR *d;
  struct dirent *de;
  char path[MAX_PATH];
  struct stat statbuf;
  size_t len;

  d = opendir(basedir);
  while ( (de = readdir(d)) != NULL ) {
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
      continue;
    }
  }

  /* Izvada, ja fails ir meklētais? */
  if (strcmp(de->d_name, filename) == 0) {
    printf("./%s\n", de->d_name);
  }

  /* Rekursīvi iet tālāk apakšdirektorijās */
  if (lstat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
    meklet(filename, basedir);
  }

}

int main(int argc, char *argv[]) {
  struct stat statbuf;

  /* Pārbaudām argumentus */
  if (argc != 3) {
    fprintf(stderr, "Izmantošana: %s <fails> <direktorija>\n", argv[0]);
    return 0;
  }

  /* Pārbaudām vai otrais arguments ir direktorija */
  if (stat(argv[2], &statbuf) != 0 || !S_ISDIR(statbuf.st_mode)) {
    fprintf(stderr, "Kļūda: %s nav direktorija!\n", argv[2]);
    return -1;
  }

  /* Sākam meklēšanu */
  meklet(argv[1], argv[2]);

  return 0;
}