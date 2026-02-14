#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 50000000

void add_arrays(int* result, int* a, int* b, int array_size) {
    // @TODO
	// Add a + b together and store in the result array.
    // DO NOT create any local variables in this function.
    // +4
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
    // @TODO
    // Randomly initialize an array of size array_size
    // using numbers between 0 and 9.
    // +2


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

    /* Time initialization of a and b */
    double start = clock();
    rand_init(a, ARRAY_SIZE);
    rand_init(b, ARRAY_SIZE);
    double stop = clock();

    printf("Total init time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    /* Time add */
    start = clock();
    add_arrays(c, a, b, ARRAY_SIZE);
    stop = clock();

    /* Print first 10 values of c on one line (like the sample) */
    for (int i = 0; i < 10; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");

    printf("Total add time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    free(a);
    free(b);
    free(c);
    return 0;
    // @TODO
	// Allocate the source arrays (a, b, c)
    // a and b are the source arrays.
    // c is the result storage array.
    // c[i] should equal a[i] + b[i]
    // Make sure to error check.
    // +1
    
    // @TODO
    // Initialize the two arrays, a and b, with random values 
    // of size ARRAY_SIZE and measure the time it takes to do so.
    // +1

    // @TODO
    // Print the total time for initialization
    // +1

    // @TODO
    // Run and time the add operation, print the first ten elements
    // of the result array, and print the total add time.
    // +2

    return 0;
}
