#include <iostream>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int n = 0;
    while (n < argc && strncmp(argv[n], "|", 1) != 0) {
        n++;
    }
    if (n == argc) {
        std::cout << "no pipe in command line arguments" << std::endl;
        return -1;
    }

    char **first = new char*[n];
    for (int i = 0; i < n; i++)
        first[i] = argv[i + 1];
    first[n - 1] = NULL;

    char **second = argv + n + 1;

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        perror("pipe");
        return -1;
    }

    pid_t pid1 = fork();
    if (pid1 != 0) {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        if (execv(first[0], first) < 0) 
            perror("execv");
        return 0;
    } else {
        pid_t pid2 = fork();
        if (pid2 != 0) {
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[1]);
            if (execv(second[0], second) < 0)
                perror("execv");
            return 0;
        }
    }
        
    close(pipefd[0]);
    close(pipefd[1]);
    
    delete first;
    return 0;
}