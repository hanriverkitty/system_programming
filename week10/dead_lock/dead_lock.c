#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void fatal(const char *msg){
    perror(msg);
    //exit(EXIT_FAILURE);
}

int main(){
    int fd;
    struct flock first_lock, second_lock;

    first_lock.l_type = F_WRLCK;
    first_lock.l_whence = SEEK_SET;
    first_lock.l_start = 0;
    first_lock.l_len = 10;

    second_lock.l_type = F_WRLCK;
    second_lock.l_whence = SEEK_SET;
    second_lock.l_start = 10;
    second_lock.l_len = 5;

    fd = open("locktest", O_RDWR);

    if(fcntl(fd, F_SETLKW, &first_lock)==-1)
        fatal("Error (A)");
    printf("A: lock succeeded (proc %d)\n", getpid());

    switch(fork()){
        case -1:
            fatal("error on fork");
        case 0:
            if (fcntl(fd, F_SETLKW, &second_lock)==-1)
                fatal("Error (B)");
            printf("B: lock succeeded (proc %d)\n",getpid());
                printf("child sleeping\n");
            sleep(10);
            if(fcntl(fd,F_SETLKW,&first_lock)==-1)
                fatal("Error (C)");
            printf("C: lock succeeded (proc %d)\n", getpid());
            exit(0);
        default:
            sleep(5);
        if(fcntl(fd,F_SETLKW,&second_lock)==-1)
            fatal("Error (D)");
        printf("D: lock succeeded (proc %d)\n",getpid());
    }

}