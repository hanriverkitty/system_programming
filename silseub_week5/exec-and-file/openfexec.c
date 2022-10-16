#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv){
    int fd = atoi(argv[1]);
    long pos;
    pos = lseek(fd,0l,SEEK_CUR);
    printf("\tPos in openfexec(): is %ld\n",pos);

    pos = lseek(fd,50l,SEEK_CUR);
    printf("\tNewpos after lseek() in openfexec() is %ld\n",pos);

    return (pos<0 ? !0 :0);
}