#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>

#define SHM_SIZE 100000

int main()
{
    int shmid, semid;
    key_t key_shm = 1234;
    key_t key_sem = 5678;

    if ((shmid = shmget(key_shm, SHM_SIZE, 0666)) < 0)
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

    if ((semid = semget(key_sem, 1, 0666)) < 0)
    {
        perror("semget");
        exit(1);
    }

    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 0;
    sem_op.sem_flg = 0;

    int width, height, channel;
    unsigned char *image_data = stbi_load("my_image.png", &width, &height, &channel, 0);
    int total_pixels = width * height * channel;
    if (total_pixels >= 100000)
    {
        printf("you image is greater than 325×155×2");
        exit(1);
    }
    memcpy(shared_memory, image_data, total_pixels);

    printf("sending the data");

    sem_op.sem_op = 1;
    semop(semid, &sem_op, 1);

    sem_op.sem_op = -1;
    semop(semid, &sem_op, 1);
    printf("received the data");

    unsigned char buffer1[total_pixels];
    for (int y = 0; y < total_pixels; y++)
    {
        buffer1[y] = shared_memory[y];
    }

    stbi_write_jpg("output_image11.jpg", width, height, channel, buffer1, width * channel);

    stbi_image_free(image_data);

    shmdt(shared_memory);

    return 0;
}
