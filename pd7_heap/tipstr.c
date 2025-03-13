#include <stdio.h>
#include <stddef.h>

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

  ptr = myalloc(bytes);
  if (ptr != NULL) {
    printf("Allocated %lu bytes at %p\n", (unsigned long)bytes, ptr);
  } else {
    printf("Failed to allocate %lu bytes\n", (unsigned long)bytes);
  }

  if (myfree(ptr) == 0) {
    printf("Freed memory at %p\n", ptr);
  } else {
    printf("Failed to free memory at %p\n", ptr);
  }
}

int main() {
  test_case(100);
  test_case(200);
  test_case(50);
  test_case(1024);
  test_case(2048); 
  test_case(3072); 
  test_case(4000); 
  test_case(4096); 
  return 0;
}
