#include "http_tcpserver.h"

#include <csignal>

http::TcpServer* serverInstance = nullptr;

void signalHandler(int signal) {
    if (serverInstance) {
        delete serverInstance;
        serverInstance = nullptr;
    }
}

int main()
{
    http::TcpServer server = http::TcpServer("0.0.0.0", 8080);
    serverInstance = &server;

    signal(SIGINT, signalHandler);

    server.startListen();

    return 0;
}