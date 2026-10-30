#include "kernel/types.h"
#include "user/user.h"

#define JOB_COUNT 4

// PA2 Task 5: compare response and turnaround for competing priorities to
// check that waiting-time aging eventually schedules both.
struct timing {
  uint priority;
  uint response;
  uint turnaround;
};

static void
run_job(int result_fd, uint launch, uint priority, uint duration)
{
  struct timing result;
  uint start = uptime();

  while (uptime() - start < duration)
    ;

  result.priority = priority;
  // PA2 Task 5: response is launch-to-first-run; turnaround is launch-to-exit.
  result.response = start - launch;
  result.turnaround = uptime() - launch;
  if (write(result_fd, &result, sizeof(result)) != sizeof(result))
    exit(1);
  exit(0);
}

int
main(void)
{
  int result_pipe[2];
  int status;
  int i;
  uint launch;
  uint response_sum = 0;
  uint turnaround_sum = 0;
  uint low_response = 0;
  uint high_turnaround = 0;
  uint priorities[JOB_COUNT] = {40, 25, 10, 0};
  uint durations[JOB_COUNT] = {80, 30, 20, 8};
  struct timing results[JOB_COUNT];

  if (pipe(result_pipe) < 0) {
    printf("agingtest: pipe failed\n");
    exit(1);
  }

  launch = uptime();
  for (i = 0; i < JOB_COUNT; i++) {
    if (setpriority(priorities[i]) < 0) {
      printf("agingtest: setpriority failed\n");
      exit(1);
    }
    int pid = fork();
    if (pid < 0) {
      printf("agingtest: fork failed for priority %u\n", priorities[i]);
      exit(1);
    }
    if (pid == 0) {
      close(result_pipe[0]);
      run_job(result_pipe[1], launch, priorities[i], durations[i]);
    }
  }

  close(result_pipe[1]);
  for (i = 0; i < JOB_COUNT; i++) {
    if (wait2(&status, 0) < 0 || status != 0) {
      printf("agingtest: worker failed\n");
      exit(1);
    }
  }

  for (i = 0; i < JOB_COUNT; i++) {
    if (read(result_pipe[0], &results[i], sizeof(results[i])) !=
        sizeof(results[i])) {
      printf("agingtest: could not read worker timings\n");
      exit(1);
    }
    response_sum += results[i].response;
    turnaround_sum += results[i].turnaround;
    if (results[i].priority == 0)
      low_response = results[i].response;
    else if (results[i].priority == 40)
      high_turnaround = results[i].turnaround;
    printf("priority %u: response %u ticks, turnaround %u ticks\n",
           results[i].priority, results[i].response,
           results[i].turnaround);
  }
  close(result_pipe[0]);

  printf("average response: %u ticks\n", response_sum / JOB_COUNT);
  printf("average turnaround: %u ticks\n", turnaround_sum / JOB_COUNT);
  if (low_response >= high_turnaround) {
    printf("agingtest: low-priority job was not scheduled before the "
           "high-priority job finished\n");
    exit(1);
  }
  printf("agingtest: aging let the low-priority job run before the "
         "high-priority job finished\n");
  exit(0);
}
