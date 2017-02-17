#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS NUM_PHILOSOPHERS
#define THINKING_TIME 1000000
#define EATING_TIME 500000

pthread_mutex_t chopsticks[NUM_CHOPSTICKS];
pthread_t philosophers[NUM_PHILOSOPHERS];

void grab_chopstick(int philos_id, int chopstick_id);
void drop_chopsticks(int philos_id, int chopstick_left, int chopstick_right);
void *philosopher(void *param);

int main(void) {
    int t;
    for(t = 0; t < NUM_CHOPSTICKS; ++t) {
        pthread_mutex_init(&chopsticks[t], NULL);
    }

    for(t = 0; t < NUM_PHILOSOPHERS; ++t) {
        pthread_create(&philosophers[t], NULL, philosopher, (void *) t);
    }

    for(t = 0; t < NUM_PHILOSOPHERS; ++t) {
        pthread_join(philosophers[t], NULL);
    }

    for(t = 0; t < NUM_CHOPSTICKS; ++t) {
        pthread_mutex_destroy(&chopsticks[t]);
    }

    return 0;
}

void *philosopher(void *param) {
    int id = (int) param;
    int left_chopstick = id;
    int right_chopstick = (id + 1 == NUM_CHOPSTICKS) ? 0 : (id + 1);

    while(1) {
        printf("Philosopher %d is thinking...\n", id);
        usleep(THINKING_TIME);

        printf("Philosopher %d wants to eat!\n", id);
        grab_chopstick(id, left_chopstick);
        grab_chopstick(id, right_chopstick);
        printf("Philosopher %d started to eat!\n", id);

        usleep(EATING_TIME);
        printf("Philosopher %d ate everything he needed. Going back to meditation...\n", id);
        drop_chopsticks(id, left_chopstick, right_chopstick);
    }
}

void grab_chopstick(int philos_id, int chopstick_id) {
    pthread_mutex_lock(&chopsticks[chopstick_id]);   
    printf("Philosopher %d got chopstick %d\n", philos_id, chopstick_id);
}

void drop_chopsticks(int philos_id, int chopstick_left, int chopstick_right) {
    pthread_mutex_unlock(&chopsticks[chopstick_left]);
    pthread_mutex_unlock(&chopsticks[chopstick_right]);
    printf("Philosopher %d dropped its chopsticks\n");
}
