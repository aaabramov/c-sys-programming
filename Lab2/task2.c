#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/wait.h>

#define FORKED_CHILD 0
#define INTERVAL_SEC 3
#define ECHO_COUNT 15

FILE *logger;


void open_log_file(const char *filename) {
    logger = fopen(filename, "w+");
}

void open_default_log_file() {
    open_log_file("log.log");
}

void write_to_log(const char *msg) {
    fprintf(logger, msg);
}

void close_log() {
    fclose(logger);
}

void child_action(void) {
    int iteration = 0;

    printf("Child process");
    daemon(/* chdir to root */ 1, /* redirect to /dev/null */ 1);

    for (; iteration < ECHO_COUNT; ++iteration) {
        printf("After sleep. Iteration #%d", iteration);
        write_to_log(sprintf("Child process: iteration #%d", iteration));
        sleep(INTERVAL_SEC);
    }

    exit(EXIT_SUCCESS);
}

void parent_action(void) {
    printf("Parent process");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {

    __pid_t forked_id = fork();

    open_default_log_file();

    if (forked_id == FORKED_CHILD) {
        write_to_log("forked child process");
        child_action();
        close_log();
    } else {
        write_to_log("forked parent process");
        parent_action();
    }

    return EXIT_SUCCESS;
}

