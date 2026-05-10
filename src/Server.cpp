#include "../include/Server.h"

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port) {
    this->port = port;
    serverSocket = -1;
}

void Server::start() {

    // 1. Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        std::cerr << "Socket creation failed\n";
        return;
    }

    std::cout << "Socket created successfully\n";

    // 2. Define server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // 3. Bind socket to port
    if (bind(serverSocket,
             (sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0) {

        std::cerr << "Bind failed\n";
        return;
    }

    std::cout << "Bind successful\n";

    // 4. Listen for connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listen failed\n";
        return;
    }

    std::cout << "Server listening on port "
              << port << "\n";

    // 5. Accept client connection
    sockaddr_in clientAddress{};
    socklen_t clientSize = sizeof(clientAddress);

    int clientSocket = accept(
        serverSocket,
        (sockaddr*)&clientAddress,
        &clientSize
    );

    if (clientSocket < 0) {
        std::cerr << "Client connection failed\n";
        return;
    }

    std::cout << "Client connected!\n";

    // 6. Receive data
    char buffer[4096] = {0};

    ssize_t bytesReceived =
        recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived < 0) {
        std::cerr << "Receive failed\n";
        return;
    }

    std::cout << "\n========= REQUEST =========\n";
    std::cout << buffer << "\n";
    std::cout << "===========================\n";

    // 7. Send response
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello from C++ server";

    send(clientSocket,
         response,
         strlen(response),
         0);

    // 8. Close sockets
    close(clientSocket);
    close(serverSocket);
}