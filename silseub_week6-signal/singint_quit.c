#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void errExit(const char *str){
    perror(str);
    exit(EXIT_FAILURE);
}

static void sigHandler(int sig){
    static int count = 0;
    if(sig==SIGINT){
        count++;
        printf("Caught SIGINT (%d)\n",count);
        return;
    }

    printf("Caught SIGQUIT!\n");
    exit(EXIT_SUCCESS);
}

int main(void){
    if(signal(SIGINT,sigHandler)==SIG_ERR) errExit("singnal");
    if(signal(SIGQUIT,sigHandler)==SIG_ERR) errExit("signal");
    for(;;)
    pause;
    return 0;
}