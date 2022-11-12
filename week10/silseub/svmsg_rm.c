#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>

void fatal(const char* msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int j;
    if (argc > 1 && strcmp(argv[1],"--help")==0)
        fprintf(stderr, "%s [msqid...]\n", argv[0]);
    
    for (j=1; j<argc; j++)
        if(msgctl(strtol(argv[j],NULL,0),IPC_RMID,NULL)==-1)
            fatal("msgctl");
    exit(EXIT_SUCCESS);
}