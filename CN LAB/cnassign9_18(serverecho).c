#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 9020
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread, num_clients = 5;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *file_contents;
    FILE *file;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to the port 9020
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the port 9020
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, num_clients) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for clients to connect...\n");

    while (1) {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read filename sent by client
        memset(buffer, 0, BUFFER_SIZE);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        // Open file and read contents
        file = fopen(buffer, "r");
        if (file == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        rewind(file);
        file_contents = malloc((file_size + 1) * sizeof(char));
        fread(file_contents, sizeof(char), file_size, file);
        fclose(file);
        file_contents[file_size] = '\0';

        // Send file contents back to client
        if (send(new_socket, file_contents, strlen(file_contents), 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }

        close(new_socket);
    }

    return 0;
}
