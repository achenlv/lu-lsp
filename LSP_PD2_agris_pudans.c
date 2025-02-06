#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 65536

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
    return -1;
  }

  FILE *source = fopen(argv[1], "rb");
  if (!source) {
    fprintf(stderr, "Error: Cannot open source file\n");
    return -1;
  }

  FILE *dest = NULL;
  if (access(argv[2], F_OK) == 0) {
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
    fprintf(stderr, "Error: Cannot create destination file\n");
    fclose(source);
    return -1;
  }

  unsigned char buffer[BUFFER_SIZE];
  size_t bytes_read;

  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
    if (fwrite(buffer, 1, bytes_read, dest) != bytes_read) {
      fprintf(stderr, "Error: Write failed\n");
      fclose(source);
      fclose(dest);
      return -1;
    }
  }

  if (ferror(source)) {
    fprintf(stderr, "Error: Read failed\n");
    fclose(source);
    fclose(dest);
    return -1;
  }

  fclose(source);
  fclose(dest);
  return 0;
}
