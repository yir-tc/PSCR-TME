#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"
#include <unistd.h>
#include <cstring>

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port);

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept();

	void close() { ::close(socketfd); socketfd = -1; };
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */
