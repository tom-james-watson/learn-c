#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    close(STDOUT_FILENO); // no open file descriptors - output lost
    printf("hello\n");
  } else { // parent goes down this path (main)
    wait(NULL);
    printf("goodbye\n");
  }
  return 0;
}
