#include "ServerSocket.h"

using namespace pr;

ServerSocket::ServerSocket(int port) {
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
    };
    sa.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketfd, (struct sockaddr*) &sa, sizeof(sa)) < 0) {
        socketfd = -1;
        return;
    }
    listen(socketfd, 10);
}

Socket ServerSocket::accept() {
    struct sockaddr_in sa;
    socklen_t sz = sizeof(sa);
    memset(&sa, 0, sz);

    int fd = ::accept(socketfd, (struct sockaddr*) &sa, &sz);
    
    Socket s(fd);
    return s;
}
