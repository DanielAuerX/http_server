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
    TcpServer::TcpServer(std::string ipAddress, int port) : mIpAddress(ipAddress), mPort(port), mSocket(), mNewSocket(),
                                                            mIncomingMessage(),
                                                            mSocketAddress(), mSocketAddressLen(sizeof(mSocketAddress)),
                                                            mServerMessage(buildResponse())
    {
        startServer();
    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
    {
        mSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (mSocket < 0)
        {
            exitWithError("Cannot create a socket");
            return 1;
        }
        if (bind(mSocket, (sockaddr *)&mSocketAddress, mSocketAddressLen) < 0)
        {
            exitWithError("Cannot connect socket to address");
            return 1;
        }
        return 0;
    }

    void TcpServer::closeServer()
    {
        close(mSocket);
        close(mNewSocket);
        exit(0);
    }
}