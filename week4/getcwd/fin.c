#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

void creatEnv();
char *current_name = "";
char answer[255]="";
/**
 * 현재 디렉토리의 절대경로를 반환한다.
 * @param buf   경로명을 저장할 버퍼 변수.
 * @param size  버퍼 변수의 크기
 * @return 버퍼 변수의 시작 주소, 즉 buf
 */
char *mygetcwd(char *buf, size_t size) {
	char file_name[255]= ".";
	struct stat file_info;
	mode_t file_mode;
	ino_t inode;
    ino_t root_inode;
    ino_t search_inode;
    struct stat root_info;
    struct stat search_info;
	int return_stat,return_stat1,return_stat2;
	char search_dir[255]="..";

	//루트 디렉토리의 inode확인
    if((return_stat1 = stat("/", &root_info))==-1){
		perror("Error : ");
		exit(0);
	}
	root_inode = root_info.st_ino;
	//printf("%ld\n",root_inode);

    struct dirent *item;
    DIR *dp;
    struct dirent *root_item;
    DIR *root_dp;

    root_dp = opendir("/");
    if (root_dp != NULL)
    {
        for(;;)
        {
            root_item = readdir(root_dp);
            if (root_item == NULL)
                break;
            //printf("%s  %ld\n", root_item->d_name,root_item->d_ino);
        }
        closedir(root_dp);
    }

    
   for(;;){
    //현재디렉토리 inode 확인
    if((return_stat2 = stat(file_name, &search_info))==-1){
		perror("Error : ");
		exit(0);
	}
	search_inode = search_info.st_ino;

    //printf("%ld\n",search_inode);

    //루트디렉토리랑 같을경우 for문 빠져나오기

    if(search_inode==root_inode)break;

    dp = opendir(search_dir);
    if (dp != NULL)
    {
        for(;;)
        {
            item = readdir(dp);
            if (item == NULL)
                break;
            //printf("%s  %ld\n", item->d_name,item->d_ino);
            if(search_inode==item->d_ino){
                current_name=item->d_name;
                //printf("find direct %s\n",item->d_name);
            }
            
        }
        closedir(dp);
    }
	//printf("%s\n\n",current_name);
    //현재의 경로에서 부모경로로 경로 추가
    strcat(search_dir,"/..");  
    strcat(file_name,"/..");
    char temporary_string[255];
    //파일 경로 순서 바꿔서 저장
    strcpy(temporary_string,"/");
    strcat(temporary_string,current_name);
    strcat(temporary_string,answer);  
    strcpy(answer,temporary_string);
    }
    //printf("%s\n",answer);
    strcpy(buf,answer);
  return buf;
}

int main(void) {
  pid_t pid;
  int status;
  char buf[255];

	
  creatEnv();
  chdir("dir/sub");

  printf("original func: %s\n", getcwd(NULL, 0));
  printf("mygetcwd func: %s\n", mygetcwd(buf, 255));

  return 0;
}

void creatEnv(){
  mkdir("dir", 0755);
  mkdir("dir/sub", 0755);
  mkdir("dir/sub2", 0);
  
  creat("dir/a", 0755);
  creat("dir/b", 0755);
  creat("dir/sub/x", 0755);
  symlink("dir/a", "dir/sl");
  symlink("dir/x", "dir/dsl");
}