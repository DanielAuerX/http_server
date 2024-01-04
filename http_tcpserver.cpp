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
        mSocketAddress.sin_family = AF_INET;
        mSocketAddress.sin_port = htons(mPort);
        mSocketAddress.sin_addr.s_addr = inet_addr(mIpAddress.c_str());

        startServer();
    }

    TcpServer::~TcpServer()
    {
        closeServer();
    }

    int TcpServer::startServer()
    {
        log("Starting server...");
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
        log("Closing server...");
        close(mSocket);
        close(mNewSocket);
        exit(0);
    }

    void TcpServer::startListen()
    {
        if (listen(mSocket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: "
           << inet_ntoa(mSocketAddress.sin_addr)
           << " PORT: " << ntohs(mSocketAddress.sin_port)
           << " ***\n\n";
        log(ss.str());
    }
    
    
    std::string TcpServer::buildResponse()
    {
        return std::string();
    }
    void TcpServer::acceptConnection(int &newSocket)
    {
        newSocket = accept(mSocket, (sockaddr *)&mSocketAddress, &mSocketAddressLen);
        if (newSocket < 0)
        {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(mSocketAddress.sin_addr) << "; PORT: " << ntohs(mSocketAddress.sin_port);
            exitWithError(ss.str());
        }
    }
}