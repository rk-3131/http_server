#pragma once
using namespace std;

class Server {
public:
    Server(int port);

    void start();

private:
    int port;
    int serverSocket;
};