cmake_minimum_required(VERSION 3.10)
project(https_server)
set(CMAKE_CXX_STANDARD 17)
add_executable(server 
                src/main.cpp
                src/Server.cpp
                src/FileHandler.cpp
                src/MIME_type.cpp
                src/HandleClient.cpp)
