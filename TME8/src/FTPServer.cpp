#include "TCPServer.h"
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

class SimpleFTPServer : public pr::ConnectionHandler {
private:
    char* path;

public :
    SimpleFTPServer(char* path): ConnectionHandler(), path(path) {}

	void handleConnection (pr::Socket sc) {
		int fd = sc.getFD();
		char raw_message[1024];
        std::string message;

		while (1) {
			memset(raw_message, 0, 1024);
			auto nblu = read(fd, raw_message, 1024);
			if (nblu == 0) {
				break;
			}

            raw_message[strcspn(raw_message, "\n")] = 0;
            message = raw_message;
			std::cout << "Received: " << message << std::endl;

            std::string command = message.substr(0, message.find(" "));
            message.erase(0, message.find(" ") + 1);

            if (command == "LIST") {
                DIR *dp;
                struct dirent *dirp;

                dp = opendir(".");
                while ((dirp = readdir(dp)) != NULL) {
                    write(fd, dirp->d_name, strlen(dirp->d_name));
                    write(fd, "\n", 1);
                }
                unsigned char c = 0xff;
                write(fd, &c, 1);

                closedir(dp);
            } else if (command == "UPLOAD") {
                std::string arg = message;
                std::cout << "uploading " << arg << " ..." << std::endl;
                
                unsigned int size = 0;
                read(fd, &size, 4);

                std::cout << "size = " << size << " bytes" << std::endl;

                int ffd = open(arg.c_str(), O_CREAT | O_WRONLY);

                char c; unsigned int n = 1;
                while ((nblu = read(fd, &c, 1)) != 0 && n < size) {
                    write(ffd, &c, 1);
                    n++;
                }
                close(ffd);
            } else if (command == "DOWNLOAD") {
                std::string arg = message;
                std::cout << "downloading " << arg << " ..." << std::endl;

                int ffd = open(arg.c_str(), O_RDONLY);

                off_t size = lseek(ffd, 0, SEEK_END);
                write(fd, &size, 4);
                lseek(ffd, 0, SEEK_SET);

                if (ffd == -1) {
                    write(fd, "ERR: file not found\n", 20);
                } else {
                    char c;
                    while ((nblu = read(ffd, &c, 1)) != 0) {
                        write(fd, &c, 1);
                    }
                    write(fd, "\n", 1);
                }

                close(ffd);
            } else {
                std::cout << "unknown command" << std::endl;
            }
		}
	}
	ConnectionHandler * clone() const {
		return new SimpleFTPServer(path);
	}
};

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " [port] [dir]" << std::endl;
        return 0;
    }

    int port = atoi(argv[1]);

	pr::TCPServer server(new SimpleFTPServer(argv[2]));
    std::cout << "listening on port " << port << std::endl;

	server.startServer(port);
	server.stopServer();

	return 0;
}

