#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#include "rsleep.h"

int lives = 3;
std::string name = "???";

void hit(int signum) {
    lives--;
    if (lives == 0) {
        std::cout << name << ": I die..." << std::endl;
        exit(1);
    }
    std::cout << name << ": " << lives << " LP left" << std::endl;
}

void attaque(pid_t adversaire) {
    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_handler = hit;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, nullptr);
    
    std::cout << "pid adv: " << adversaire << std::endl;
    if (kill(adversaire, SIGINT) == -1) {
        std::cout << name << ": I won !" << std::endl;
        exit(0);
    }

    randsleep();
}

void parry(int signum) {
    std::cout << "Luke: parry" << std::endl;
}

void defense() {
    struct sigaction act;
    sigfillset(&act.sa_mask);
    act.sa_flags = 0;

    if (name == "Luke") {
        act.sa_handler = parry;
    } else {
        act.sa_handler = SIG_IGN;
    }

    sigaction(SIGINT, &act, nullptr);
    randsleep();
}

void combat(pid_t adversaire) {
    while (true) {
        defense();
        attaque(adversaire);

        if (name == "Vador") {
            int status = 0;
            /*if (waitpid(adversaire, &status, WNOHANG) == adversaire
            && WIFEXITED(status)) 
                exit(0);
            std::cout << "status: " << WEXITSTATUS(status) << std::endl;
        */}
    }
}

int wait_till_pid(pid_t pid) {
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t fpid = fork();
    if (fpid == 0) {
        name = "Luke";
        srand(time(NULL));
        combat(getppid());
    } else {
        name = "Vador";
        srand(time(NULL));
        combat(fpid);
    }
}