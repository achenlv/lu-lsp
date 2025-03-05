/*
 * Vārds Uzvārds
 * st. apl. nr. XX12345
 * Mājas darbs #4 - atmiņas rezervācija
 * Programma rezervē 100MB atmiņas un mēra izpildes laiku
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MB (1024 * 1024)
#define ALLOC_SIZE 100 /* 100 MB */

/* Funkcija atmiņas rezervēšanai ar malloc() */
void allocate_with_malloc() {
  size_t i;
  char **chunks;

  /* Vispirms alocējam masīvu norādēm */
  chunks = (char **)malloc(ALLOC_SIZE * sizeof(char *));
  if (!chunks) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  /* Alocējam atmiņu pa 1MB */
  for (i = 0; i < ALLOC_SIZE; i++) {
    chunks[i] = (char *)malloc(MB);
    if (!chunks[i]) {
      printf("Neizdevās alocēt atmiņu pēc %lu MB\n", (unsigned long)i);
      break;
    }
    
    /* Aizpildam atmiņu ar datiem, lai pārliecinātos, ka tā ir reāli pieejama */
    memset(chunks[i], 1, MB);
  }
  
  /* Atbrīvojam atmiņu */
  for (i = 0; i < ALLOC_SIZE; i++) {
    if (chunks[i]) {
      free(chunks[i]);
    }
  }
  free(chunks);
}

/* Funkcija atmiņas rezervēšanai ar mmap() */
void allocate_with_mmap() {
  size_t i;
  void **regions;
  
  /* Vispirms alocējam masīvu norādēm */
  regions = (void **)malloc(ALLOC_SIZE * sizeof(void *));
  if (!regions) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  /* Alocējam atmiņu pa 1MB */
  for (i = 0; i < ALLOC_SIZE; i++) {
    regions[i] = mmap(NULL, MB, PROT_READ | PROT_WRITE, 
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (regions[i] == MAP_FAILED) {
      printf("Neizdevās alocēt atmiņu pēc %lu MB\n", (unsigned long)i);
      break;
    }
    
    /* Aizpildam atmiņu ar datiem, lai pārliecinātos, ka tā ir reāli pieejama */
    memset(regions[i], 1, MB);
  }
  
  /* Atbrīvojam atmiņu */
  for (i = 0; i < ALLOC_SIZE; i++) {
    if (regions[i] != MAP_FAILED) {
      munmap(regions[i], MB);
    }
  }
  free(regions);
}

/* Funkcija atmiņas rezervēšanai ar sbrk() */
void allocate_with_sbrk() {
  size_t i;
  void **chunks;
  void *prev_brk, *start_brk;
  
  /* Vispirms alocējam masīvu norādēm */
  chunks = (void **)malloc(ALLOC_SIZE * sizeof(void *));
  if (!chunks) {
    printf("Neizdevās alocēt masīvu norādēm!\n");
    return;
  }
  
  /* Saglabājam sākotnējo programm break */
  start_brk = sbrk(0);
  
  /* Alocējam atmiņu pa 1MB */
  for (i = 0; i < ALLOC_SIZE; i++) {
    prev_brk = sbrk(0); /* Iegūstam pašreizējo programm break */
    if (sbrk(MB) == (void *)-1) {
      printf("Neizdevās alocēt atmiņu pēc %lu MB\n", (unsigned long)i);
      break;
    }
    
    chunks[i] = prev_brk;
    
    /* Aizpildam atmiņu ar datiem, lai pārliecinātos, ka tā ir reāli pieejama */
    memset(chunks[i], 1, MB);
  }
  
  /* Atbrīvojam atmiņu - ar sbrk() to var izdarīt tikai atgriežot brk atpakaļ */
  sbrk(start_brk - sbrk(0));
  free(chunks);
}

int main() {
  struct rusage usage_start, usage_end;
  struct timeval usr_start, usr_end, sys_start, sys_end;
  double usr_time, sys_time;
  
  printf("Atmiņas rezervācijas tests - 100MB\n");
  printf("==================================\n\n");
  
  /*******************************************************
  1. eksperiments - malloc()
  *******************************************************/
  
  printf("1. malloc() - 100MB rezervācija\n");
  
  /* Sākam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  
  allocate_with_malloc();
  
  /* Beidzam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  
  /* Aprēķinam patērēto laiku */
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Kopējais laiks: %.6f sekundes\n\n", usr_time + sys_time);
  
  /*******************************************************
  2. eksperiments - mmap()
  *******************************************************/
  
  printf("2. mmap() - 100MB rezervācija\n");
  
  /* Sākam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  
  allocate_with_mmap();
  
  /* Beidzam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  
  /* Aprēķinam patērēto laiku */
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Kopējais laiks: %.6f sekundes\n\n", usr_time + sys_time);
  
  /*******************************************************
  3. eksperiments - sbrk()
  *******************************************************/
  
  printf("3. sbrk() - 100MB rezervācija\n");
  
  /* Sākam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_start);
  usr_start = usage_start.ru_utime;
  sys_start = usage_start.ru_stime;
  
  allocate_with_sbrk();
  
  /* Beidzam mērīt laiku */
  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  
  /* Aprēķinam patērēto laiku */
  usr_time = (usr_end.tv_sec - usr_start.tv_sec) +
            (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time = (sys_end.tv_sec - sys_start.tv_sec) +
            (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Kopējais laiks: %.6f sekundes\n\n", usr_time + sys_time);
  
  return 0;
}