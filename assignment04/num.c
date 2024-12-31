#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 3

int *numbers;
int count;

typedef struct {
    int         start_idx;
    int         end_idx;
    long long   partial_sum;
    long long   partial_min;
    long long   partial_max;
} thread_data_t;

// thread : start_idx ~ end_idx까지 순회하여 구간합, 구간최소, 구간최대 계산
void* worker(void *arg) {
    thread_data_t *data = (thread_data_t*)arg;
    int start = data->start_idx;
    int end   = data->end_idx;

    data->partial_sum = 0;
    data->partial_min = numbers[start];
    data->partial_max = numbers[start];

    for (int i = start; i <= end; i++) {
        data->partial_sum += numbers[i];
        if (numbers[i] < data->partial_min)     data->partial_min = numbers[i];
        if (numbers[i] > data->partial_max)     data->partial_max = numbers[i];
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <integers>\n", argv[0]);
        return 1;
    }

    count = argc - 1;
    
    numbers = (int *)malloc(count * sizeof(int));
    if (numbers == NULL) {
        fprintf(stderr, "Memory Allocate\n");
        return 1;
    }

    // parsing integer
    for (int i = 1; i < argc; i++)  numbers[i - 1] = atoi(argv[i]);

    pthread_t threads[3];
    thread_data_t thread_data[3];
    int start_idx = 0;

    for (int i = 0; i < 3; i++) {
        int segment_size = (count / 3) + (i < (count % 3) ? 1 : 0);

        // start_idx부터 segment_size만큼
        thread_data[i].start_idx = start_idx;
        thread_data[i].end_idx   = start_idx + segment_size - 1;

        start_idx += segment_size;
    }

    // 스레드 생성
    for (int i = 0; i < 3; i++)     pthread_create(&threads[i], NULL, worker, (void *)&thread_data[i]);
    for (int i = 0; i < 3; i++)     pthread_join(threads[i], NULL);

    long long total_sum = 0;
    int global_min = thread_data[0].partial_min;
    int global_max = thread_data[0].partial_max;

    for (int i = 0; i < 3; i++) {
        total_sum += thread_data[i].partial_sum;

        if (thread_data[i].partial_min < global_min)    global_min = thread_data[i].partial_min;
        if (thread_data[i].partial_max > global_max)    global_max = thread_data[i].partial_max;
    }
    double average = (double)total_sum / count;

    printf("The average value is %.f\n", average);
    printf("The minimum value is %d\n", global_min);
    printf("The maximum value is %d\n", global_max);

    free(numbers);
    return 0;
}