/*

Korišćenjem programskog jezika C kreirati dva Linux procesa.
Prvi proces kreira u deljenom memorijskom segmentu matricu dimenzija
MxN i inicijalizuje istu random vrednostima. Proces 2 sumira svaku od vrsti i rezultat upisuje na
random poziciju u okviru vrste koju obrađuje.

*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHARED_MEM_KEY 10001
#define SEM_KEY_1 10012
#define SEM_KEY_2 10013
#define MAX_SIZE 80

#define N 5
#define M 6

struct buff {
    int mat[N * M];
};
union semun {
    int val;
    struct semid_ds *buf;
};

int main() {
    int pid;
    int S1, S2;
    int shmID;

    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = {0, 1, 0};

    if ((S1 = semget((key_t)SEM_KEY_1, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Greska pri kreiranju semafora!\n");
        exit(1);
    }
    if ((S2 = semget((key_t)SEM_KEY_2, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Greska pri kreiranju semafora!\n");
        exit(1);
    }
    if ((shmID = shmget((key_t)SHARED_MEM_KEY, sizeof(struct buff), 0666 | IPC_CREAT)) < 0) {
        printf("Greska pri kreiranju deljene memorije!\n");
        exit(1);
    }

    union semun sop;
    sop.val = 1;
    semctl(S1, 0, SETVAL, sop);
    sop.val = 0;
    semctl(S2, 0, SETVAL, sop);

    if ((pid = fork()) == 0) {
        // DETE
        struct buff *buf = shmat(shmID, 0, 0);
        int i, j;
        int sum;
        semop(S2, &sem_lock, 1);
        for (i = 0; i < N; i++) {
            sum = 0;
            for (j = 0; j < M; j++) {
                sum += buf->mat[i * N + j];
            }
            j = random() % M;
            buf->mat[i * N + j] = sum;
        }

        semop(S1, &sem_unlock, 1);
        shmdt(buf);

    } else {
        // RODITELJ
        struct buff *buf = shmat(shmID, 0, 0);
        int i, j;
        semop(S1, &sem_lock, 1);
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                buf->mat[i * N + j] = rand() % 100;
                printf("%d ", buf->mat[i * N + j]);
            }
            printf("\n");
        }
        printf("\n");
        semop(S2, &sem_unlock, 1);

        wait(NULL);
        semop(S1, &sem_lock, 1);

        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                printf("%d ", buf->mat[i * N + j]);
            }
            printf("\n");
        }
        semop(S2, &sem_unlock, 1);
        shmdt(buf);

        semctl(S1, 0, IPC_RMID);
        semctl(S2, 0, IPC_RMID);
    }
}
