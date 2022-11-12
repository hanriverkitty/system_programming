#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MTEXT 1024

struct mbuf {
    long mtype;
    char mtext[MAX_MTEXT];
};

void fatal(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int msqid, flags, msglen;
    struct mbuf msg;
    int opt;
    
    flags = 0;
    while((opt = getopt(argc, argv, "n")) != -1){
        if (opt=='n')
            flags |= IPC_NOWAIT;
        else
            fatal(NULL);
    }

    if (argc < optind+2 || argc > optind +3)
        fatal("Wrong number of arguments\n");
    
    msqid = strtol(argv[optind],NULL,0);
    msg.mtype = strtol(argv[optind+1],NULL,0);
    
    if (argc > optind+2){
        msglen = strlen(argv[optind+2])+1;
        memcpy(msg.mtext, argv[optind+2],msglen);
    } else {
        msglen = 0;
    }

    if (msgsnd(msqid,&msg,msglen,flags)==-1) fatal("msgsnd");
    exit(EXIT_SUCCESS);
    
}