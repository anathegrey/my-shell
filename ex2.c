#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

#define MAX_SIZE 1024

char *cmd1[MAX_SIZE];

int main(int argc, char* argv[]) {
  pid_t pid;
  int i=0, fd[2];
  char *command;
  char buf[MAX_SIZE];
  /* fork a child process */
  for( ; ; ) {
    fprintf(stdout, "$ ");
    if((command = fgets(buf, sizeof(buf), stdin)) == NULL)
      break;
    command[strlen(buf) - 1] = '\0';
    if(strcmp(command, "quit") == 0)
      exit(0);
    char *token = strtok(command, " ");
    while(token != NULL) {
      cmd1[i++] = token;
      token = strtok(NULL, " ");
    }
    i=0;
    if(pipe(fd) < 0) {
      perror("Erro\n");
    }
    if ((pid= fork()) < 0) {
      printf("%s: cannot fork()\n", argv[0]);
      return EXIT_FAILURE;
    }
    else if (pid == 0) {
      /* child process */
      close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      if (execvp(cmd1[0], cmd1) < 0) {
	fprintf(stderr, "bummer, did not exec %s\n", strerror(errno));
	return EXIT_FAILURE;
      }
    }
    /* parent process */
    close(fd[0]);
    if (waitpid(pid, NULL, 0) < 0) {
      printf("%s: cannot wait for child\n", argv[0]);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
