#include "../include/MIME_type.h"
#include "../include/FileHandler.h"

#include <thread>
#include <iostream>
#include <cstring>
#include <sstream>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <fstream>


void handleClient(int clientSocket);

