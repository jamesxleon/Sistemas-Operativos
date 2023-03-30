#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5 //Number of philosophers

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];
sem_t chopsticks_sem[NUM_PHILOSOPHERS];
sem_t mutex;

void *philosopher(void *arg) {
    int id = *(int*)arg; // philosopher id
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        // Think
        printf("Philosopher %d is thinking\n", id);
        sleep(rand() % 5 + 1); // philosopher thinks for a random time between 1 and 5 second

        // Pick up chopsticks
        printf("Philosopher %d is hungry\n", id);
        sleep(rand() % 5 + 1); 

        // Wait until we can pick up both chopsticks
        sem_wait(&mutex); // Wait for the mutex semaphore to be available
        pthread_mutex_lock(&chopsticks[left]); // Lock
        pthread_mutex_lock(&chopsticks[right]); // Lock
        sem_post(&mutex); // Release the mutex semaphore

        // Eat
        printf("Philosopher %d is eating\n", id);
        sleep(rand() % 5 + 1); 

        // Put down chopsticks
        pthread_mutex_unlock(&chopsticks[left]);
        pthread_mutex_unlock(&chopsticks[right]);
        sem_post(&chopsticks_sem[left]); // Release
        sem_post(&chopsticks_sem[right]);
    }
}

int main() {
    int i;
    pthread_t threads[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize chopsticks and semaphores
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
        sem_init(&chopsticks_sem[i], 0, 1);
    }

    // Initialize mutex semaphore with value NUM_PHILOSOPHERS-1
    sem_init(&mutex, 0, NUM_PHILOSOPHERS - 1);

    // Create threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    // Wait for threads to finish
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy chopsticks and semaphores
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
        sem_destroy(&chopsticks_sem[i]);
    }

    // Destroy mutex semaphore
    sem_destroy(&mutex);

    return 0;
}

