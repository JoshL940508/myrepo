#include <stdio.h> 
#include <stdlib.h>
#include <time.h>	//used for timing - background notes.
#include <pthread.h>

#define ARRAY_SIZE 50000000
pthread_mutex_t lock;

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

    pthread_mutex_lock(&lock);
    *(thread_data.sum) += local_sum;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(void) {
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Lock init failed\n");
        return 1;
    }

    int* a = (int*)malloc(sizeof(int) * ARRAY_SIZE);
    if (a == NULL) {
        printf("Not enough memory.\n");
        pthread_mutex_destroy(&lock);
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < ARRAY_SIZE; i++) {
        a[i] = rand() % 10;
    }

    pthread_t t0, t1;
    int threaded_sum = 0;

    struct data d0;
    struct data d1;

    d0.a = a; d0.start_idx = 0; d0.sum = &threaded_sum;
    d1.a = a; d1.start_idx = ARRAY_SIZE / 2; d1.sum = &threaded_sum;

    double start = clock();

    pthread_create(&t0, NULL, sum_array, (void*)&d0);
    pthread_create(&t1, NULL, sum_array, (void*)&d1);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    double stop = clock();

    printf("Threaded sum = %d\n", threaded_sum);
    printf("Total threaded sum time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    /* Serial sum */
    int serial_sum = 0;
    start = clock();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        serial_sum += a[i];
    }
    stop = clock();

    printf("Serial sum = %d\n", serial_sum);
    printf("Total Serial sum time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    if (serial_sum != threaded_sum) {
        printf("ERROR: Summation do not match\n");
        free(a);
        pthread_mutex_destroy(&lock);
        return 1;
    }

    free(a);
    pthread_mutex_destroy(&lock);

    return 0;
}
