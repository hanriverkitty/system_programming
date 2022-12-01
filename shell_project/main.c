#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *get_usr_name();
char *current_name = "";
void erase_space(char *a);
char *dir;
char command[7][3] = {{">"},{">>"},{">|"},{"&"},{";"},{"<"},{"|"}};


int main(void) {
  char *user = get_usr_name();

  while (1) {
    char buf[255];
    char buf1[255];
    int m = 0;
    printf("%s:%s$ ", user, getcwd(NULL, 0));
    fgets(buf, 255, stdin);
    /* history 파일에 명령어 추가 */
    if(buf[0]!='\n'){
    int his_fd;
    if((his_fd = open("history",O_CREAT|O_WRONLY|O_APPEND,0600))<0){
      perror("open err");
      exit(1);
    }
    if((write(his_fd,buf,strlen(buf)))!=strlen(buf)){
      perror("write err");
      exit(1);
    }
    close(his_fd);
    }

    
    buf[strlen(buf)-1]='\0';

    /*공백제거*/
    erase_space(buf);
    printf("%s\n", buf);
    char a[255];
    strcpy(a,buf);

    if(strcmp(buf,"history")==0){
      int his_fd1;
      int n;
      int i=1;
      char read_buf[255];
      
      his_fd1=open("history",O_RDONLY);
      while(n=read(his_fd1,read_buf,255)>0){
          char *ptr = strtok(read_buf, "\n");    
          while (ptr != NULL){
            printf("\t\t%d\t%s\n", i,ptr);         
            ptr = strtok(NULL, "\n");
            i++; 
          }
        }
      close(his_fd1);
    }
    // char *sear = strchr(buf, ';');
    // if (sear != NULL) {
    //   printf("%s\n", sear);
    //   sear = strtok(NULL, ";");
    // }
 //    char *mode = strtok(buf, "><|&");
	//   printf("%s\n",mode);
	// if(mode==NULL){
	// 	m=10;
	// }
	// mode = strchr(buf,'>');
 //    if (mode != NULL) {
	// 	printf("%s\n",mode);
 //      if (*(mode + 1) == '>') {
 //        m = 1;
	// 	char *mode = strtok(buf,">>");
 //      } else if (*(mode + 1) == '|') {
 //        m = 2;
	// 	char *mode = strtok(buf,">|");
 //      }
 //    }
	// mode = strchr(buf, '<');
	//   if(mode !=NULL){
	// 	  m=3;
	//   }
	   printf("%d\n",m);
  }
}

char *get_usr_name() {
  int a, b, c;
  if ((a = open("get_usr_name", O_CREAT, 0600)) == -1) {
    perror("open");
    exit(1);
  }
  struct stat info;
  char *user_name;
  struct passwd *my_passwd;
  if ((b = stat("get_usr_name", &info)) == -1) {
    perror("stat error");
    exit(1);
  }
  my_passwd = getpwuid(info.st_uid);
  user_name = my_passwd->pw_name;
  if ((c = remove("get_usr_name")) == -1) {
    perror("remove error");
    exit(1);
  }
  return user_name;
}

void erase_space(char *buf) {
  char *a = buf;
  int b = 0;

  while (*buf != 0) {
    if (*buf != ' ') {
      *a = *buf;
      a++;
    }
    buf++;
  }
  *a = 0;
}