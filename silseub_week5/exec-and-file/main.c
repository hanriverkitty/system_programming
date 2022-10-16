#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int fdes, pid, cflag, excode;
    long pos;
    char sfdes[10];

    fdes = open("main.c",O_RDONLY);
    sprintf(sfdes,"%d",fdes);
    printf("fdes = %d\n",fdes);
    pos = lseek(fdes,20l,SEEK_SET);
    printf("Current position before fork() is %ld\n",pos);
    if(!fork()){
        pos = lseek(fdes,40l,SEEK_SET);
        printf("Current position in child after fork() is %ld\n",pos);
    }
    else {
    wait((int *)0);
    pos = lseek(fdes, 0l, SEEK_CUR);
    printf("Current position in parent after fork() is %ld\n", pos);
    if (!fork()) {
        execl("./openfexec", "openfexec", sfdes, (char *)0);
        printf("It is an error to print this line out\n");
    }
    wait(&excode);
    pos = lseek(fdes, 0l, SEEK_CUR);
    printf("Current pos in parent after exec() is %ld\n", pos);
    printf("Exit code of a child = %d\n", WEXITSTATUS(excode));
    cflag = fcntl(fdes, F_GETFD, 0);
    printf("close-on-exec flag = %d\n", cflag);
    //FD_CLOEXEC에 플래그를 설정하여 file descriptor를 close한다.
    fcntl(fdes, F_SETFD, 1); 
    if ((pid = fork()) != 0) {
        waitpid(pid, &excode, 0);
        printf("Exit code of a specific child = %d\n", WEXITSTATUS(excode));
        exit(0);
        }
        execl("./openfexec", "openfexec", sfdes, (char *)0);
        printf("It is an error to print this line out\n");
        }
}