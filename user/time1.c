#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// PA1: run a child command and report elapsed wall-clock time in ticks.

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("usage: time1 command [args...]\n");
        exit(1);
    }

    int start = uptime();   // ticks before running child

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    if (pid == 0) {
        // child: exec the command
        exec(argv[1], &argv[1]);
        // if exec returns, it failed
        printf("exec failed\n");
        exit(1);
    }

    // parent waits
    wait(0);

    int end = uptime();     // ticks after child finishes
    int elapsed = end - start;

    printf("elapsed time: %d ticks\n", elapsed);

    exit(0);
}
