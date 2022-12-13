#include "Socket.h"

using namespace pr;

void Socket::connect(const std::string & host, int port) {
    in_addr ipv4;
    inet_aton(host.c_str(), &ipv4);
    connect(ipv4, port);
}

void Socket::connect(in_addr ipv4, int port) {
    if (fd == -1) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
    }

    struct sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = ipv4
    };
    ::connect(fd, (struct sockaddr*) &sa, sizeof(sa));
}

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr) {
	char host[256];
	char serv[256];
	getnameinfo((struct sockaddr*) addr, sizeof(addr), 
		host, sizeof(host), 
		serv, sizeof(serv), 0);
	
	os << "IP: " << inet_ntoa(addr->sin_addr);
	os << " Host: " << host;
	os << " Port: " << ntohs(addr->sin_port);
	os << " (" << serv << ")" << std::endl;
    return os;
}