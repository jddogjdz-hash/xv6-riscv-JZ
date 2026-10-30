#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

// PA2 Tasks 1-2 and PA3 Task 3: display process details and ready-queue age.
int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  uint now;
  char *state;
  static char *states[] = {
    [USED]      "used    ",
    [SLEEPING]  "sleeping",
    [RUNNABLE]  "runnable",
    [RUNNING]   "running ",
    [ZOMBIE]    "zombie  "
  };

  nprocs = getprocs(uproc);
  if (nprocs < 0)
    exit(-1);

  // PA3 Task 3: calculate age from readytime only for runnable processes.
  now = uptime();
  printf("pid\tstate\t\tsize\tage\tpriority\tppid\tname\n");
  for (i = 0; i < nprocs; i++) {
    state = states[uproc[i].state];
    printf("%d\t%s\t%lu\t", uproc[i].pid, state, uproc[i].size);
    if (uproc[i].state == RUNNABLE)
      printf("%u", now - uproc[i].readytime);
    else
      printf("-");
    printf("\t%d\t\t%d\t%s\n",
           uproc[i].priority, uproc[i].ppid, uproc[i].name);
  }

  exit(0);
}
