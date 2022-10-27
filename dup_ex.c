#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    int fd,fd1;

    if((fd = open("tmp",O_CREAT | O_WRONLY | O_TRUNC,0644))==-1){
        perror("create");
        exit(1);
    }
   

    fd1=dup(fd);
    printf("Ori FD = %d\n",fd);
    printf("DUP FD = %d\n",fd1);
    printf("Standard Output Redirection\n");
    close(fd);
    return 0;
}