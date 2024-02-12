#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <ctype.h>

#define SHM_SIZE 1024 // Size of the shared memory segment

int main()
{
    int shmid, semid;
    key_t key_shm = 1234; // Unique key for shared memory segment
    key_t key_sem = 5678; // Unique key for semaphore

    // Create or access the shared memory segment
    if ((shmid = shmget(key_shm, SHM_SIZE, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        exit(1);
    }

    // Attach to the shared memory segment
    char *shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    // Create or access the semaphore
    if ((semid = semget(key_sem, 1, IPC_CREAT | 0666)) < 0)
    {
        perror("semget");
        exit(1);
    }

    // Set semaphore value to 0 (locked initially)
    semctl(semid, 0, SETVAL, 0);

    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1; // Wait for semaphore to be unlocked
    sem_op.sem_flg = 0;

    // Wait for the semaphore to be unlocked
    semop(semid, &sem_op, 1);

    // Modify data in shared memory (convert to uppercase)
    int i = 0;
    while (shared_memory[i] != '\0')
    {
        shared_memory[i] = toupper(shared_memory[i]);
        i++;
    }

    printf("Server has modified the data: %s\n", shared_memory);

    // Signal client that modifications are done
    sem_op.sem_op = 1; // Unlock the semaphore
    semop(semid, &sem_op, 1);

    // Detach from the shared memory
    shmdt(shared_memory);

    return 0;
}
