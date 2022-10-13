#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid;
    char path[3000];
    char *vector[]={"echoall","myarg1","MY AGR2",(char*)0};
    if ((pid=fork())<0){
        perror("fork error");
        exit(1);
    }else if(pid==0){
        sprintf(path,"PATH=%s",getcwd(NULL,0));
        char *env_init[]={"USER=unknown",path,NULL};
        if(execve("echoall",vector,env_init)<0){
            fprintf(stderr,"execve error\n");
            exit(1);
        }
    }//부모
    if(waitpid(pid,NULL,0)<0){
        perror("waitpid error");
        exit(1);
    }
    if((pid==fork())<0){
        perror("fork2 error");
        exit(1);
    }else if(pid==0){
        if(execlp("echoall","echoall","only one arg",(char*)0)<0){
            perror("execlp error");
            exit(1);
        }
    }
    return 0;
}