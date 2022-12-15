#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "Socket.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " ip port" << std::endl;
        return 0;
    }

    std::string ip(argv[1]);
    int port = std::stoi(argv[2]);

    pr::Socket s;
    s.connect(ip, port);
    int fd = s.getFD();

    while (true) {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);

        std::string command = message.substr(0, message.find(" "));
        if (command == "LIST") {
            std::cout << message << std::endl;
            write(fd, message.c_str(), message.size());
            unsigned char c;
            while (read(fd, &c, 1) != 0) {
                if (c == 0xff) break;
                std::cout << c;
            }
            std::cout << std::endl;
        } else if (command == "DOWNLOAD") {
            write(fd, message.c_str(), message.size());
            message.erase(0, message.find(" ") + 1);

            unsigned int size = 0;
            read(fd, &size, 4);

            std::cout << "size = " << size << " bytes" << std::endl;

            int ffd = open(message.c_str(), O_CREAT | O_WRONLY);

            char c; unsigned int n = 1;
            while (read(fd, &c, 1) != 0 && n < size) {
                write(ffd, &c, 1);
                n++;
            }
            close(ffd);
        } else if (command == "UPLOAD") {
            write(fd, message.c_str(), message.size());
            message.erase(0, message.find(" ") + 1);

            int ffd = open(message.c_str(), O_RDONLY);
            if (ffd == -1) {
                std::cerr << "ERR: file not found\n" << std::endl;
            } else {
                off_t size = lseek(ffd, 0, SEEK_END);
                write(fd, &size, 4);
                lseek(ffd, 0, SEEK_SET);
                std::cout << "uploading " << size << " bytes" << std::endl;

                char c;
                off_t n = 0;
                while (read(ffd, &c, 1) != 0 && n < size) {
                    write(fd, &c, 1);
                    n++;
                }
            }

            close(ffd);
        } else {
            std::cout << "unknown command" << std::endl;
        }
    }
    return 0;
}