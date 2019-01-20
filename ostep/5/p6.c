#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

// parent 1(pid:22498)
// parent 2 (pid:22498)
// parent 1(pid:22498)
// child 1 (pid:22499)

int main(int argc, char *argv[]) {
  close(STDOUT_FILENO);
  open("p6.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
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
  return 0;
}
