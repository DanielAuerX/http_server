#include "http_tcpserver.h"

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace
{
    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string &errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}

namespace http
{
    TcpServer::TcpServer()
    {
    }
    TcpServer::~TcpServer()
    {
    }

    int TcpServer::startServer()
    {
        mSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (mSocket < 0)
        {
            exitWithError("Cannot create a socket");
            return 1;
        }
        return 0;
    }

    void TcpServer::closeServer(){
        close(mSocket);
        close(mNewSocket);
        exit(0);
    }
}