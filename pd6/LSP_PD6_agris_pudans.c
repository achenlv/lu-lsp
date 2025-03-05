/*
 * Agris Pudāns
 * st. apl. nr. ap08426
 * LSP Praktiskais darbs #6 - atmiņas rezervācija
*/

/*
## Izpildes piemērs uz asm1 servera:
---
1.1. Rezervēšana vienā gabalā bez izmantošanas
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000037 sekundes
Kopējais laiks: 0.000037 sekundes
1.2. Rezervēšana vienā gabalā sarakstot ar nullēm
Sistēmas laiks: 0.100965 sekundes
Lietotāja laiks: 0.005452 sekundes
Kopējais laiks: 0.106417 sekundes
Rezervēti 100 MB
2.1. Rezervēšana pa MB bez izmantošanas
Sistēmas laiks: 0.000625 sekundes
Lietotāja laiks: 0.000050 sekundes
Kopējais laiks: 0.000675 sekundes
Rezervēti 100 MB
2.2. Rezervēšana pa MB gabalā sarakstot ar nullēm
Sistēmas laiks: 0.095249 sekundes
Lietotāja laiks: 0.011960 sekundes
Kopējais laiks: 0.107209 sekundes
3.2. Tests ar statisko atmiņu izmantojot alloca()
Sistēmas laiks: 0.096074 sekundes
Lietotāja laiks: 0.004008 sekundes
Kopējais laiks: 0.100082 sekundes
4. Cik daudz atmiņas var rezervēt stekā ar alloca()
Rezervēts 1 MB 
Rezervēts 2 MB 
Rezervēts 3 MB 
Rezervēts 4 MB 
^C
---

## Secinājumi
1. Rezervācija uzreiz ir daudz ātrāka. Rezervācija pa gabaliem ir lēnāka, jo ir jāveic vairākas operācijas.
2. Atmiņas aizpildīšana ar nullēm ir lēnāka, jo ir jāveic papildus operācijas. Tomēr šādā veidā mums ir pārliecība, ka atmiņas saturs pēķšņi nesatur kādu citu informāciju.
3. f() sasniedz atļautās atmiņas rezervācijas limitu stekā un izraisa segfault, jo steks ir ierobežots, bet f_static() izdodas, jo statiskā atmiņa tiek rezervēta heap segmentā.
4. Izskatas, ka atļautā steka atmiņas rezervācijas limits ir lielāks par 4 MB un noteikti mazāks par 5 MB, jo pēc tam programma izbeidzas ar segfault.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MB (1024 * 1024)

/* Funkcija, kas rezervē visu atmiņu uzreiz vienā gabalā */
void allocate_one_chunk(size_t size_mb, int zeros) {
  size_t size = size_mb * MB;
  char *memory;

  memory = (char *)malloc(size);
  if (memory == NULL) {
    printf("Neizdevās rezervēt %lu MB\n", (unsigned long)(size / MB));
  } else {
    if (zeros) {
      /* Aizpildam atmiņu ar nullēm, ja prasīts */
      memset(memory, 0, size);
    }
    /* 
    free(memory); 
    */ 
  }
}

/* Funkcija, kas rezervē atmiņu pa 1MB gabaliem */
void allocate_many_chunks(size_t size_mb, int zeros) {
  size_t i;
  char **memory_chunks;

  memory_chunks = (char **)malloc(size_mb * sizeof(char *));

  for (i = 0; i < size_mb; i++) {
    memory_chunks[i] = (char *)malloc(MB);  
    if (zeros) {
      /* Aizpildam atmiņu ar nullēm, ja prasīts */
      memset(memory_chunks[i], 0, MB);
    }
    if ((i + 1) % 100 == 0 || i == size_mb - 1) {
      printf("Rezervēti %lu MB\n", (unsigned long)(i + 1));
    }
  }
  
  /*
  for (i = 0; i < size_mb; i++) {
    free(memory_chunks[i]);
  }
  free(memory_chunks);
  */
}

void f() {
  char array[100 * MB];
  memset(array, 0, sizeof(array));
}

void f_static() {
  static char array[100 * 1024 * 1024];
  memset(array, 0, sizeof(array));
}

void test_stack() {
  size_t size_mb = 1;
  
  while (1) {
    char *memory;
    size_t size = size_mb * MB;
    
    printf("Rezervēts %lu MB \n", (unsigned long)size_mb);
    memory = (char *)alloca(size);
    // memset(memory, 0, size);
    size_mb++;
  }
}

int main(void) {
  size_t size_mb = 100; 
  int zeros = 1; /* 1 - aizpildīt ar nullēm, 0 - neaizpildīt */
  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;

  /*******************************************************
  1.1. eksperiments
  Rezervēšana vienā gabalā bez izmantošanas
  *******************************************************/

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  allocate_one_chunk(size_mb, 0);
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("1.1. Rezervēšana vienā gabalā bez izmantošanas\n");            
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);

  /*******************************************************
  1.2. eksperiments
  Rezervēšana vienā gabalā sarakstot ar nullēm
  *******************************************************/

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  allocate_one_chunk(size_mb, 1);
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("1.2. Rezervēšana vienā gabalā sarakstot ar nullēm\n");
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);



  /*******************************************************
  2.1. eksperiments
  Rezervēšana pa MB bez izmantošanas
  *******************************************************/

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  allocate_many_chunks(size_mb, 0);
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("2.1. Rezervēšana pa MB bez izmantošanas\n"); 
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);

  /*******************************************************
  2.2. eksperiments
  Rezervēšana pa MB sarakstot ar nullēm
  *******************************************************/

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  allocate_many_chunks(size_mb, 1);
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("2.2. Rezervēšana pa MB gabalā sarakstot ar nullēm\n");
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);

  /*******************************************************
  3.1. eksperiments
  Eksperiments ar steka atmiņu izmantojot alloca()
  *******************************************************/
  /*
  f();
  */


  /*******************************************************
  3.2. eksperiments
  Eksperiments ar statisko atmiņu izmantojot alloca()
  *******************************************************/
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  f_static();
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("3.2. Tests ar statisko atmiņu izmantojot alloca()\n");
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);

  /*******************************************************
  4. eksperiments
  Cik daudz atmiņas var rezervēt stekā ar alloca()
  *******************************************************/
  printf("4. Cik daudz atmiņas var rezervēt stekā ar alloca()\n"); 
  test_stack();
  

  return 0;
}