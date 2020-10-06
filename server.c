#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 3001
#define QUEUE_NUM 5
#define true 1
#define BUFFER_SIZE 256
#define RESULT_SIZE 128
#define ERROR -1

int combination(int n, int k)
{
    if(k > n) return 0;
    return factorial(n)/(factorial(n-k)*factorial(k));
}

int factorial(int n)
{
    int result = 1;
    for(int i = 1; i <= n; i++)
        result *= i;

    return result;
}


int main(void) {
    
    int serverFD;
    struct sockaddr_in adr = {0};

    if ((serverFD = socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
        perror("Could not create a new socket file descriptor");
        exit(EXIT_FAILURE);
    }

    adr.sin_port = htons(PORT);
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((bind(serverFD, (struct sockaddr*) &adr, sizeof(adr))) == ERROR) {
        perror("Could not bind this socket to server");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    if (listen(serverFD, QUEUE_NUM) == ERROR) {
        perror("Could not listen this socket");
        close(serverFD);
        exit(EXIT_FAILURE);
    }

    while (true) {

        char buffer[BUFFER_SIZE] = {0};
        int status;
        
        socklen_t adrlen = sizeof(adr);
        
        int connectionFD = accept(serverFD, (struct sockaddr*) &adr, &adrlen);

        status = read(connectionFD, buffer, BUFFER_SIZE);

        if (status == ERROR) {
            perror("Could not read a sequense of bytes");
            close(serverFD);
            exit(EXIT_FAILURE);
        } else if (status == 0) {
            perror("End of byte stream");
        }

        char *strN = strtok(buffer, " ");
        char *strK = strtok(NULL, " ");

        char result[RESULT_SIZE] = {0};

        sprintf(result, "%d", combination(
            atoi(strN),
            atoi(strK)
        ));

        write(connectionFD, result, RESULT_SIZE);

        shutdown(connectionFD, SHUT_RDWR);

        close(connectionFD);
    }


    close(serverFD);

    return EXIT_SUCCESS;
}