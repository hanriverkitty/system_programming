#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_BUF 1024

int main(void) {
  int fd[2], fd2[2];
  pid_t pid;

  if (pipe(fd) < 0) {
    perror("pipe call");
    exit(1);
  }
  if (pipe(fd2) < 0) {
    perror("pipe call");
    exit(1);
  }

  if ((pid = fork()) < 0) {
    perror("fork call");
    exit(2);
  }

  //자식
  if (pid == 0) {
    close(fd[1]);
    close(fd2[0]);
    for (;;) {
      char buf[MAX_BUF];
      char change[MAX_BUF];
      int n;
      if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
        perror("read fail");
        exit(1);
      }
      int a = 0;
      //대문자변환
      while (a != strlen(buf)) {
        change[a] = toupper(buf[a]);
        a++;
      }
      change[a] = '\0';
      if ((n = write(fd2[1], change, MAX_BUF)) == -1) {
        perror("write fail");
        exit(1);
      }
    }
  } else {
    close(fd[0]);
    close(fd2[1]);
    for (;;) {
      char buf[MAX_BUF];
      int n;
      fgets(buf, MAX_BUF, stdin);
      if ((n = write(fd[1], buf, MAX_BUF)) == -1) {
        perror("write fail");
        exit(1);
      };
      char change[MAX_BUF];
      if ((n = read(fd2[0], change, MAX_BUF)) < 0) {
        perror("read fail");
        exit(1);
      }
      printf("%s", change);
    }
  }

  return 0;
}