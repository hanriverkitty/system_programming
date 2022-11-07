#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main (){
    int fd;
    struct flock my_lock;

    //쓰기 락의 인수를 지정
    my_lock.l_type = F_WRLCK;
    my_lock.l_whence = SEEK_SET;
    my_lock.l_start = 0;
    my_lock.l_len - 10;

    fd = open("locktest",O_RDWR);
    if(fcntl(fd, F_SETLKW, &my_lock)==-1){
        perror("parent: locked record\n");
        exit(1);
    }

    printf("parent: locked record\n");
    switch (fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            my_lock.l_len=5;
            if(fcntl(fd,F_SETLKW,&my_lock)==-1){
                perror("child: locking");
                exit(1);
            }
            printf("child: locked\n");
            printf("child: exiting\n");
            exit(0);
    }
    sleep(5);

    printf("parent: exiting\n");
    exit(0);
}