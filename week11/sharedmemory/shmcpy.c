#include "share.h"

void main(void){
    int semid, pid;
    struct databuf *buf1, *buf2;

    semid= getsem();
    getseg(&buf1,&buf2);
    switch (pid=fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            writer(semid,buf1,buf2);
            rm();
            break;
        default:
            reader(semid,buf1,buf2);
            break;
    }
    exit(0);
}