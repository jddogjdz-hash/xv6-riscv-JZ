#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

// PA2 Tasks 1-2: display active processes and their priority using getprocs().
int
main(int argc, char **argv)
{
  struct pstat uproc[NPROC];
  int nprocs;
  int i;
  char *state;
  uint now;
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

  now = uptime();
  // PA2 Task 3: age is meaningful only while a process waits in the ready queue.
  printf("pid\tstate\t\tsize\tage\tpriority\tppid\tname\n");
  for (i = 0; i < nprocs; i++) {
    state = states[uproc[i].state];
    if (uproc[i].state == RUNNABLE) {
      printf("%d\t%s\t%lu\t%u\t%d\t\t%d\t%s\n",
             uproc[i].pid, state, uproc[i].size,
             now - uproc[i].readytime, uproc[i].priority,
             uproc[i].ppid, uproc[i].name);
    } else {
      printf("%d\t%s\t%lu\t\t%d\t\t%d\t%s\n",
             uproc[i].pid, state, uproc[i].size, uproc[i].priority,
             uproc[i].ppid, uproc[i].name);
    }
  }

  exit(0);
}
