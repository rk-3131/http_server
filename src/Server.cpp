#include "../include/Server.h"

#include <iostream>
#include <cstring>
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fstream>

using namespace std;

Server::Server(int port) {
    this->port = port;
    serverSocket = -1;
}

string getContentType(const string& path) {

    if (path.find(".html") != string::npos)
        return "text/html";

    if (path.find(".css") != string::npos)
        return "text/css";

    if (path.find(".js") != string::npos)
        return "application/javascript";

    return "text/plain";
}

string readFile(const string& filePath) {
    cout << "Inside of the readfile function "<<endl;
    ifstream file(filePath);

    if (!file.is_open()) {
        return "Nothing found";
    }

    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    return content;
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

        // Convert request into string
        string request(buffer);

        cout << "\n========= REQUEST =========\n";
        cout << request << "\n";
        cout << "===========================\n";

        // Parse HTTP request
        istringstream requestStream(request);

        string method;
        string path;
        string version;

        requestStream >> method >> path >> version;

        cout << "Method: " << method << endl;
        cout << "Path: " << path << endl;
        cout << "Version: " << version << endl;

        string filePath;

        if (path == "/") {
            filePath = "../public/index.html";
        }
        else {
            filePath = "../public" + path;
        }
        string body = readFile(filePath);
        if (body.empty()) {

            body = "<h1>404 File Not Found</h1>";

            string response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + to_string(body.size()) + "\r\n"
                "\r\n" +
                body;

            send(clientSocket,
            response.c_str(),
            response.size(),
            0);

            close(clientSocket);

            continue;
        }
        // Build HTTP response
        string contentType = getContentType(filePath);

        string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: " + contentType + "\r\n"
            "Content-Length: " + to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        send(clientSocket,
             response.c_str(),
             response.size(),
             0);

        close(clientSocket);
    }
}

