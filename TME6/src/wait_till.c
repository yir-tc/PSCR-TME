#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int wait_till_pid(pid_t pid) {
    pid_t child_pid = -1;
    int wstatus;

    while (1) {
        child_pid = wait(&wstatus);
        if (child_pid < 0)
            return -1;

        if (child_pid == pid && WIFEXITED(wstatus))
            break;
    }

    return child_pid;
}

unsigned char wtp_alarm_finished = 0;

void wtp_alarm_handler(int sig) {
    wtp_alarm_finished = 1;
}

int wait_till_pid_2(pid_t pid, int sec) {
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = wtp_alarm_handler;
    sigaction(SIGALRM, &act, NULL);
    
    alarm(sec);

    pid_t child_pid = -1;
    int wstatus;

    while (!wtp_alarm_finished) {
        child_pid = wait(&wstatus);
        if (child_pid < 0)
            break;

        if (child_pid == pid && WIFEXITED(wstatus))
            break;        
    }
    
    alarm(0);
    act.sa_handler = SIG_DFL;
    sigaction(SIGALRM, &act, NULL);

    if (wtp_alarm_finished)
        return 0;
    return child_pid;
}

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        sleep(2);
        printf("fils\n");
    } else {
        printf("père (wait_till_pid = %d)\n", wait_till_pid_2(pid, 1));
    }
    return 0;
}