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
Sistēmas laiks: 0.064721 sekundes
Lietotāja laiks: 0.012147 sekundes
Kopējais laiks: 0.076868 sekundes
--------------------------------
Veiksmīgi rezervēts 1000 MB
Veiksmīgi rezervēts 2000 MB
Veiksmīgi rezervēts 2210 MB
Veiksmīgi rezervēts 2220 MB
Veiksmīgi rezervēts 2230 MB
Veiksmīgi rezervēts 2240 MB
Veiksmīgi rezervēts 2250 MB
Veiksmīgi rezervēts 2260 MB
Veiksmīgi rezervēts 2270 MB

2. eksperiments. Rezervēju atmiņu ar mmap()...
--------------------------------
100 MB rezervēšana ar mmap:
Sistēmas laiks: 0.102177 sekundes
Lietotāja laiks: 0.006796 sekundes
Kopējais laiks: 0.108973 sekundes
--------------------------------
Veiksmīgi rezervēts 1000 MB
Veiksmīgi rezervēts 2000 MB
Veiksmīgi rezervēts 2210 MB
Veiksmīgi rezervēts 2220 MB
Veiksmīgi rezervēts 2230 MB
Veiksmīgi rezervēts 2240 MB
Veiksmīgi rezervēts 2250 MB
Veiksmīgi rezervēts 2260 MB
Veiksmīgi rezervēts 2270 MB

3. eksperiments. Rezervēju atmiņu ar sbrk()...
--------------------------------
100 MB rezervēšana ar malloc:
Sistēmas laiks: 0.085103 sekundes
Lietotāja laiks: 0.010252 sekundes
Kopējais laiks: 0.095355 sekundes
--------------------------------
Veiksmīgi rezervēts 1000 MB
Veiksmīgi rezervēts 2000 MB
Veiksmīgi rezervēts 2210 MB
Veiksmīgi rezervēts 2220 MB
Veiksmīgi rezervēts 2230 MB
Veiksmīgi rezervēts 2240 MB
Veiksmīgi rezervēts 2250 MB
Veiksmīgi rezervēts 2260 MB
Veiksmīgi rezervēts 2270 MB

---

## Atruna

Programma ir pildīta uz asm1 servera. Es necentos noskaidrot, kādi ierobežojumi, kur un kāpēc ir uzstādīti šajā serverī.

## Secinājumi

1. Programma apstājas katras rezervācijas veida funkcijas izpildes laikā. Visdrīzāk ar OOM (Killed). Bet pārliecināties par to īsti nevaru. 
Katrs veids maksimāli atļauj rezervēt mazliet vairāk par 2270 MB. Tāpēc nevar pateikt, ka kāds no tiem atļāva rezervēt vairāk par citiem.

Visātrākais veids ir atmiņas rezervēšana ar malloc, ja skatās kopējo sistēmas un lietotāja laiku.
Ātrākais lietotāja laiks ir ar mmap.


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MB (1024 * 1024)

/* Funkcija atmiņas rezervēšanai ar malloc() */
void allocate_with_malloc() {
  size_t mb_count = 0;
  char **chunks;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;
  

  /* Pieņemam, ka nepārsniegsim 100 GB */
  chunks = (char **)malloc(100000 * sizeof(char *)); 
  
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;

  while (1) {
    chunks[mb_count] = (char *)malloc(MB);

    /* Pie rezervētiem 100 MB saskaitam laiku */
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
                (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
                (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");    
      printf("100 MB rezervēšana ar malloc:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }

    memset(chunks[mb_count], 1, MB);
    
    mb_count++;
    
    /* Ik pa laikam izvadām statusu */
    if (mb_count % 1000 == 0 || mb_count % 10 == 0 && mb_count > 2200) {
      printf("Veiksmīgi rezervēts %lu MB\n", (unsigned long)mb_count);
    }
  }
  

  /* Atbrīvojam atmiņu, lai varētu testēt nākamo metodi */
  while (mb_count > 0) {
    mb_count--;
    free(chunks[mb_count]);
  }
  free(chunks);
}

/* Funkcija atmiņas rezervēšanai ar mmap() */
void allocate_with_mmap() {
  size_t mb_count = 0;
  void **regions;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;

  /* Pieņemam, ka nepārsniegsim 100 000 MB */
  regions = (void **)malloc(100000 * sizeof(void *)); 

  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  
  while (1) {
    regions[mb_count] = mmap(NULL, MB, PROT_READ | PROT_WRITE, 
                            MAP_PRIVATE | MAP_ANON, -1, 0);

    /* Pie rezervētiem 100 MB saskaitam laiku */
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
                (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
                (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");    
      printf("100 MB rezervēšana ar mmap:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }


    memset(regions[mb_count], 1, MB);
    
    mb_count++;
    
    /* Ik pa laikam izvadām statusu */
    if (mb_count % 1000 == 0 || mb_count % 10 == 0 && mb_count > 2200) {
      printf("Veiksmīgi rezervēts %lu MB\n", (unsigned long)mb_count);
    }
  }
  
  printf("Ar mmap() maksimāli rezervēti: %lu MB\n", (unsigned long)mb_count);
  
  /* Atbrīvojam atmiņu, lai varētu testēt nākamo metodi */
  while (mb_count > 0) {
    mb_count--;
    munmap(regions[mb_count], MB);
  }
  free(regions);
}

/* Funkcija atmiņas rezervēšanai ar sbrk() */
void allocate_with_sbrk() {
  size_t mb_count = 0;
  void **chunks;
  void *prev_brk, *curr_brk;
  
  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;
  
  /* Pieņemam, ka nepārsniegsim 100 000 MB */
  chunks = (void **)malloc(100000 * sizeof(void *)); 
  if (!chunks) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;

  while (1) {
    prev_brk = sbrk(0); /* Iegūstam pašreizējo programm break */
    curr_brk = sbrk(MB);
    
    chunks[mb_count] = prev_brk;
    
    /* Pie rezervētiem 100 MB saskaitam laiku */
    if (mb_count == 100) {
      getrusage(RUSAGE_SELF, &usage_end);
      usr_end = usage_end.ru_utime;
      sys_end = usage_end.ru_stime;
      usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
                (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
      sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
                (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
      printf("--------------------------------\n");    
      printf("100 MB rezervēšana ar malloc:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }

    memset(chunks[mb_count], 1, MB);
    
    mb_count++;
    
    /* Ik pa laikam izvadām statusu */
    if (mb_count % 1000 == 0 || mb_count % 10 == 0 && mb_count > 2200) {
      printf("Veiksmīgi rezervēts %lu MB\n", (unsigned long)mb_count);
    }
  }
  
  printf("Ar sbrk() maksimāli rezervēti: %lu MB\n", (unsigned long)mb_count);
  
  /* Atbrīvojam atmiņu - ar sbrk() to var izdarīt tikai atgriežot brk atpakaļ */
  sbrk(-(MB * mb_count));
  free(chunks);
}

/* Galvenā funkcija */
int main() {
  printf("1. eksperiments. Rezervēju atmiņu ar malloc()...\n");
  allocate_with_malloc();

  printf("2. eksperiments. Rezervēju atmiņu ar mmap()...\n");
  allocate_with_mmap();

  printf("3. eksperiments. Rezervēju atmiņu ar sbrk()...\n");
  allocate_with_sbrk();
  
  return 0;
}