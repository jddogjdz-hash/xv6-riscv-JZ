#include "kernel/types.h"
#include "user/user.h"

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
  int pid;
  int status;
  int i;
  uint launch;
  uint response_sum = 0;
  uint turnaround_sum = 0;
  uint low_response = 0;
  uint high_turnaround = 0;
  struct timing results[2];

  if (pipe(result_pipe) < 0) {
    printf("agingtest: pipe failed\n");
    exit(1);
  }

  launch = uptime();
  if (setpriority(40) < 0) {
    printf("agingtest: setpriority failed\n");
    exit(1);
  }
  // PA2 Task 5: keep the high-priority worker busy so the low-priority
  // worker's aging bonus can overcome the base-priority gap.
  pid = fork();
  if (pid < 0) {
    printf("agingtest: high-priority fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    close(result_pipe[0]);
    run_job(result_pipe[1], launch, 40, 80);
  }

  if (setpriority(0) < 0) {
    printf("agingtest: setpriority failed\n");
    exit(1);
  }
  pid = fork();
  if (pid < 0) {
    printf("agingtest: low-priority fork failed\n");
    exit(1);
  }
  if (pid == 0) {
    close(result_pipe[0]);
    run_job(result_pipe[1], launch, 0, 8);
  }

  close(result_pipe[1]);
  if (wait2(&status, 0) < 0 || status != 0 ||
      wait2(&status, 0) < 0 || status != 0) {
    printf("agingtest: worker failed\n");
    exit(1);
  }

  for (i = 0; i < 2; i++) {
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

  printf("average response: %u ticks\n", response_sum / 2);
  printf("average turnaround: %u ticks\n", turnaround_sum / 2);
  if (low_response >= high_turnaround) {
    printf("agingtest: low-priority job was not scheduled before the "
           "high-priority job finished\n");
    exit(1);
  }
  printf("agingtest: aging let the low-priority job run before the "
         "high-priority job finished\n");
  exit(0);
}
