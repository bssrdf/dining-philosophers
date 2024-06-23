#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

/* This version is written by chatgpt with prompt

```
write a C program using posix thread and semaphore simulating 
dining philosopher problem. explain in detail every line of code

modify the above program such that deadlock could happen, i.e., 
each philosopher can pick one chopstick and no one can pick up two
```

This code assumes philosophers sit in a counter clockwise fasion 
with increading id number (0-4)


This version may trigger a deadlock situation.

*/


#define N 5  // Number of philosophers

sem_t chopsticks[N];  // Semaphore array for chopsticks

void *philosopher(void *num) {
    int id = *(int *)num;

    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);  // Simulating thinking

        // Pick up left chopstick
        sem_wait(&chopsticks[id]);
        printf("Philosopher %d picked up left chopstick\n", id);

        // Simulate delay to increase the likelihood of deadlock
        sleep(1);

        // Pick up right chopstick
        sem_wait(&chopsticks[(id + 1) % N]);
        printf("Philosopher %d picked up right chopstick\n", id);

        // Eat
        printf("Philosopher %d is eating\n", id);
        sleep(1);  // Simulating eating

        // Put down right chopstick
        sem_post(&chopsticks[(id + 1) % N]);
        printf("Philosopher %d put down right chopstick\n", id);

        // Put down left chopstick
        sem_post(&chopsticks[id]);
        printf("Philosopher %d put down left chopstick\n", id);
    }
}

int main() {
    pthread_t thread_id[N];
    int phil_num[N];

    // Initialize semaphores
    for (int i = 0; i < N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        phil_num[i] = i;
        pthread_create(&thread_id[i], NULL, philosopher, &phil_num[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < N; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
