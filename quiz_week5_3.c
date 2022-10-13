#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static void charatatiome (char*str){
    char *ptr;
    int c;
    setbuf(stdout,NULL);

    for (ptr = str;c=*ptr++;)
        putc(c,stdout);
}

int main(void){
    pid_t pid;
    if((pid=fork())<0){
        perror("fork error\n");
        exit(1);
    }else if (pid==0){
        pause();
        charatatiome("output from child\n");
    }else{
        charatatiome("output from parent\n");
        kill(pid,SIGALRM);
    }
    exit(0);
}