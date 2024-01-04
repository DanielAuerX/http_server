#pragma once

#include <sys/socket.h>

namespace http {
    class TcpServer {
        public:
        TcpServer();
        ~TcpServer();
        //void startListen();
        private:
        int mPort;
        int mSocket;
        int mNewSocket;
        long mIncomingMessage;

        int startServer();
        void closeServer();
    };
}