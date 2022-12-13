#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iosfwd>
#include <iostream>
#include <unistd.h>
#include <netdb.h>

namespace pr {

class Socket {
	int fd;

public :
	Socket() : fd(-1) {}
	Socket(int fd) : fd(fd) {}

	// tente de se connecter à l'hôte fourni
	void connect(const std::string & host, int port);
	void connect(in_addr ipv4, int port);

	bool isOpen() const {return fd != -1;}
	int getFD() { return fd ;}

	void close() { ::close(fd); };
};

std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr);

}

#endif /* SRC_SOCKET_H_ */
