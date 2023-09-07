#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    int iterations = atoi(argv[1]);

    if (iterations <= 0) {
        fprintf(stderr, "Please provide a positive number of iterations.\n");
        return 1;
    }

    pid_t parent_pid = getppid(); // Get the parent process ID.

    for (int i = 1; i <= iterations; i++) {
        printf("WORKER PID:%d PPID:%d Iteration:%d before sleeping\n", getpid(), parent_pid, i);
        sleep(1);
        printf("WORKER PID:%d PPID:%d Iteration:%d after sleeping\n", getpid(), parent_pid, i);
    }

    return 0;
}






































































