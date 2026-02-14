#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define ARRAY_SIZE 50000000

// The following is a struct in C.
// This paradigm allows for arbitrary data to be stored
// in a structured way. This also allows for multiple
// parameters to be passed to a thread function.
// You should use this struct with the add_arrays
// function when running using threads.
struct data {
  int* a;
  int* b;
  int* c;
  int start_idx; // Where the thread should start in an array
};

void* add_arrays(void* arg) {
	struct data thread_data = *(struct data*) arg;
    int start = thread_data.start_idx;
    int end;

    if (start == 0) {
        end = ARRAY_SIZE / 2;
    }

    else {
        end = ARRAY_SIZE;
    }
    for (int i = start; i < end; i++) {
        thread_data.c[i] = thread_data.a[i] + thread_data.b[i];
    }

    pthread_exit(NULL);
}

void* rand_init(void* arg) {
    int range_start = 0;
    int range_end = 9;
    int* arr = (int*)arg;

    unsigned int rand_state = (unsigned)time(NULL) ^ (unsigned)getpid() ^ (unsigned long)pthread_self();

    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = (rand_r(&rand_state) % (range_end - range_start + 1)) + range_start;
    }
    pthread_exit(NULL);
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

    pthread_t t0, t1;
    double start = clock();

    pthread_create(&t0, NULL, rand_init, (void*)a);
    pthread_create(&t1, NULL, rand_init, (void*)b);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    double stop = clock();
    printf("Total init time: %.2f ms\n", 1000 * (stop - start) / CLOCKS_PER_SEC);

    struct data d0;
    struct data d1;
    d0.a = a; d0.b = b; d0.c = c; d0.start_idx = 0;
    d1.a = a; d1.b = b; d1.c = c; d1.start_idx = ARRAY_SIZE / 2;

    start = clock();

    pthread_create(&t0, NULL, add_arrays, (void*)&d0);
    pthread_create(&t1, NULL, add_arrays, (void*)&d1);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

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
