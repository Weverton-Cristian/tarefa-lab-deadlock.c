#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1, lock2;
int resource1_count = 0, resource2_count = 0; // Contadores de recursos

void* processA(void* arg) {
    while (1) {
        // Tenta adquirir os locks na ordem correta
        pthread_mutex_lock(&lock1);
        resource1_count++;
        printf("Process A acquired lock 1 (Resource1 count: %d)\n", resource1_count);

        if (pthread_mutex_trylock(&lock2) == 0) { // Tenta adquirir o segundo lock sem bloqueio
            resource2_count++;
            printf("Process A acquired lock 2 (Resource2 count: %d)\n", resource2_count);

            // Simula processamento
            sleep(1);

            // Libera os locks
            pthread_mutex_unlock(&lock2);
            resource2_count--;
            pthread_mutex_unlock(&lock1);
            resource1_count--;
            break; // Sai do loop após realizar o trabalho
        } else {
            // Não conseguiu adquirir lock2, então libera lock1 e tenta de novo
            pthread_mutex_unlock(&lock1);
            resource1_count--;
            printf("Process A could not acquire lock 2, retrying...\n");
            sleep(1); // Espera um pouco antes de tentar novamente
        }
    }
    return NULL;
}

void* processB(void* arg) {
    while (1) {
        // Tenta adquirir os locks na ordem correta
        pthread_mutex_lock(&lock2);
        resource2_count++;
        printf("Process B acquired lock 2 (Resource2 count: %d)\n", resource2_count);

        if (pthread_mutex_trylock(&lock1) == 0) { // Tenta adquirir o segundo lock sem bloqueio
            resource1_count++;
            printf("Process B acquired lock 1 (Resource1 count: %d)\n", resource1_count);

            // Simula processamento
            sleep(1);

            // Libera os locks
            pthread_mutex_unlock(&lock1);
            resource1_count--;
            pthread_mutex_unlock(&lock2);
            resource2_count--;
            break; // Sai do loop após realizar o trabalho
        } else {
            // Não conseguiu adquirir lock1, então libera lock2 e tenta de novo
            pthread_mutex_unlock(&lock2);
            resource2_count--;
            printf("Process B could not acquire lock 1, retrying...\n");
            sleep(1); // Espera um pouco antes de tentar novamente
        }
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_create(&t1, NULL, processA, NULL);
    pthread_create(&t2, NULL, processB, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
