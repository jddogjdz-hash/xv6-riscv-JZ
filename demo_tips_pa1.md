# COPILOT LLM PA1 Demo Tips
Copilot assisted with code generation in xv6

## Goal
Keep the demo short, real, and easy to explain. The TA mainly wants to see that your xv6 commands work and that you understand the idea behind them.

## Best commands to show
Use these three in order:

1. `uptime`
   - Shows the system uptime from the kernel.
   - Simple proof that the syscall works.

2. `time1 sleep 10`
   - Shows elapsed time measurement.
   - Easy to explain using start/end uptime values.

3. `time matmul`
   - Shows CPU time and CPU utilization.
   - Best demonstration of the advanced part of the assignment.
   - Specifically: shows elapsed time and cpu time when the core has a workload (similar)

## Exact demo flow
Run these commands in xv6:

```sh
$ uptime
uptime: 56
```

Say:
> This command calls the kernel uptime system call and prints the current tick count.

```sh
$ time1 sleep 10
elapsed time: 10 ticks
```

Say:
> This measures elapsed time by recording the system uptime before and after the child command runs.

```sh
$ time matmul
Time: 2 ticks
elapsed time: 3 ticks, cpu time: 3 ticks, 100% CPU
```

Say:
> This command reports both elapsed time and CPU time. Since matmul is actively using the CPU, the CPU time is close to the elapsed time and the CPU utilization is 100%.

## Key concept to explain
This is the main idea you should say clearly:

- Elapsed time = total real time from start to finish
- CPU time = only the time the process actually used the CPU

Example:
> A sleeping process can have a nonzero elapsed time but zero CPU time, because it was waiting instead of running.

## Short script you can say out loud
> I implemented the xv6 user utilities and system calls for PA1. I added uptime, which calls the kernel uptime system call and prints the current tick count. I then implemented time1, which measures elapsed time by recording the system uptime before and after a child command finishes. Finally, I added the time command, which reports elapsed time, CPU time, and CPU utilization. The key idea is that elapsed time measures total wall-clock time, while CPU time measures only the time the process actually used the CPU.

## TA Q&A
### Q: What is the difference between elapsed time and CPU time?
A: Elapsed time measures total real time, while CPU time measures only the time the process spent using the CPU.

### Q: Why is `sleep 10` useful for a demo?
A: It clearly shows that elapsed time can increase even when CPU time stays at zero.

### Q: Why do we use ticks?
A: xv6 timer interrupts advance system ticks, and those ticks are used to track uptime and CPU usage.

## Files to jump to during the demo

### User-space commands
- `user/uptime.c` — new utility that calls the kernel uptime syscall and prints the current tick count.
- `user/time1.c` — measures elapsed time by recording uptime before and after a child command runs.
- `user/time.c` — runs a target command, then prints elapsed time, CPU time, and CPU utilization.
- `user/matmul.c` — CPU-heavy workload used to verify high CPU utilization.
- `user/sleep.c` — low-CPU workload used to show that elapsed time can increase while CPU time stays low.

### Kernel timing/accounting
- `kernel/proc.h` — adds `cputime` to each process so the kernel can track CPU time.
- `kernel/proc.c` — initializes `cputime` and copies it back through `wait2()`.
- `kernel/trap.c` — increments `cputime` on timer interrupts to count CPU ticks used by a process.
- `kernel/sysproc.c` — implements `sys_wait2()` and `sys_uptime()` for user processes.
- `kernel/syscall.c` — wires the new syscall numbers to their handlers.
- `kernel/syscall.h` — assigns syscall IDs like `SYS_uptime` and `SYS_wait2`.
- `user/user.h` — declares `uptime()` and `wait2()` for user programs.
- `user/usys.pl` — generates the user-space syscall stubs.
- `Makefile` — includes the new user programs so they are built into xv6.

### What this means
The user programs in the `user` directory are what you run in the shell. The actual timing state and accounting live in the kernel process and syscall code. The user command asks the kernel for timing data, and the kernel tracks that information in each process.

## Demo tips
- Do not read the whole report.
- Show only 2-3 commands.
- Keep explanations short and simple.
- Pause after each command so the TA can read the output.
- If you get nervous, focus on the idea: timing and CPU usage are measured in ticks.

## Closing line
> The demo works because the outputs show that the kernel is correctly tracking system uptime and process timing, and that the utilities are interpreting those values correctly.
