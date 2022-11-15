#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <unistd.h>

#define SHMKEY1 (key_t)0x10
#define SHMKEY2 (key_t)0x15
#define SEMKEY  (key_t)0x20

#define SIZ 5*BUFSIZ
struct databuf{
    int d_nread;
    char d_buf[SIZ];
};

int getseg(struct databuf **p1, struct databuf **p2);
int getsem(void);
int rm(void);
void reader(int semid,struct databuf *buf1, struct databuf *buf2);
void writer(int semid, struct databuf *buf1, struct databuf *buf2);