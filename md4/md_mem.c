/*
 * Agris Pudāns
 * st. apl. nr. ap08426
 * Mājas darbs #4 - atmiņas rezervācija
 *  Programma mēra maksimālo pieejamo atmiņas daudzumu
*/

/*
## Izpildes piemērs uz asm1 servera:
---
1. eksperiments. Rezervēju atmiņu ar malloc()...
--------------------------------
100 MB rezervēšana ar malloc:
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000612 sekundes
Kopējais laiks: 0.000612 sekundes
--------------------------------
Veiksmīgi rezervēts 3831 MB
2. eksperiments. Rezervēju atmiņu ar mmap()...
--------------------------------
100 MB rezervēšana ar mmap:
Sistēmas laiks: 0.000136 sekundes
Lietotāja laiks: 0.000034 sekundes
Kopējais laiks: 0.000170 sekundes
--------------------------------
Veiksmīgi rezervēts 3831 MB
3. eksperiments. Rezervēju atmiņu ar sbrk()...
--------------------------------
100 MB rezervēšana ar sbrk:
Sistēmas laiks: 0.000032 sekundes
Lietotāja laiks: 0.000000 sekundes
Kopējais laiks: 0.000032 sekundes
--------------------------------
Veiksmīgi rezervēts 37398901 MB
================================
Programmas izpildes laiks:
Sistēmas laiks: 8.788944 sekundes
Lietotāja laiks: 1.849708 sekundes
Kopējais laiks: 10.638652 sekundes
================================
---

## Atruna

Programma ir pildīta uz asm1 servera. Es necentos noskaidrot, kādi ierobežojumi, kur un kāpēc ir uzstādīti šajā serverī.

## Secinājumi


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MB (1024 * 1024)

/* Definējam MAP_ANON, ja tas nav definēts */
#ifndef MAP_ANON
#define MAP_ANON 0x20  /* Tipiska vērtība daudzām sistēmām */
#endif


/* Funkcija atmiņas rezervēšanai ar malloc() */
int allocate_with_malloc(void) {
  int mb_count = 0;
  size_t total_allocated = 0;
  void *ptr = NULL;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;
  
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;

  while (1) {
    /* Pie rezervētiem 100 MB saskaitam laiku */
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time =  (usr_end.tv_sec - usr_start.tv_sec) +
                  (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time =  (sys_end.tv_sec - sys_start.tv_sec) +
                  (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");    
      printf("100 MB rezervēšana ar malloc:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }
    ptr = malloc(total_allocated + MB);
    if (ptr == NULL) {
      break;
    }
    total_allocated += MB;
    mb_count++;

  }
  return mb_count;
}

/* Funkcija atmiņas rezervēšanai ar mmap() */
int allocate_with_mmap(void) {
  int mb_count = 0;
  size_t total_allocated = 0;
  void *ptr = NULL;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  
  while (1) {
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time =  (usr_end.tv_sec - usr_start.tv_sec) +
                  (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time =  (sys_end.tv_sec - sys_start.tv_sec) +
                  (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");    
      printf("100 MB rezervēšana ar mmap:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }

    /* Rezervē atmiņu mb_count MB. Pirmais solis 0 MB, otrais 1 MB, utt. */
    ptr = mmap(NULL, total_allocated + MB, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    /* Ja vairs nevar rezervēt atmiņu, tad ir atrasta robeža un izejam no while(1) cikla */
    if (ptr == MAP_FAILED) {
      break;
    }
    total_allocated += MB;
    mb_count++;
    /* Atbrīvojam atmiņu nākošajam solim*/
    munmap(ptr, total_allocated);
  }
  
  return mb_count;
}

/* Funkcija atmiņas rezervēšanai ar sbrk() */
int allocate_with_sbrk(void) {
  void *ptr = sbrk(0);
  int mb_count = 0;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;

  while (1) {
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time =  (usr_end.tv_sec - usr_start.tv_sec) +
                  (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time =  (sys_end.tv_sec - sys_start.tv_sec) +
                  (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");
      printf("100 MB rezervēšana ar sbrk:\n");
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");
    }

    if ( sbrk(MB) == (void *)-1 ) {
      break;
    }
    mb_count++;
  }

  sbrk(0);
  return mb_count;
}

/* Galvenā funkcija */
int main() {
  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;

  printf("1. eksperiments. Rezervēju atmiņu ar malloc()...\n");
  printf("Veiksmīgi rezervēts %lu MB\n", (long unsigned)allocate_with_malloc());

  printf("2. eksperiments. Rezervēju atmiņu ar mmap()...\n");
  printf("Veiksmīgi rezervēts %lu MB\n", (long unsigned)allocate_with_mmap());

  printf("3. eksperiments. Rezervēju atmiņu ar sbrk()...\n");
  printf("Veiksmīgi rezervēts %lu MB\n", (long unsigned)allocate_with_sbrk());

  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time =  (usr_end.tv_sec - usr_start.tv_sec) +
              (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time =  (sys_end.tv_sec - sys_start.tv_sec) +
              (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("================================\n");    
  printf("Programmas izpildes laiks:\n");            
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
  printf("================================\n");  

  return 0;
}