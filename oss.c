#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <getopt.h>

void print_help() {
    printf("Usage: oss [-h] [-n proc] [-s simul] [-t iter]\n");
    printf("  -h: Display this help message\n");
    printf("  -n proc: Number of total children to launch\n");
    printf("  -s simul: Number of children allowed to run simultaneously\n");
    printf("  -t iter: Number of iterations for each worker\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int total_children = 0;
    int max_simultaneous = 0;
    int iterations = 0;

    while ((opt = getopt(argc, argv, "hn:s:t:")) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'n':
                total_children = atoi(optarg);
                break;
            case 's':
                max_simultaneous = atoi(optarg);
                break;
            case 't':
                iterations = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Invalid option: -%c\n", opt);
                print_help();
                return 1;
        }
    }

    if (total_children <= 0 || max_simultaneous <= 0 || iterations <= 0) {
        fprintf(stderr, "Invalid parameters. Please provide valid values for -n, -s, and -t.\n");
        print_help();
        return 1;
    }

    // Keep track of the number of children currently running.
    int running_children = 0;

    // Start the worker processes.
    for (int i = 0; i < total_children; i++) {
        if (running_children >= max_simultaneous) {
            // Wait for a child to finish before launching another.
            wait(NULL);
            running_children--;
        }

        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            exit(1);
        }

        if (child_pid == 0) {
            // This code runs in the child process.
            char iter_str[32];
            snprintf(iter_str, sizeof(iter_str), "%d", iterations);

            char *worker_args[] = {"./worker", iter_str, NULL};
            execvp(worker_args[0], worker_args);

            // If exec fails, print an error and exit.
            perror("execvp");
            exit(1);
        } else {
            // This code runs in the parent process.
            running_children++;
        }
    }

    // Wait for any remaining children to finish.
    while (running_children > 0) {
        wait(NULL);
        running_children--;
    }

    printf("oss has finished.\n");

    return 0;
}






































    








































