#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

#define USE_SIGSETJMP
sig_atomic_t canJump = 0;
#ifdef USE_SIGSETJMP
    sigjmp_buf senv;
#else
    jmp_buf env;
#endif

void errExit(const char* str){
    perror(str);
    exit(EXIT_FAILURE);
}
void printSigset(FILE *of,const char*prefix,const sigset_t *sigset){
    int sig,cnt;
    cnt=0;
    for(sig=1;sig<NSIG;sig++){
        if(sigismember(sigset,sig)){
            cnt++;
            fprintf(of,"%s%d (%s)\n",prefix,sig,strsignal(sig));
        }
    }
    if(cnt==0)
    fprintf(of,"%s<empty signal set>\n",prefix);
}
int printSigMask(FILE *of,const char *msg){
    sigset_t currMask;
    if(msg!=NULL)
    fprintf(of,"%s",msg);
    if(sigprocmask(SIG_BLOCK,NULL,&currMask)==-1)
    return -1;
    printSigset(of,"\t\t",&currMask);
    return 0;
}
static void handler(int sig){
    printf("Received siganl %d (%s), signal mask is:\n",sig,strsignal(sig));
    printSigMask(stdout,NULL);
    if(!canJump){
        printf("'env' buffer not yet set, doing a simmple return\n");
        return;
    }
#ifdef USE_SIGSETJMP
siglongjmp(senv,1);
#else
longjmp(env,1);
#endif
}

int main(int argc,char*argv[]){
    struct sigaction sa;
    printSigMask(stdout, "Signal mask at startup:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if(sigaction(SIGINT,&sa,NULL)==-1) errExit("sigaction");

#ifdef USE_SIGSETJMP
    printf("Calling sigsetjmp()\n");
    if(sigsetjmp(senv,1)==0)
#else
    printf("Calling setjmp()\n");
    if(setjmp(env)==0)
#endif
    canJump=1;
    else
        printSigMask(stdout,"After jump from handler, signal mask is:\n");

    for(;;)
        pause();
    return 0;
}