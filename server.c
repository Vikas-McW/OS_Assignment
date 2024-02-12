#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>
#include <limits.h>
#define SHM_SIZE 100000

int main()
{
    while (1)
    {

        int shmid, semid;
        key_t key_shm = 1234;
        key_t key_sem = 5678;

        if ((shmid = shmget(key_shm, SHM_SIZE, IPC_CREAT | 0666)) < 0)
        {
            perror("shmget");
            exit(1);
        }

        unsigned char *shared_memory = shmat(shmid, NULL, 0);
        if (shared_memory == (unsigned char *)-1)
        {
            perror("shmat");
            exit(1);
        }

        if ((semid = semget(key_sem, 1, IPC_CREAT | 0666)) < 0)
        {
            perror("semget");
            exit(1);
        }

        semctl(semid, 0, SETVAL, 0);

        struct sembuf sem_op;
        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;

        semop(semid, &sem_op, 1);

        int i = 0;
        while (shared_memory[i] != '\0')
        {
            shared_memory[i] = 255 - shared_memory[i];

            i++;
        }

        sem_op.sem_op = 1;
        semop(semid, &sem_op, 1);

        shmdt(shared_memory);
    }

    return 0;
}