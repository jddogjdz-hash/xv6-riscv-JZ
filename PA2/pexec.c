#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
#define MAXARGS 16

int
main(int argc, char *argv[])
{
    int i, rc;
    char *newargv[MAXARGS];
    int priority;

    if (argc < 3) {
        printf("Usage: pexec <priority> <command> [args...]\n");
        exit(-1);
    }

    priority = atoi(argv[1]);
    if (setpriority(priority) < 0) {
        printf("pexec: priority must be between 0 and 49\n");
        exit(-1);
    }
    if (argc - 2 >= MAXARGS) {
        printf("pexec: too many command arguments\n");
        exit(-1);
    }
    for(i=2; i<argc; i++)
        newargv[i-2] = argv[i];
    newargv[argc-2] = 0;

    rc = fork();
    if (rc == 0)
        exec(newargv[0], newargv);
    wait(0);
    exit(0);
}
