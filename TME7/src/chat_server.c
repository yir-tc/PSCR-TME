#include "chat_common.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s <ident>\n", argv[1]);
        return -1;
    }

    int fd = shm_open(argv[1], O_RDWR | O_CREAT, 0600);
    ftruncate(fd, TAILLE_MESS);
    
}