#pragma once

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

namespace http
{
    class TcpServer
    {
    public:
        TcpServer(std::string ipAddress, int port);
        ~TcpServer();
        void startListen();

    private:
        std::string mIpAddress;
        int mPort;
        int mSocket;
        int mNewSocket;
        long mIncomingMessage;
        struct sockaddr_in mSocketAddress;
        unsigned int mSocketAddressLen;

        int startServer();
        void closeServer();
        void acceptConnection(int &newSocket);
        std::string buildResponse();
        void sendResponse(const std::string &serverMessage);
        void logRequest(const char* buffer);
        std::string extractRequest(const char* buffer);
        std::string handleRequest(const std::string &request);
    };
}