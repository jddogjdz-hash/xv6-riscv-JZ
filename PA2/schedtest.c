#include "kernel/types.h"
#include "user/user.h"

// PA2 Task 4: compete CPU-bound children to demonstrate priority selection.
static void
run_job(char *name)
{
  uint start = uptime();

  printf("schedtest: %s started\n", name);
  while (uptime() - start < 3)
    ;
  printf("schedtest: %s finished\n", name);
  exit(0);
}

int
main(void)
{
  int high_pid;
  int low_pid;

  if (setpriority(40) < 0) {
    printf("schedtest: could not set high priority\n");
    exit(1);
  }
  high_pid = fork();
  if (high_pid < 0) {
    printf("schedtest: high-priority fork failed\n");
    exit(1);
  }
  if (high_pid == 0)
    run_job("priority-40");

  if (setpriority(0) < 0) {
    printf("schedtest: could not set low priority\n");
    exit(1);
  }
  low_pid = fork();
  if (low_pid < 0) {
    printf("schedtest: low-priority fork failed\n");
    exit(1);
  }
  if (low_pid == 0)
    run_job("priority-0");

  if (wait(0) < 0 || wait(0) < 0) {
    printf("schedtest: wait failed\n");
    exit(1);
  }
  printf("schedtest: both jobs finished\n");
  exit(0);
}
