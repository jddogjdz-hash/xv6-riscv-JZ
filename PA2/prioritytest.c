#include "kernel/types.h"
#include "user/user.h"

// PA2 Task 2: check priority syscall bounds and inheritance using ps.
int
main(void)
{
  int status;
  int pid;
  char *args[] = { "ps", 0 };

  printf("prioritytest: starting priority is %d\n", getpriority());
  if (setpriority(-1) != -1 || setpriority(50) != -1 ||
      getpriority() != 0) {
    printf("prioritytest: invalid priority handling failed\n");
    exit(1);
  }
  if (setpriority(17) < 0 || getpriority() != 17) {
    printf("prioritytest: set/get priority failed\n");
    exit(1);
  }
  printf("prioritytest: priority changed to %d; launching ps\n",
         getpriority());

  pid = fork();
  if (pid < 0) {
    printf("prioritytest: fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    exec("ps", args);
    printf("prioritytest: exec ps failed\n");
    exit(1);
  }

  if (wait2(&status, 0) != pid || status != 0) {
    printf("prioritytest: ps failed\n");
    exit(1);
  }
  exit(0);
}
