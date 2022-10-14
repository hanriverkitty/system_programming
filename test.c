#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#define MAXARGS 31
#define MAXFILENAME 1024
#define MAXPATHSTR 5000

extern char **environ;
int myexeclp(const char *file, const char *args, ...);

int main(void) {
  char path[MAXPATHSTR];
  sprintf(path, "PATH=%s:%s", getcwd(NULL, 0), getenv("PATH"));
  putenv(path);
	myexeclp("hello", "-a","-b","-c",(char*)0);
	
  return 0;
}

int myexeclp(const char *file, const char *args, ...){
    printf("hi\n");
	char vector[MAXARGS];
	va_list list;
	char *s;
    strcat(vector,args);
	va_start(list, args);
	for(int i=0;i<2;i++){
		 s = va_arg(list, char *);
        strcat(vector,s);
	}
	printf("%s\n",vector);
    printf("%c\n",vector[2]);
	return 0;
}