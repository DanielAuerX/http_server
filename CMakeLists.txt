cmake_minimum_required(VERSION 3.22.1)

project(http_server)

add_executable(${PROJECT_NAME} server.cpp http_tcpserver.cpp)
