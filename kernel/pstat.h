#ifndef PSTAT_H
#define PSTAT_H

#include "types.h"

enum procstate { UNUSED, USED, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

struct pstat {
  // PA2 Task 1: user-visible process snapshot filled by getprocs().
  int pid;
  enum procstate state;
  uint64 size;
  int ppid;
  char name[16];
  // PA2 Task 2: base scheduling priority.
  int priority;
  // PA2 Task 3: tick when this process most recently entered the ready queue.
  uint readytime;
};

struct rusage {
  uint cputime;
};

#endif