// RollNo_22_client.cpp
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr{};
    char buffer[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Set server IP (localhost or remote)
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return 1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return 1;
    }

    std::cout << "Client: Connected to server.\n";

    std::ifstream infile("send_file.txt", std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening file to send\n";
        return 1;
    }

    std::cout << "Client: Sending file 'send_file.txt'...\n";

    int bytesRead;
    while ((bytesRead = infile.readsome(buffer, BUFFER_SIZE)) > 0) {
        send(sock, buffer, bytesRead, 0);
    }

    std::cout << "Client: File sent successfully.\n";

    infile.close();
    close(sock);

    return 0;
}
