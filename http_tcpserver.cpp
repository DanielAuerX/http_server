#include "http_tcpserver.h"
#include "pages.h"

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <stdexcept>

namespace
{
    const int BUFFER_SIZE = 30720;

    void log(const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        time_t nowTimeT = std::chrono::system_clock::to_time_t(now);
        std::ostringstream timeStream;
        timeStream << std::put_time(std::localtime(&nowTimeT), "%Y-%m-%d %X");
        std::cout << "[" << timeStream.str() << "] " << message << std::endl;
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
                                                            mSocketAddress(), mSocketAddressLen(sizeof(mSocketAddress))
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
    }

    void TcpServer::startListen()
    {
        if (listen(mSocket, 20) < 0)
        {
            exitWithError("Socket listen failed");
        }
        std::ostringstream ss;
        ss << "*** Listening on ADDRESS: "
           << inet_ntoa(mSocketAddress.sin_addr)
           << " PORT: " << ntohs(mSocketAddress.sin_port)
           << " ***";
        log(ss.str());

        int bytesReceived;

        while (true)
        {
            log("====== Waiting for a new connection ======");
            acceptConnection(mNewSocket);

            // reading the request
            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = read(mNewSocket, buffer, BUFFER_SIZE);
            if (bytesReceived < 0)
            {
                exitWithError("Failed to read bytes from client socket connection");
            }
            std::ostringstream ss;
            ss << "------ Received Request from client ------";
            log(ss.str());

            logRequest(buffer);

            std::string request = extractRequest(buffer);

            std::string serverMessage = handleRequest(request);

            sendResponse(serverMessage);

            close(mNewSocket);
        }
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

    void TcpServer::sendResponse(const std::string &serverMessage)
    {
        long bytesSent;

        bytesSent = write(mNewSocket, serverMessage.c_str(), serverMessage.size());

        if (bytesSent == serverMessage.size())
        {
            log("------ Server Response sent to client ------");
        }
        else
        {
            log("Error sending response to client");
        }
    }
    void TcpServer::logRequest(const char *buffer)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        getpeername(mNewSocket, (sockaddr *)&clientAddr, &clientAddrLen);
        std::string clientIP = inet_ntoa(clientAddr.sin_addr);

        // log the raw buffer content
        std::ostringstream logStreamRaw;
        logStreamRaw << "Request from: " << clientIP << "\n"
                     << buffer;
        log(logStreamRaw.str());
    }

    std::string TcpServer::extractRequest(const char *buffer)
    {
        const char *getStart = strstr(buffer, "GET ");
        if (getStart != nullptr)
        {
            const char *requestStart = strstr(getStart + 4, " HTTP/1.1");
            if (requestStart != nullptr)
            {
                size_t length = requestStart - (getStart + 4); // length of request part
                return std::string(getStart + 4, length);
            }
        }
        return std::string();
    }

    std::string TcpServer::handleRequest(const std::string &request)
    {
        html::PageWizard pageWizard;

        if (request == "/")
        {
            return pageWizard.getHomepage();
        }
        if (request == "/dog")
        {
            return pageWizard.getDogPage();
        }
        if (request == "/cute_doggy.jpg")
        {
            return pageWizard.getImage("cute_doggy.jpg");
        }
        if (request == "/styles.css")
        {
            return pageWizard.getCss();
        }
        return pageWizard.get404Page();
    }
}