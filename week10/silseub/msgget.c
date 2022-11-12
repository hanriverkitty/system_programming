#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

void fatal(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int numKeyFlags;
    int flags, msqid, opt;
    unsigned int perms;
    long lkey;
    key_t key;

    numKeyFlags = 0;
    flags = 0;

    while ((opt = getopt(argc,argv, "cf:k:px"))!= -1){
        switch (opt){
            case 'c':
                flags |= IPC_CREAT;
                break;
            case 'f':
                key = ftok(optarg,1);
                if (key==-1) fatal("ftok");
                numKeyFlags++;
                break;
            case 'k':
                if(sscanf(optarg,"%li",&lkey)!=1)
                    fatal("-k operation requires a numeric argument\n");
                key = lkey;
                numKeyFlags++;
                break;
            case 'p':
                key = IPC_PRIVATE;
                numKeyFlags++;
                break;
            case 'x':
                flags |= IPC_EXCL;
                break;
            default:
                fatal("Bad option\n");
        }
    }
    if (numKeyFlags !=1)
        fatal("Exactly one of the options -f, -k, ""or -p muset be supplied\n");
    perms = (optind==argc) ? (S_IRUSR | S_IWUSR) :
        strol(argv[optind], NULL, 8);
    msqid = msgget(key,flags|perms);
    if (msqid == -1) fatal("msgget");
    printf("%d\n",msqid);
    exit(EXIT_SUCCESS);
}