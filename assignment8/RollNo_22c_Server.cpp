// RollNo_22_server.cpp
#include <iostream>
#include <cstring>
#include <cmath>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

float toRadians(float degrees) {
    return degrees * (M_PI / 180.0);
}

std::string calculate(const std::string& input) {
    char op[10];
    float a, b;

    // Handle trigonometric operations
    if (sscanf(input.c_str(), "%s %f", op, &a) == 2) {
        if (strcmp(op, "sin") == 0) return std::to_string(sin(toRadians(a)));
        if (strcmp(op, "cos") == 0) return std::to_string(cos(toRadians(a)));
        if (strcmp(op, "tan") == 0) return std::to_string(tan(toRadians(a)));
    }

    // Handle arithmetic operations
    if (sscanf(input.c_str(), "%f %s %f", &a, op, &b) == 3) {
        if (strcmp(op, "+") == 0) return std::to_string(a + b);
        if (strcmp(op, "-") == 0) return std::to_string(a - b);
        if (strcmp(op, "*") == 0) return std::to_string(a * b);
        if (strcmp(op, "/") == 0) {
            if (b == 0) return "Error: Division by zero";
            return std::to_string(a / b);
        }
    }

    return "Invalid operation";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address{};
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        return 1;
    }

    std::cout << "Server is running. Waiting for connection...\n";

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("Accept");
        return 1;
    }

    memset(buffer, 0, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    std::cout << "Received operation: " << buffer << std::endl;

    std::string result = calculate(buffer);
    send(new_socket, result.c_str(), result.length(), 0);
    std::cout << "Sent result: " << result << std::endl;

    close(new_socket);
    close(server_fd);
    return 0;
}
