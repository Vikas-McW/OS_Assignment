#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{

    char *ip = "127.0.0.1";
    int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while (1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+]Client connected.\n");

        unsigned char buffer[20000];
        bzero(buffer, 20000);
        printf("receiving the data");
        int v1 = recv(client_sock, buffer, sizeof(buffer), 0);
        // printf("%d\n", v1);
        // printf("Client: %s\n", buffer);
        unsigned char buffer1[20000];
        printf("%ld\n", sizeof(buffer));
        // printf("%d\n%d\n", buffer[0], buffer[10]);
        for (int i = 0; i < sizeof(buffer); ++i)
        {
            // Perform image negation by subtracting the pixel value from 255 (for 8-bit images)
            buffer1[i] = 255 - buffer[i];
        }
        printf("%d", buffer1[0]);
        printf("%ld", sizeof(buffer1));
        //  bzero(buffer, 1024);
        //  strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        //  printf("Server: %s\n", buffer);
        send(client_sock, buffer1, sizeof(buffer1), 0);

        close(client_sock);
        printf("[+]Client disconnected.\n\n");
    }

    return 0;
}