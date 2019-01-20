#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int test = 1;
  printf("parent pre-fork (test:%d) (pid:%d)\n", test, (int) getpid());
  int rc = fork();
  if (rc < 0) { // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc == 0) { // child (new process)
    test = 2;
    printf("child (test:%d) (pid:%d)\n", test, (int) getpid());
  } else { // parent goes down this path (main)
    test = 3;
    printf("parent post-fork (test:%d) (pid:%d)\n", test, (int) getpid());
  }
  return 0;
}
