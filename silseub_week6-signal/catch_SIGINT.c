#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int count;

void catch_sigint(int signum){
    printf("\n(count=%d) CTRL-C pressed!\n",count);
    return;
}

int main(int argc, char *argv[]){
    struct sigaction act;
    sigset_t masksets;
    int i;
    char buf[10];
    sigfillset(&masksets);
    act.sa_handler = catch_sigint;

    act.sa_mask = masksets;
    act.sa_flags = 0;
    sigaction(SIGINT,&act,NULL);

    for(count=0; count<3; count++)
        read(0,buf,sizeof(buf));
    return 0;
    }
