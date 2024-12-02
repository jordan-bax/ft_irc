#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#define SERVER_PORT 8080
#define PROXY_PORT 9090
#define BUFFER_SIZE 1024

void handle_connection(int client_socket, int server_socket) {
    char buffer[BUFFER_SIZE] = {0};

    // Proxy communication loop
    while (true) {
        // Receive data from client
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            std::cout << "Client disconnected" << std::endl;
            break;
        }
        std::cout << "Client -> Proxy: " << buffer;

        // Forward data to the server
        send(server_socket, buffer, bytes_read, 0);

        // Receive response from server
        memset(buffer, 0, BUFFER_SIZE);
        bytes_read = read(server_socket, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            std::cout << "Server disconnected" << std::endl;
            break;
        }
        std::cout << "Server -> Proxy: " << buffer;

        // Forward response back to the client
        send(client_socket, buffer, bytes_read, 0);
    }
}

int main() {
    int proxy_server_fd, client_socket, server_socket;
    struct sockaddr_in proxy_addr, client_addr, server_addr;
    int addrlen = sizeof(proxy_addr);

    // Create proxy server socket
    if ((proxy_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Proxy socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure proxy address
    proxy_addr.sin_family = AF_INET;
    proxy_addr.sin_addr.s_addr = INADDR_ANY;
    proxy_addr.sin_port = htons(PROXY_PORT);

    // Bind the proxy socket
    if (bind(proxy_server_fd, (struct sockaddr *)&proxy_addr, sizeof(proxy_addr)) < 0) {
        perror("Proxy bind failed");
        close(proxy_server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(proxy_server_fd, 3) < 0) {
        perror("Proxy listen failed");
        close(proxy_server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Proxy listening on port " << PROXY_PORT << std::endl;

    // Accept client connection
    if ((client_socket = accept(proxy_server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen)) < 0) {
        perror("Proxy accept client failed");
        close(proxy_server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "Client connected to proxy" << std::endl;

    // Connect to the actual server
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Proxy socket to server creation failed");
        close(proxy_server_fd);
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    // Convert localhost address
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(proxy_server_fd);
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(proxy_server_fd);
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    std::cout << "Proxy connected to server" << std::endl;

    // Start relaying data between client and server
    handle_connection(client_socket, server_socket);

    // Clean up
    close(client_socket);
    close(server_socket);
    close(proxy_server_fd);

    return 0;
}
