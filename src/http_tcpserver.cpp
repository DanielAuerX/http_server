#include "../include/http_tcpserver.h"
#include "../include/pages.h"

#include <sstream>
#include <unistd.h>
#include <cstring>

namespace http
{
    const int BUFFER_SIZE = 30720;

    TcpServer::TcpServer(std::string ipAddress, int port) : mIpAddress(ipAddress), mPort(port), mSocket(), mNewSocket(),
                                                            mIncomingMessage(),
                                                            mSocketAddress(),
                                                            mSocketAddressLen(sizeof(mSocketAddress)),
                                                            mCache(memory::Cache()),
                                                            mToolbox(utils::SwissArmyToolbox())
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
        mToolbox.log("Starting server...");
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
        mToolbox.log("Closing server...");
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
        ss << "*** Listening on ADDRESS: "
           << inet_ntoa(mSocketAddress.sin_addr)
           << " PORT: " << ntohs(mSocketAddress.sin_port)
           << " ***";
        mToolbox.log(ss.str());

        int bytesReceived;

        while (true)
        {
            mToolbox.log("====== Waiting for a new connection ======");
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
            mToolbox.log(ss.str());

            logRequest(buffer);

            std::string request = extractRequest(buffer);
            std::string cookie = extractCookie(buffer);

            std::string serverMessage = handleRequest(request, cookie);

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
            mToolbox.log("------ Server Response sent to client ------");
        }
        else
        {
            mToolbox.log("Error sending response to client");
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
        mToolbox.log(logStreamRaw.str());
    }

    void TcpServer::exitWithError(const std::string &errorMessage)
    {
        mToolbox.log("ERROR: " + errorMessage);
        exit(1);
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

    std::string TcpServer::extractCookie(const char *buffer)
    {
        const char *prefix = "Cookie: user=";
        const char *getStart = strstr(buffer, prefix);
        if (getStart != nullptr)
        {
            getStart += strlen(prefix);
            const char *endPtr = strchr(getStart, '\n');
            if (endPtr != nullptr)
            {
                size_t length = endPtr - getStart;
                return std::string(getStart, length);
            }
        }
        return std::string();
    }

    std::string TcpServer::handleRequest(const std::string &request, const std::string &cookie)
    {
        html::PageWizard pageWizard;

        if (mCache.contains(request) && cookie != "")
        {
            return mCache.retrieve(request);
        }

        if (request.find("/?user=") != std::string::npos)
        {
            std::string cookie = request.substr(7);
            return pageWizard.getHomepageWithCookie(cookie);
        }
        if (request == "/styles.css")
        {
            const std::string response = pageWizard.getCss();
            mCache.add(request, response);
            return response;
        }
        if (cookie == "")
        {
            const std::string response = pageWizard.getIndex();
            mCache.add(request, response);
            return response;
        }
        if (request == "/")
        {
            return pageWizard.getHomepage(cookie);
        }
        if (request == "/dog")
        {
            const std::string response = pageWizard.getDogPage();
            mCache.add(request, response);
            return response;
        }
        if (request == "/cute_doggy.jpg")
        {
            const std::string response = pageWizard.getImage("cute_doggy.jpg");
            mCache.add(request, response);
            return response;
        }
        const std::string response = pageWizard.get404Page();
        mCache.add(request, response);
        return response;
    }

}