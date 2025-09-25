// RollNo_22_server.cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    // Set server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen for connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        return 1;
    }

    std::cout << "Server: Waiting for client connection..." << std::endl;

    // Accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address,
                        (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    std::cout << "Server: Client connected. Receiving file..." << std::endl;

    std::ofstream outfile("received_file.txt", std::ios::binary);
    if (!outfile) {
        std::cerr << "Error creating file to write\n";
        return 1;
    }

    int bytesReceived;
    while ((bytesReceived = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        outfile.write(buffer, bytesReceived);
    }

    std::cout << "Server: File received and saved as 'received_file.txt'.\n";

    outfile.close();
    close(new_socket);
    close(server_fd);

    return 0;
}
