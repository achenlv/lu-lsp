#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_MEMORY 1024

// Funkcija, lai nolasītu skaitļus no faila
int read_next_int(FILE *fp) {
    int num;
    if (fscanf(fp, "%d", &num) == 1) {
        return num;
    } else {
        return EOF;
    }
}

// Funkcija, lai aprēķinātu fragmentāciju
double calculate_fragmentation(int memory[]) {
    int free_memory = 0;
    for (int i = 0; i < TOTAL_MEMORY; i++) {
        if (memory[i] == 0) {
            free_memory++;
        }
    }
    return (double)free_memory / TOTAL_MEMORY;
}

// Funkcija, lai veiktu First Fit algoritmu
int first_fit(FILE *chunks_fs, FILE *sizes_fs) {
    int size;
    int memory[TOTAL_MEMORY] = {0};
    int unallocated_memory = 0;

    while ((size = read_next_int(sizes_fs)) != EOF) {
        int start_index = -1;
        for (int j = 0; j <= TOTAL_MEMORY - size; j++) {
            int available = 1;
            for (int k = 0; k < size; k++) {
                if (memory[j + k] == 1) {
                    available = 0;
                    break;
                }
            }
            if (available) {
                start_index = j;
                break;
            }
        }
        if (start_index != -1) {
            for (int j = 0; j < size; j++) {
                memory[start_index + j] = 1;
            }
        } else {
            unallocated_memory += size;
        }
    }
    printf("First Fit: Fragmentation = %f, Unallocated Memory = %d\n", calculate_fragmentation(memory), unallocated_memory);
    return unallocated_memory;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Lietošana: %s -c <chunks_fails> -s <sizes_fails>\n", argv[0]);
        return 1;
    }

    char *chunks_file = NULL;
    char *sizes_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            chunks_file = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0) {
            sizes_file = argv[++i];
        }
    }

    if (chunks_file == NULL || sizes_file == NULL) {
        printf("Nepareizi parametri\n");
        return 1;
    }

    FILE *chunks_fs = fopen(chunks_file, "r");
    FILE *sizes_fs = fopen(sizes_file, "r");

    if (chunks_fs == NULL || sizes_fs == NULL) {
        printf("Nevar atvērt failus\n");
        return 1;
    }

    clock_t start_time = clock();
    first_fit(chunks_fs, sizes_fs);
    clock_t end_time = clock();

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("First Fit: Execution Time = %f seconds\n", time_taken);

    fclose(chunks_fs);
    fclose(sizes_fs);

    return 0;
}