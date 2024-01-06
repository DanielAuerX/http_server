#include "http_tcpserver.h"

#include <csignal>

http::TcpServer* serverInstance = nullptr;

void signalHandler(int signal)
{
    if (serverInstance)
    {
        delete serverInstance;
        serverInstance = nullptr;
    }
    exit(0);
}

int main()
{
    using namespace http;
    
    TcpServer server = TcpServer("0.0.0.0", 8080);
    serverInstance = &server;

    signal(SIGINT, signalHandler);
    
    server.startListen();
    
    return 0;
}