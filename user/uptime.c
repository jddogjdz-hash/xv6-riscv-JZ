#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// PA1: user program that calls uptime() and prints total system runtime in ticks.

int
main(int argc, char *argv[])
{
    int ticks = uptime();
    printf("uptime: %d\n", ticks);
    exit(0);
}
