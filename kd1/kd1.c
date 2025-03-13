/*
*
* Agris Pudāns
* st. apl. nr. ap08426
* 12.03.2025.
* KD1
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CYPHER_TABLE_SIZE 256

/* Noklusējuma šifrēšana - XOR 0x80 */
void processDataDefault(FILE *input, FILE *output) {
  int c;
  while ((c = fgetc(input)) != EOF) {
    fputc(c ^ 0x80, output);
    /* fputc(c, output);*/
  }
}

/* Ar šifrēšanas failu */
void processDataCypher(FILE *input, FILE *output) {
  int c;
  while ((c = fgetc(input)) != EOF) {
    fputc(c, output);
  }
}

/* Ar translācijas failu */
void processDataTransltr(FILE *input, FILE *output) {
  int c;
  while ((c = fgetc(input)) != EOF) {
    fputc(c, output);
  }
}


int main(int argc, char *argv[]) {
  int i;
  FILE *input = stdin;
  FILE *output = stdout;
  char *transltr_file = NULL;
  char *cypher_file = NULL;
  char *output_file = NULL;
  char *input_file = NULL;


  /* Apstrādā argv[] */
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-t") == 0 && i + 1 < argc ) {
      transltr_file = argv[++i];
      printf("Transliterator file: %s\n", transltr_file);
    }
    if (strcmp(argv[i], "-s") ==0 && i + 1 < argc) {
      cypher_file = argv[++i];
      printf("Cypher file: %s\n", cypher_file);
    }
    if (strcmp(argv[i], "-o") ==0 && i + 1 < argc) {
      output_file = argv[++i];
      printf("Output file: %s\n", output_file);
    }
    if (argv[i][0] == '-') {
      input_file = argv[i];
    } 
  }

  /* -t un -s vienlaicīgi nelieto */
  if (transltr_file && cypher_file) {
    fprintf(stderr, "[ ERROR ] -t and -s cannot be used together\n");
    return 1;
  }

  /* ja nav input_file, tad lasa no stdin */
  if (input_file != NULL) {
    input = fopen(input_file, "r");
    if (input == NULL) {
      fprintf(stderr, "[ ERROR ] cannot open input file %s\n", input_file);
      return 1;
    }
  }
  
  /* ja nav output_file, tad raksta uz stdout */
  if (output_file != NULL) {
    output = fopen(output_file, "w");
    if (output == NULL) {
      fprintf(stderr, "[ ERROR ] cannot open output file %s\n", output_file);
      return 1;
    }
  }

  /* ja nav ne transltr_file, ne cypher_file, tad izmanto noklusējumu XOR 0x80 */
  /* TODO: šķiet, ka loģika mazliet nav ok  ar pēdējo izvēli - pārbaudī nosacījumus */
  if (transltr_file != NULL ) {
    /* Lietot translācijas failu */
    fprintf(stdout,"Lieto trnaslācijas failu\n");
    processDataTransltr(input, output);
  } else if (cypher_file != NULL) {
    /* Lietot šifrēšanas failu */
    fprintf(stdout,"Lieto šifrēšanas failu\n");
    processDataCypher(input, output);
  } else {
    /* Lietot XOR 0x80 */
    fprintf(stdout,"Lieto ar katru ievada-simbolu izpilda operāciju XOR 0x80, tātad, nomainot katra baita kreiso bitu uz pretējo\n");
    processDataDefault(input, output);
  }

  /* Aizvērt failus */
  if (input != stdin) {
    fclose(input);
  }
  if (output != stdout) {
    fclose(output);
  }

  return 0;
}