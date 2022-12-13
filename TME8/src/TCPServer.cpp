#include "TCPServer.h"

using namespace pr;

bool TCPServer::startServer (int port) {
    ss = new ServerSocket(port);
    while (ss->getFD()) {
        Socket s = ss->accept();
        handler->handleConnection(s);
    }
    return true;
}

void TCPServer::stopServer () {
    ss->close();
}
