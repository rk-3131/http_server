#include "../include/Server.h"

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

Server::Server(int port) {
    this->port = port;
    serverSocket = -1;
}

void Server::start() {

    // 1. Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0) {
        cerr << "Socket creation failed\n";
        return;
    }

    cout << "Socket created successfully\n";

    // 2. Define server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    // 3. Bind socket to port
    if (bind(serverSocket,
             (sockaddr*)&serverAddress,
             sizeof(serverAddress)) < 0) {

        cerr << "Bind failed\n";
        return;
    }

    cout << "Bind successful\n";

    // 4. Listen for connections
    if (listen(serverSocket, 5) < 0) {
        cerr << "Listen failed\n";
        return;
    }

    cout << "Server listening on port "
          << port << "\n";

    while (true) {

        sockaddr_in clientAddress{};
        socklen_t clientSize = sizeof(clientAddress);

        int clientSocket = accept(
            serverSocket,
            (sockaddr*)&clientAddress,
            &clientSize
        );

        if (clientSocket < 0) {
            cerr << "Client connection failed\n";
            continue;
        }

        cout << "\nClient connected!\n";

        char buffer[4096] = {0};

        ssize_t bytesReceived =
            recv(clientSocket,
                buffer,
                sizeof(buffer),
                0);

        if (bytesReceived < 0) {
            cerr << "Receive failed\n";
            close(clientSocket);
            continue;
        }

        cout << "\n========= REQUEST =========\n";
        cout << buffer << "\n";
        cout << "===========================\n";

        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Hello from C++ server";

        send(clientSocket,
            response,
            strlen(response),
            0);

        close(clientSocket);
    }
}