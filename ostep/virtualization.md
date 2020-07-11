# Processes

## Process Control Block

The PCB, also known as a process descriptor, is a structure used to store all state necessary about a process in order to be able to swap it in and out of execution, i.e. to perform a context switch.

#### Typical Process Structure (xv6 kernel)

```c
// the registers xv6 will save and restore
// to stop and subsequently restart a process
struct context {
  int eip;
  int esp;
  int ebx;
  int ecx;
  int edx;
  int esi;
  int edi;
  int ebp;
};

// the different states a process can be in
enum proc_state { UNUSED, EMBRYO, SLEEPING,
                  RUNNABLE, RUNNING, ZOMBIE };

// the information xv6 tracks about each process
// including its register context and state
struct proc {
  char *mem;                  // Start of process memory
  uint sz;                    // Size of process memory
  char *kstack;               // Bottom of kernel stack
                              // for this process
  enum proc_state state;      // Process state
  int pid;                    // Process ID
  struct proc *parent;        // Parent process
  void *chan;                 // If non-zero, sleeping on chan
  int killed;                 // If non-zero, have been killed
  struct file *ofile[NOFILE]; // Open files
  struct inode *cwd;          // Current directory
  struct context context;     // Switch here to run process
  struct trapframe *tf;       // Trap frame for the
                              // current interrupt
};
```

## Process API

#### `fork()`

Create a new process by duplicating the calling process. The new process is referred to as the _child process_ and the calling process the _parent process_.

```c
  printf("parent 1(pid:%d)\n", (int) getpid());
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    printf("child 1 (pid:%d)\n", (int) getpid());
  } else { // parent goes down this path (main)
    printf("parent 2 (pid:%d)\n", (int) getpid());
  }
```

#### `wait()`

The wait system-call puts the process to sleep and waits for a child process to end. It then fills in the argument with the exit code of the child-process (if the argument is not NULL).

```c
  int status;
  if (wait(&status) >= 0)
  {
    if (WEXITED(status))
    {
      /* Child process exited normally, through `return` or `exit` */
      printf("Child process exited with %d status\n", WEXITSTATUS(status));
    }
  }
```

#### `exec()`

The `exec()` family of system calls allows a child to break free from its similarity to its parent and execute an entirely new program.

```c
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    printf("hello, I am child (pid:%d)\n", (int) getpid());
    execlp("ls", "ls", "-l", (char *) NULL);
    printf("this shouldn't print out");
  } else { // parent goes down this path (main)
    int rc_wait = wait(NULL);
    printf("hello, I am parent of %d (rc_wait:%d) (pid:%d)\n", rc, rc_wait, (int) getpid());
  }
```

Functions in the `exec()` family have different behaviours:

`l` : arguments are passed as a list of strings to the `main()`
`v` : arguments are passed as an array of strings to the `main()`
`p` : path/s to search for the new running program
`e` : the environment can be specified by the caller

You can mix them, therefore you have:

```c
  int execl(const char *path, const char *arg, ...);
  int execlp(const char *file, const char *arg, ...);
  int execle(const char *path, const char *arg, ..., char * const envp[]);
  int execv(const char *path, char *const argv[]);
  int execvp(const char *file, char *const argv[]);
  int execvpe(const char *file, char *const argv[], char *const envp[]);
```

## Process Control

Signals can be sent to processes to control their flow. These include `SIGINT` (interrupt) and `SIGSTOP` (suspend).

## System Calls

Processes can run in one of two modes of execution - *user mode* or *kernel mode*.

Normal processes run in user mode. In order to perform certain restricted actions, such as allocating memory, a process needs to perform a _system call_. To execute a system call, a user process must execute a special *trap* instruction. This instruction simultaneously jumps into the kernel and raises the privilege level to kernel mode. Once in kernel mode, the OS can perform whatever priveliged operations are needed and, when done, call a special *return-from-trap* instruction to return into the calling program while simultaneously reducing the privilege level back to user mode.

#### How does the trap know which code to run?

The kernel sets up a *trap table* at boot time, saving the location of what code to run for each type of interrupt and system call.

## Context Switch

The OS needs to save a few register values for the currently-executing process and restore a few for the soon-to-be-executing process. By doing so, the OS thus ensures that when the return-from-trap instruction is finally executed, instead of returning to the process that was running, the system resumes execution of another process.

To save the context of the currently-running process, the OS will execute some low-level assembly code to save the general purpose registers, PC, and the kernel stack pointer of the currently-running process, and then restore said registers, PC, and switch to the kernel stack for the soon-to-be-executing process

## Switching Between Processes

Either:

#### Wait for system calls

At which point you can perform a context switch during the return-from-trap.

#### Timer Interrupt

A timer device can be programmed to raise an interrupt every so many milliseconds. When the interrupt handler runs, the OS regains control and can context switch.

## Scheduling

#### Multi Level Feedback Queue

This scheduler has multiple distinct queues, each with its own associated priority level. The basic rules for this scheduler are as follows:

* if Priority(A) > Priority(B), A runs, B doesn't
* if Priority(A) == Priority(B), A & B run in Round Robin

The key to the MLFQ is how it assigns priority. Processes that relinquish CPU control for I/O often will be given a high priority to ensure responsiveness. Long running CPU intensive tasks will be assigned low priority.  Processes don't have a fixed priority - they vary over time as the scheduler learns about them.

The basic version of how that works is that if a process executes a whole CPU time slice without giving up the processor, it will be moved down a priority level. This approximates SJF (Shortest Job First), as processes wil lbe assumed to be short jobs until they prove themselves not to be as they are moved down through the queues over time.

To prevent processes in lower queues from becoming CPU starved, for example if there are many high priority processes using CPU, we introduce the concept of priority boosting - after some time period S, move all jobs in the system to the highest priority. This also accommodates for a process switching from a CPU-bound to an interactive job.

It would be possible for a process to game the scheduler by performing I/O just before its CPU time slice ends, thus staying at the same priority forever. To prevent this, we instead can keep track of how much CPU time a process has used at a given queue level over time, regardless of I/O, and once used up lower the process a level.

This type of scheduler is the base scheduler used on BSD derivatives, Solaris and Windows.
