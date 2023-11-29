#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char *argv[]) {
  int pipe1[2], pipe2[2];
  int pid1, pid2;

  if(argc != 2) {
    fprintf(stderr, "Usage: %s <argument>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if(pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if((pid1 = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(pid1 == 0) {
    close(pipe1[0]);
    dup2(pipe1[1], STDOUT_FILENO);
    close(pipe1[1]);
    execlp("cat", "cat", "scores", NULL);
    perror("execlp cat");
    exit(EXIT_FAILURE);
  }
  if((pid2 = fork()) == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(pid2 == 0){
    close(pipe1[1]);
    dup2(pipe1[0], STDIN_FILENO);
    close(pipe1[0]);

    close(pipe2[0]);
    dup2(pipe2[1], STDOUT_FILENO);
    close(pipe2[1]);

    execlp("grep", "grep", argv[1], NULL);
    perror("execlp grep");
    exit(EXIT_FAILURE);
  }

  close(pipe1[0]);
  close(pipe1[1]);

  if((pid1 = fork()) == -1)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if(pid1 == 0) {
      close(pipe2[1]);
      dup2(pipe2[0], STDIN_FILENO);
      close(pipe2[0]);
      execlp("sort", "sort", NULL);
      perror("execlp sort");
      exit(EXIT_FAILURE);
    }
    return 0;
}