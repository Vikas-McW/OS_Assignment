#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <math.h>

int main()
{
    int width, height, channel;
    unsigned char *image_data = stbi_load("download.png", &width, &height, &channel, 0);
    int total_pixels = width * height * channel;
    unsigned char buffer[20000];
    unsigned char buffer1[total_pixels];
    int i, k = total_pixels / 20000, c = 1, x = -1, y = -1, j;
    printf("%d", k);
    printf("%d", (total_pixels - (20000 * k)));
    for (i = 0; i < k; i++)
    {
        char *ip = "127.0.0.1";
        int port = 5566;

        int sock, sock1;
        struct sockaddr_in addr;
        socklen_t addr_size;

        int n;

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("[-]Socket error");
            exit(1);
        }
        printf("[+]TCP server socket created.\n");

        memset(&addr, '\0', sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = port;
        addr.sin_addr.s_addr = inet_addr(ip);

        connect(sock, (struct sockaddr *)&addr, sizeof(addr));
        printf("Connected to the server.\n");
        bzero(buffer, 20000);

        for (j = 0; j <= 20000; j++)
        {
            buffer[j] = image_data[x++];
        }

        send(sock, buffer, 20000, 0);
        recv(sock, buffer, 20000, 0);
        for (j = 0; j < 20000; j++)
        {
            buffer1[y++] = buffer[j];
        }
        close(sock);
        c = c + 1;
    }

    int p = total_pixels - (20000 * k), l = -1;
    bzero(buffer, p);
    for (int y = 20000 * k; y < total_pixels; y++)
    {
        buffer[l = l + 1] = image_data[y];
    }
    char *ip = "127.0.0.1";
    int port = 5566;

    int sock, sock1;
    struct sockaddr_in addr;
    socklen_t addr_size;

    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");
    send(sock, buffer, 20000, 0);
    recv(sock, buffer, 20000, 0);
    for (j = 0; j < p; j++)
    {
        buffer1[y++] = buffer[j];
    }
    close(sock);
    stbi_write_jpg("output_image1.jpg", width, height, channel, buffer1, width * channel);

    stbi_image_free(image_data);
    printf("Disconnected from the server.\n");

    return 0;
}