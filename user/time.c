#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

// PA1: run a child command, then print elapsed time, CPU time, and CPU %.

int
main(int argc, char *argv[])
{
  int status;
  int elapsed;
  int percent;
  int start;
  int end;
  struct rusage usage;

  if (argc < 2) {
    fprintf(2, "usage: time command [args...]\n");
    exit(1);
  }

  start = uptime();
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "time: fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    exec(argv[1], &argv[1]);
    fprintf(2, "time: exec %s failed\n", argv[1]);
    exit(1);
  }

  if (wait2(&status, &usage) < 0) {
    fprintf(2, "time: wait2 failed\n");
    exit(1);
  }

  end = uptime();
  elapsed = end - start;
  percent = elapsed == 0 ? 0 : (usage.cputime * 100) / elapsed;
  printf("elapsed time: %d ticks, cpu time: %d ticks, %d%% CPU\n", elapsed,
         usage.cputime, percent);
  exit(0);
}