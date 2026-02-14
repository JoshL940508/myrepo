#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 50000000

void add_arrays(int* result, int* a, int* b, int array_size) {
  
    while (array_size--) {
        result[array_size] = a[array_size] + b[array_size];
    }

}

void rand_init(int* a, int array_size) {
    int range_start = 0;
    int range_end = 9;
    for (int i = 0; i < array_size; i++) {
        a[i] = (rand() % (range_end - range_start + 1)) + range_start;
    }
   
}

int main(void) {
    
int* a = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    int* b = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    int* c = (int*)malloc(sizeof(int) * ARRAY_SIZE);

    if (a == NULL || b == NULL || c == NULL) {
        printf("Not enough memory.\n");
        free(a);
        free(b);
        free(c);
        return 1;
    }

    srand((unsigned)time(NULL));

    double start = clock();
    rand_init(a, ARRAY_SIZE);
    rand_init(b, ARRAY_SIZE);
    double stop = clock();

    printf("Total init time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    start = clock();
    add_arrays(c, a, b, ARRAY_SIZE);
    stop = clock();

    for (int i = 0; i < 10; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");

    printf("Total add time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    free(a);
    free(b);
    free(c);
    return 0;
   
}
