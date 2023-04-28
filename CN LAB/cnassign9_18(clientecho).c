#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 9020
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char *file_contents;

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    // Set server details
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return -1;
    }

    // Send filename to server
    char *filename = "file.txt";
    if (send(sock, filename, strlen(filename), 0) < 0) {
        perror("send failed");
        return -1;
    }

    // Read file contents from server
    file_contents = malloc(BUFFER_SIZE * sizeof(char));
    valread = read(sock, file_contents, BUFFER_SIZE);
    if (valread < 0) {
        perror("read failed");
        return -1;
    }

    printf("File contents:\n%s\n", file_contents);

    close(sock);
    free(file_contents);

    return 0;
}
