/*
 * Agris Pudāns
 * st. apl. nr. ap08426
 * LSP Praktiskais darbs #6 - atmiņas rezervācija
*/

/*
## Izpildes piemērs uz asm1 servera:
---
[ INFO ] Rezervē 64 baitus adresē 0x5587c2a95040
Atbrīvota atmiņa adresē 0x5587c2a95040
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000503 sekundes
Kopējais laiks: 0.000503 sekundes
--------------------------------

[ INFO ] Rezervē 128 baitus adresē 0x5587c2a95080
Atbrīvota atmiņa adresē 0x5587c2a95080
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000543 sekundes
Kopējais laiks: 0.000543 sekundes
--------------------------------

[ INFO ] Rezervē 512 baitus adresē 0x5587c2a95100
Atbrīvota atmiņa adresē 0x5587c2a95100
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000580 sekundes
Kopējais laiks: 0.000580 sekundes
--------------------------------

[ INFO ] Rezervē 1024 baitus adresē 0x5587c2a95300
Atbrīvota atmiņa adresē 0x5587c2a95300
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000620 sekundes
Kopējais laiks: 0.000620 sekundes
--------------------------------

[ INFO ] Rezervē 2048 baitus adresē 0x5587c2a95700
Atbrīvota atmiņa adresē 0x5587c2a95700
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000666 sekundes
Kopējais laiks: 0.000666 sekundes
--------------------------------

[ INFO ] Rezervē 3072 baitus adresē 0x5587c2a95f00
Atbrīvota atmiņa adresē 0x5587c2a95f00
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000713 sekundes
Kopējais laiks: 0.000713 sekundes
--------------------------------

[ INFO ] Rezervē 4000 baitus adresē 0x5587c2a95b00
Atbrīvota atmiņa adresē 0x5587c2a95b00
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000766 sekundes
Kopējais laiks: 0.000766 sekundes
--------------------------------

[ ERROR ] Kļūda rezervējot 4096 baitus
Kļūda atbrīvojot datus adresē (nil)
Sistēmas laiks: 0.000000 sekundes
Lietotāja laiks: 0.000807 sekundes
Kopējais laiks: 0.000807 sekundes
---
*/

#include <stdio.h>
#include <stddef.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MY_BUFFER_SIZE 4096

unsigned char mybuffer[MY_BUFFER_SIZE];
size_t next_fit_index = 0;

void *myalloc(size_t size) {
  size_t start_index;
  size_t free_space;
  size_t alloc_start;
  size_t i;

  if (size == 0 || size > MY_BUFFER_SIZE) {
    return NULL;
  }

  start_index = next_fit_index;
  free_space = 0;

  while (free_space < size) {
    if (mybuffer[next_fit_index] == 0) {
      free_space++;
    } else {
      free_space = 0;
    }

    next_fit_index = (next_fit_index + 1) % MY_BUFFER_SIZE;

    if (next_fit_index == start_index) {
      return NULL;
    }
  }

  alloc_start = (next_fit_index + MY_BUFFER_SIZE - free_space) % MY_BUFFER_SIZE;
  for (i = 0; i < size; i++) {
    mybuffer[alloc_start + i] = 1;
  }

  next_fit_index = (alloc_start + size) % MY_BUFFER_SIZE;
  return &mybuffer[alloc_start];
}

int myfree(void *ptr) {
  unsigned char *byte_ptr;

  if (ptr == NULL || ptr < (void *)mybuffer || ptr >= (void *)(mybuffer + MY_BUFFER_SIZE)) {
    return -1;
  }

  byte_ptr = (unsigned char *)ptr;
  while (byte_ptr < mybuffer + MY_BUFFER_SIZE && *byte_ptr == 1) {
    *byte_ptr = 0;
    byte_ptr++;
  }

  return 0;
}

void test_case(size_t bytes) {
  void *ptr;

  struct rusage usage_start, usage_end;
  struct timeval sys_start, sys_end, usr_start, usr_end;
  double sys_time, usr_time;  

  ptr = myalloc(bytes);
  if (ptr != NULL) {
    printf("[ INFO ] Rezervē %lu baitus adresē %p\n", (unsigned long)bytes, ptr);
  } else {
    printf("[ ERROR ] Kļūda rezervējot %lu baitus\n", (unsigned long)bytes);
  }

  if (myfree(ptr) == 0) {
    printf("Atbrīvota atmiņa adresē %p\n", ptr);
  } else {
    printf("Kļūda atbrīvojot datus adresē %p\n", ptr);
  }

  getrusage(RUSAGE_SELF, &usage_end);
  usr_end = usage_end.ru_utime;
  sys_end = usage_end.ru_stime;
  usr_time =  (usr_end.tv_sec - usr_start.tv_sec) +
              (usr_end.tv_usec - usr_start.tv_usec) / 1000000.0;
  sys_time =  (sys_end.tv_sec - sys_start.tv_sec) +
              (sys_end.tv_usec - sys_start.tv_usec) / 1000000.0;
  printf("Sistēmas laiks: %.6f sekundes\n", sys_time);
  printf("Lietotāja laiks: %.6f sekundes\n", usr_time);
  printf("Kopējais laiks: %.6f sekundes\n", sys_time + usr_time);
  printf("--------------------------------\n\n");
}

int main() {
  test_case(64);
  test_case(128);
  test_case(512);
  test_case(1024);
  test_case(2048); 
  test_case(3072); 
  test_case(4000); 
  test_case(4096); 
  return 0;
}
