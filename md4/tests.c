#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

#define MB (1024 * 1024)  

#ifndef MAP_ANON
#define MAP_ANON 0x20  /* Tipiska vērtība daudzām sistēmām */
#endif

/* Funkcija atmiņas rezervēšanai ar sbrk() */
void *reserve_memory_sbrk(size_t size) {
    intptr_t result;
    void *ptr;

    result = (intptr_t)sbrk(size);  
    if (result == -1) {
        perror("sbrk failed");
        exit(1);
    }

    ptr = (void *)result;  
    return ptr;
}

/* Funkcija atmiņas rezervēšanai ar mmap() */
void *reserve_memory_mmap(size_t size) {
    void *ptr;
    ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    return ptr;
}

/* Funkcija atmiņas rezervēšanai ar malloc() */
void *reserve_memory_malloc(size_t size) {
    void *ptr;
    ptr = malloc(size);  /* Rezervē atmiņu ar malloc */
    if (ptr == NULL) {
        perror("malloc failed");
        exit(1);
    }
    return ptr;
}

int main() {
    size_t block_size = MB;  
    void *ptr;

    /* Rezervējam atmiņu ar sbrk */
    ptr = reserve_memory_sbrk(block_size);
    printf("Atmiņa rezervēta ar sbrk: 1 MB\n");

    /* Rezervējam atmiņu ar mmap */
    ptr = reserve_memory_mmap(block_size);
    printf("Atmiņa rezervēta ar mmap: 1 MB\n");

    /* Rezervējam atmiņu ar malloc */
    ptr = reserve_memory_malloc(block_size);
    printf("Atmiņa rezervēta ar malloc: 1 MB\n");


    return 0;
}
