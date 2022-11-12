#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>

void fatal (const char*msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int maxind, ind, msqid;
    struct msqid_ds ds;
    struct msginfo msginfo;

    maxind = msgctl(0,MSG_INFO, (struct msqid_ds *) &msginfo);
    if(maxind==-1)
        fatal("msgctl-MSG_INFO");
    printf("maxind: %d\n\n", maxind);
    printf("index       id      key     messages\n");

    for(ind=0; ind<=maxind;ind++){
        msqid = msgctl(ind, MSG_STAT, &ds);
        if (msqid == -1){
            if(errno != EINVAL && errno != EACCES)
                fatal("msgctl-MSG_STAT");
            continue;
        }
        printf("%4d %8d 0x%08lx %7ld\n", ind, msqid, (unsigned long)ds.msg_perm.__key,(long)ds.msg_qnum);
    }
    exit(EXIT_SUCCESS);
}