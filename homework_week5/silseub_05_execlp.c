#define _XOPEN_SOURCE
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 5000

extern char **environ;
int myexeclp(const char *file, const char *args, ...);
int check(char *pathnames, char *file);
char answer[MAXPATHSTR] = "";
int main(void) {
  char path[MAXPATHSTR];
  sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
  // prepare the executable file named "hello"
  system("cp hello.tmp hello.c");
  system("clang -pthread -lm -o hello hello.c");
  system("rm hello.c");
  // char *file_dir=strcat(getcwd(NULL,0),"/hello");
  // char *exam[] = {file_dir,"-a","-b","-c",NULL};
  // char *pathvar[] = {getenv("PATH"), NULL};
  // execve(file_dir, exam, pathvar);
  printf("%d\n", myexeclp("hello", "-a", "-b", "-c", (char *)0));

  return 0;
}

int myexeclp(const char *file, const char *args, ...) {
  char *vector[MAXARGS];
  char ars[MAXFILENAME];
  va_list list;
  char *s;
  char *pathvar[] = {getenv("PATH"), NULL};
  int cnt = 2;

  va_start(list, args);
  vector[0] = (char *)file;
  vector[1] = (char *)args;

  while (*args) {
    vector[cnt] = va_arg(list, char *);
    cnt++;
    args++;
  }
  va_end(list);
  vector[cnt] = NULL;

  // printf("%s\n",vector[1]);
  // printf("%s\n",vector[2]);
  // printf("%s\n",vector[3]);

  char file_dir[MAXPATHSTR];
  strcpy(file_dir, getcwd(NULL, 0));
  strcat(file_dir, "/");
  strcat(file_dir, file);

  //파일이 존재하고 읽기가 가능하면 2를받음
  if (check(pathvar[0], (char *)file) == 2) {
    //복사된 경로로 실행
    if (execve(answer, vector, pathvar) < 0) {
      fprintf(stderr, "execve error\n");
      exit(-1);
    }
  } else {
    return -1;
  }
  return 0;
}

int check(char *pathnames, char *file) {
  char pathname[MAXPATHSTR];
  char copy_pathnames[MAXPATHSTR];
  strcpy(copy_pathnames, pathnames);

  int cnt = 0;

  char *ptr1 = strtok(copy_pathnames, ":");
  //콜론 기준으로 몇개인지 검색
  while (ptr1 != NULL) {
    cnt++;
    ptr1 = strtok(NULL, ":");
  }
  // printf("%d\n", cnt);
  char copy_pathname[cnt][MAXPATHSTR];
  //콜론 기준으로 나누기
  char *ptr = strtok(pathnames, ":");
  int i = 0;
  int j = cnt;

  //배열뒤쪽부터 경로를 저장
  while (ptr != NULL) {
    strcpy(pathname, ptr);
    strcat(pathname, "/");
    strcat(pathname, file);
    // printf("%s\n", pathname);
    strcpy(copy_pathname[j - 1], (pathname));
    j--;
    ptr = strtok(NULL, ":");
  }
  // home/runner/silseub-0-execlp-gunheonhagi-ShinChanKyu/hello
  while (i <= cnt) {
    // printf("%s\n",copy_pathname[i]);
    //나눈 콜론으로 파일이 있는지, 읽기권한이 있는지 확인후 있으면 2 return
    if (access(copy_pathname[i], F_OK) < 0) {
      i++;
      continue;
    } else {
      mode_t file_mode;
      struct stat file_info;
      int return_stat;

      if ((return_stat = stat(copy_pathname[i], &file_info)) == -1) {
        perror("Error : ");
        exit(0);
      }
      file_mode = file_info.st_mode;
      //읽기권한이 있는지
      if ((file_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0) {
        printf("No permission\n");
        exit(-1);
      } else {
        printf("%s\n", copy_pathname[i]);
        // pathname 복사
        strcpy(answer, copy_pathname[i]);
        // printf("%s\n",answer);
        return 2;
      }
    }
    i++;
  }
  return 0;
}