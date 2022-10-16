#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_handler(int signo){
    printf("SIGINT\n");
}

int main(){
    printf("hello world!\n");
    signal(SIGINT,(void *)sig_handler);
    pause();
    printf("Interrupted\n");
}