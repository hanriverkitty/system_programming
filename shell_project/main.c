#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pwd.h>

char buf[255];
char *cur_dir(char *buf1, size_t size);
char *get_usr_name();
char *current_name = "";
char *dir;

int main(void) {
	char *user = get_usr_name();
 
  while (1) {
    char buf1[255];
	printf("%s:%s$ ",user,cur_dir(buf1,255));
	fgets(buf,255,stdin);
  }
}

char *get_usr_name(){
	int a,b,c;
	if((a=open("get_usr_name",O_CREAT,0600))==-1){
		perror("open");
		exit(1);
	}
	struct stat info;
	char *user_name;
	struct passwd *my_passwd;
	if((b=stat("get_usr_name",&info))==-1){
		perror("stat error");
		exit(1);
	}
	my_passwd = getpwuid(info.st_uid);
	user_name = my_passwd->pw_name;
	if((c = remove("get_usr_name"))==-1){
		perror("remove error");
		exit(1);
	}
	return user_name;
}


char *cur_dir(char *buf1, size_t size) {

  char file_name[255] = ".";
  struct stat file_info;
  mode_t file_mode;
  ino_t inode;
  ino_t root_inode;
  ino_t search_inode;
  struct stat root_info;
  struct stat search_info;
  int return_stat, return_stat1, return_stat2;
  char search_dir[255] = "..";
  char dir_name[255] = "";

  //루트 디렉토리의 inode확인
  if ((return_stat1 = stat("/", &root_info)) == -1) {
    perror("Error : ");
    exit(0);
  }
  root_inode = root_info.st_ino;
  // printf("%ld\n",root_inode);

  struct dirent *item;
  DIR *dp;
  struct dirent *root_item;
  DIR *root_dp;

  root_dp = opendir("/");
  if (root_dp != NULL) {
    for (;;) {
      root_item = readdir(root_dp);
      if (root_item == NULL)
        break;
      // printf("%s  %ld\n", root_item->d_name,root_item->d_ino);
    }
    closedir(root_dp);
  }

  for (;;) {
    //현재디렉토리 inode 확인
    if ((return_stat2 = stat(file_name, &search_info)) == -1) {
      perror("Error : ");
      exit(0);
    }
    search_inode = search_info.st_ino;

    // printf("%ld\n",search_inode);

    //루트디렉토리랑 같을경우 for문 빠져나오기

    if (search_inode == root_inode)
      break;

    dp = opendir(search_dir);
    if (dp != NULL) {
      for (;;) {
        item = readdir(dp);
        if (item == NULL)
          break;
        // printf("%s  %ld\n", item->d_name,item->d_ino);
        if (search_inode == item->d_ino) {
          current_name = item->d_name;
          // printf("find direct %s\n",item->d_name);
        }
      }
      closedir(dp);
    }
    // printf("%s\n\n",current_name);
    //현재의 경로에서 부모경로로 경로 추가
    strcat(search_dir, "/..");
    strcat(file_name, "/..");
    char temporary_string[255];
    //파일 경로 순서 바꿔서 저장
    strcpy(temporary_string, "/");
    strcat(temporary_string, current_name);
    strcat(temporary_string, dir_name);
    strcpy(dir_name, temporary_string);
  }
  // printf("%s\n",answer);
  strcpy(buf1, dir_name);
  return buf1;
}
