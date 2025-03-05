/*
 * Agris Pudāns
 * st. apl. nr. ap08426
 * Mājas darbs #4 - atmiņas rezervācija
 *  Programma mēra maksimālo pieejamo atmiņas daudzumu
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

  printf("Rezervēju atmiņu ar malloc()...\n");
  

  /* Pieņemam, ka nepārsniegsim 100 GB */
  chunks = (char **)malloc(100000 * sizeof(char *)); 
  
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;

  while (1) {
    chunks[mb_count] = (char *)malloc(MB);
    if (!chunks[mb_count]) {
      printf("Sasniegts malloc() limits pēc %lu MB\n", (unsigned long)mb_count);
      break;
    }

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
      printf("1. 100 MB rezervēšana ar malloc:\n");            
      printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
      printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
      printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
      printf("--------------------------------\n");    
    }


    memset(chunks[mb_count], 1, MB);
    
    mb_count++;
    
    /* Izvadām statusu ik pēc 1000 MB vai ja sasniedzam apaļu skaitli */
    if (mb_count % 100 == 0) {

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
  
  printf("\nRezervēju atmiņu ar mmap()...\n");
  
  /* Vispirms alocējam masīvu norādēm */
  regions = (void **)malloc(100000 * sizeof(void *)); /* Pieņemam, ka nepārsniegsim 100 000 MB */
  if (!regions) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  /* Mēģinām alocēt pēc iespējas vairāk atmiņas */
  while (1) {
    regions[mb_count] = mmap(NULL, MB, PROT_READ | PROT_WRITE, 
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (regions[mb_count] == MAP_FAILED) {
      printf("Sasniegts mmap() limits pēc %lu MB\n", (unsigned long)mb_count);
      break;
    }
    
    /* Pārbaudām, vai atmiņa ir izmantojama, ierakstot un lasot datus */
    memset(regions[mb_count], 1, MB);
    
    mb_count++;
    
    /* Izvadām statusu ik pēc 1000 MB vai ja sasniedzam apaļu skaitli */
    if (mb_count % 1000 == 0 || mb_count == 1 || mb_count == 10 || mb_count == 100) {
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
  
  printf("\nRezervēju atmiņu ar sbrk()...\n");
  
  /* Vispirms alocējam masīvu norādēm */
  chunks = (void **)malloc(100000 * sizeof(void *)); /* Pieņemam, ka nepārsniegsim 100 000 MB */
  if (!chunks) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  /* Mēģinām alocēt pēc iespējas vairāk atmiņas */
  while (1) {
    prev_brk = sbrk(0); /* Iegūstam pašreizējo programm break */
    curr_brk = sbrk(MB);
    
    if (curr_brk == (void *)-1) {
      printf("Sasniegts sbrk() limits pēc %lu MB\n", (unsigned long)mb_count);
      break;
    }
    
    chunks[mb_count] = prev_brk;
    
    /* Pārbaudām, vai atmiņa ir izmantojama, ierakstot un lasot datus */
    memset(chunks[mb_count], 1, MB);
    
    mb_count++;
    
    /* Izvadām statusu ik pēc 1000 MB vai ja sasniedzam apaļu skaitli */
    if (mb_count % 1000 == 0 || mb_count == 1 || mb_count == 10 || mb_count == 100) {
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
  printf("Atmiņas rezervācijas tests - maksimālais daudzums\n");
  printf("=================================================\n");
  
  allocate_with_malloc();
  // allocate_with_mmap();
  // allocate_with_sbrk();
  
  return 0;
}