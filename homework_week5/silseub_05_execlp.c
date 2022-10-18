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
char answer[MAXPATHSTR]="";
int main(void) {
  char path[MAXPATHSTR];
  sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
  // prepare the executable file named "hello"
  system("cp hello.tmp hello.c");
  system("gcc -pthread -lm -o hello hello.c");
  system("rm hello.c");
  // char *file_dir=strcat(getcwd(NULL,0),"/hello");
  // char *exam[] = {file_dir,"-a","-b","-c",NULL};
  // char *pathvar[] = {getenv("PATH"), NULL};
  // execve(file_dir, exam, pathvar);
  printf("%d\n",myexeclp("hello", "-a", "-b", "-c", (char *)0));

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

  if (check(pathvar[0], (char *)file)==2) {
	  //복사된 경로로 실행
    if (execve(answer, vector, pathvar) < 0) {
      fprintf(stderr, "execve error\n");
      exit(-1);
    }
  } 
  else{
    return -1;
	  }
  return 0;
}

int check(char *pathnames, char *file) {
  char pathname[MAXPATHSTR];
//콜론 기준으로 나누기
  char *ptr = strtok(pathnames, ":");
	int i =0;
  while (ptr != NULL) {
    strcpy(pathname, ptr);
    strcat(pathname, "/");
    strcat(pathname, file);
    printf("%s\n", pathname);
	// strcpy(copy_pathname[i] ,(pathname));
	// i++;

	  //나눈 콜론으로 파일이 있는지, 읽기권한이 있는지 확인후 있으면 2 return
    if (access(pathname, F_OK) < 0) {
      ptr = strtok(NULL, ":");
      continue;
    } else {
      mode_t file_mode;
      struct stat file_info;
      int return_stat;
		
      if ((return_stat = stat(pathname, &file_info)) == -1) {
        perror("Error : ");
        exit(0);
      }
      file_mode = file_info.st_mode;
		//읽기권한이 있는지
      if ((file_mode & S_IXUSR) == 0) {
        ptr = strtok(NULL, ":");
        printf("No permission\n");
        exit(-1);
      } else {
        //printf("%s\n", pathname);
		//pathname 복사
		strcpy(answer,pathname);
		//printf("%s\n",answer);
		return 2;
      }
    }
	  	ptr = strtok(NULL, ":");
  }
	return 0;
}