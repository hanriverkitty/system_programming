#include <stdio.h>
#include "pv.h"

int handlesem(key_t skey){
    int semid, pid = getpid();
    if((semid = initsem(skey))<0)
        exit(1);
    printf("\nprocess %d before critical section\n",pid);
    p(semid);
    printf("process %d in critical section\n",pid);
    sleep(10);
    printf("process %d leaving critical section\n",pid);
    v(semid);
    printf("process %d exiting\n",pid);
    exit(0);
}

void main(void){
    key_t semkey = 0x200;
    pid_t p1,p2,p3;
    if((p1=fork())==0) handlesem(semkey);
    if((p2=fork())==0) handlesem(semkey);
    if((p3=fork())==0) handlesem(semkey);
    waitpid(p1,NULL,0);
    waitpid(p2,NULL,0);
    waitpid(p3,NULL,0);

}