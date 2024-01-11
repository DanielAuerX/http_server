#pragma once

#include "cache.h"
#include "utils.h"

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
        memory::Cache mCache;
        utils::SwissArmyToolbox mToolbox;

        int startServer();
        void closeServer();
        void acceptConnection(int &newSocket);
        void sendResponse(const std::string &serverMessage);
        void logRequest(const char *buffer);
        std::string extractRequest(const char *buffer);
        std::string extractCookie(const char *buffer);
        std::string handleRequest(const std::string &request, const std::string &cookie);
        void exitWithError(const std::string &errorMessage);

    };
}