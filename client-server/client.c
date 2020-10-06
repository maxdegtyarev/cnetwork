#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 3001
#define true 1
#define BUFFER_SIZE 256
#define RESULT_SIZE 128
#define IP_ADDRESS "127.0.0.1"
#define ERROR -1

int main(int argc, char* argv[]) {
    int connFD;
    struct sockaddr_in adr = {0};

    if ((connFD = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
        perror("Could not create a new socket file descriptor");
        exit(EXIT_FAILURE);
    }

    adr.sin_port = htons(PORT);
    adr.sin_family = AF_INET;

    if (inet_pton(AF_INET, IP_ADDRESS, &adr.sin_addr) == ERROR) {
        perror("Could not translate address");
        exit(EXIT_FAILURE);
    }

    if ((connect(connFD, (struct sockaddr*) &adr, sizeof(adr))) == ERROR) {
        perror("Could not connect this socket to server");
        close(connFD);
        exit(EXIT_FAILURE);
    }

    char request[BUFFER_SIZE] = {0};

    strcat(request, argv[1]);
    strcat(request, " ");
    strcat(request, argv[2]);

    write(connFD, request, BUFFER_SIZE);

    char buffer[BUFFER_SIZE] = {0};

    int nread = read(connFD, buffer, BUFFER_SIZE);

    if (nread == ERROR) {
        perror("Could not read data");
        close(connFD);
        exit(EXIT_FAILURE);
    }

    printf("Answer: %s\n", buffer);
    
    return 0;
}