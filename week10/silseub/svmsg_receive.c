#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MTEXT 1024

struct mbuf{
    long mtype;
    char mtext[MAX_MTEXT];
};

void fatal(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int msqid, flags, type;
    ssize_t msglen;
    size_t maxBytes;
    struct mbuf msg;
    int opt;

    flags = 0;

    type = 0;
    while((opt = getopt(argc,argv,"ent:x"))!=-1){
        switch (opt){
            case 'e': flags |= MSG_NOERROR; break;
            case 'n': flags |= IPC_NOWAIT; break;
            case 't': type = atoi(optarg); break;
    #ifdef MSG_EXCEPT
            case 'x': flags |= MSG_EXCEPT; break;
    #endif 
            default:  fatal(NULL);
        }
    }

    if (argc < optind +1 || argc > optind+2)
        fatal("Wrong numver of arguments\n");

    msqid = strtol(argv[optind],NULL,0);
    maxBytes = (argc > optind+1) ? strtol(argv[optind+1],NULL,0) : MAX_MTEXT;

    msglen = msgrcv(msqid, &msg, maxBytes, type, flags);
    if(msglen==-1)
        fatal("msgrcv");
    printf("Received: type=%ld; length=%ld", msg.mtype, (long)msglen);
    if(msglen>0)
        printf("; body=%s", msg.mtext);
    printf("\n");
    exit(EXIT_SUCCESS);
}
