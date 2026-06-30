#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5

void *worker(void *arg) {
    long id = (long)arg;
    int computed = (id + 1) * (id + 1) * 10; // any deterministic computation
    printf("Thread %ld (tid=%lu) computed value: %d\n", id, pthread_self(), computed);
    sleep(5); // make threads visible to ps -eLf while alive
    long *result = malloc(sizeof(long));
    *result = computed;
    return (void *)result;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    long results[NUM_THREADS];

    for (long i = 0; i < NUM_THREADS; i++)
        pthread_create(&threads[i], NULL, worker, (void *)i);

    int sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        void *ret;
        pthread_join(threads[i], &ret);
        long val = *(long *)ret;
        results[i] = val;
        sum += val;
        free(ret);
    }

    printf("\n--- Summary ---\n");
    for (int i = 0; i < NUM_THREADS; i++)
        printf("Worker %d result: %ld\n", i, results[i]);
    printf("Total: %d\n", sum);

    // LIVE CURVEBALL A: 2 extra workers, started only after originals joined
    printf("\n--- Spawning 2 extra workers (PID %d) ---\n", getpid());
    pthread_t extra[2];
    for (long i = 0; i < 2; i++)
        pthread_create(&extra[i], NULL, worker, (void *)(NUM_THREADS + i));

    printf("Extra workers alive now — check ps -eLf in another terminal\n");
    sleep(6); // window to capture them with ps while alive

    for (int i = 0; i < 2; i++) {
        void *ret;
        pthread_join(extra[i], &ret);
        printf("Extra worker %d result: %ld\n", i, *(long *)ret);
        free(ret);
    }
    printf("Extra workers joined and gone.\n");

    return 0;
}