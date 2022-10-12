#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

char *current_name = "";
char answer[255]="";

int main()
{	char file_name[255]= ".";
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
	printf("%ld\n",root_inode);

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
            printf("DIR : %s  %ld\n", root_item->d_name,root_item->d_ino);
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

    printf("%ld\n",search_inode);

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
            printf("%s  %ld\n", item->d_name,item->d_ino);
            if(search_inode==item->d_ino){
                current_name=item->d_name;
                printf("find direct %s\n",item->d_name);
            }
            
        }
        closedir(dp);
    }
	printf("%s\n\n",current_name);
    strcat(search_dir,"/..");  
    strcat(file_name,"/..");    
    strcat(answer,current_name);
    strcat(answer,"/");
    }
    printf("%s\n",answer);
}
