/*
pārveidot visu


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Izmantošana: %s <ievads> <izeja>\n", argv[0]);
    return -1;
  }

  FILE *source = fopen(argv[1], "rb");
  if (!source) {
    fprintf(stderr, "Kļūda: Nav ievada faila\n");
    return -1;
  }

  FILE *dest = fopen(argv[2], "rb");
  if (dest) {
    fclose(dest);
    char response;
    printf("Izvades fails jau eksistē. Vai tiešām pārrakstīt? (j/n) ");
    response = getchar();
    if (response != 'j') {
      fclose(source);
      return -1;
    }
  }

  dest = fopen(argv[2], "wb");
  if (!dest) {
    fprintf(stderr, "Kļūda: Nav izejas faila\n");
    fclose(source);
    return -1;
  }

  char buffer[BUFFER_SIZE];
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
    fwrite(buffer, 1, bytesRead, dest);
  }

  fclose(source);
  fclose(dest);

  return 0;
}