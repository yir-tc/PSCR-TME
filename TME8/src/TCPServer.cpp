#include "TCPServer.h"

using namespace pr;

bool TCPServer::startServer (int port) {
    ss = new ServerSocket(port);
    while (ss->getFD() > 0) {
        Socket s = ss->accept();
        std::thread t(&TCPServer::handleConnectionWrapper, this, s);
        t.detach();
    }
    return true;
}

void TCPServer::handleConnectionWrapper(Socket s) {
    handler->handleConnection(s);
}

void TCPServer::stopServer () {
    ss->close();
}
