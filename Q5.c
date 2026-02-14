#include <stdio.h> 
#include <stdlib.h>
#include <time.h>	//used for timing - background notes.
#include <pthread.h>

#define ARRAY_SIZE 50000000

struct data {
  int* a;
  int start_idx;
  int* sum;
};

void* sum_array(void* arg) {
    struct data thread_data = *(struct data*) arg;

    int mid = ARRAY_SIZE / 2;
    int start = thread_data.start_idx;
    int end;
    if (start == 0){
        end = mid;
    }
    else{
        end = ARRAY_SIZE;
    }
    
    int local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += thread_data.a[i];
    }

    *(thread_data.sum) = local_sum;

    pthread_exit(NULL);
}

int main(void) {
    int* a = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    if (a == NULL) {
        printf("Not enough memory.\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = rand() % 10;
    }

    pthread_t t0, t1;

    int sum0 = 0;
    int sum1 = 0;

    struct data d0;
    struct data d1;

    d0.a = a; d0.start_idx = 0; d0.sum = &sum0;
    d1.a = a; d1.start_idx = ARRAY_SIZE / 2; d1.sum = &sum1;

    double start = clock();

    pthread_create(&t0, NULL, sum_array, (void*)&d0);
    pthread_create(&t1, NULL, sum_array, (void*)&d1);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    int threaded_sum = sum0 + sum1;

    double stop = clock();

    printf("Threaded sum = %d\n", threaded_sum);
    printf("Total Threaded sum time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    int serial_sum = 0;
    start = clock();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        serial_sum += a[i];
    }
    stop = clock();

    printf("Serial sum = %d\n", serial_sum);
    printf("Total Serial sum time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    if (serial_sum != threaded_sum) {
        printf("ERROR: sums do not match!\n");
        free(a);
        return 1;
    }

    free(a);
    return 0;
}
