cmake_minimum_required(VERSION 3.22.1)

project(http_server)

add_executable(${PROJECT_NAME} src/server.cpp
  src/http_tcpserver.cpp
  src/pages.cpp
  src/utils.cpp
  src/cache.cpp)
