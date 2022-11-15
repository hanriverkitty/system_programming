#include "share.h"
#define IFLAGS (IPC_CREAT|IPC_EXCL)
#define ERR ((struct databuf *)-1)

static int shmid1, shmid2, semid;

int getseg(struct databuf **p1, struct databuf **p2){
    if((shmid1 = shmget(SHMKEY1,sizeof(struct databuf), 0600|IFLAGS))<0){
        perror("shmget");
        exit(1);
    }
    if((shmid2 = shmget(SHMKEY2,sizeof(struct databuf), 0600|IFLAGS))<0){
        perror("shmget");
        exit(1);
    }
    if ((*p1 = (struct databuf *) shmat(shmid1,0,0))==ERR){
        perror("shmget");
        exit(1);
    }
    if ((*p2 = (struct databuf *) shmat(shmid2,0,0))==ERR){
        perror("shmget");
        exit(1);
    }
}

int getsem(void){
    if((semid = semget(SEMKEY,2,0600|IFLAGS))<0){
        perror("semget");
        exit(1);
    }
    if(semctl(semid,1,SETVAL,0)<0){
        perror("semctl");
        exit(1);
    }
    return(semid);
}

int rm(void){
    if(shmctl(shmid1,IPC_RMID, (struct shmid_ds *)0)<0){
        perror("shmctl");
        exit(1);
    }
    if(shmctl(shmid2,IPC_RMID,(struct shmid_ds *)0)<0){
        perror("shmctl");
        exit(1);
    }
    if(semctl(semid,0,IPC_RMID,(struct shmid_ds *)0)<0){
        perror("semctl");
        exit(1);
    }
}


struct sembuf p1 = {0,-1,0}, p2 = {1,-1,0};
struct sembuf v1 = {0,1,0}, v2 = {1,1,0};

void reader(int semid, struct databuf *buf1, struct databuf *buf2){
    for(;;){
        buf1->d_nread = read(0,buf1->d_buf,SIZ);

        semop(semid, &v1,1);
        semop(semid,&p2,1);
        if(buf1->d_nread <=0) return;
        buf2->d_nread = read(0,buf2->d_buf,SIZ);
        semop(semid,&v1,1);
        semop(semid,&p2,1);
        if(buf2->d_nread <=0) return;
    }
}


void writer(int semid,struct databuf *buf1, struct databuf *buf2){
    for(;;){
        semop(semid,&p1,1);
        semop(semid,&v2,1);
        if(buf1->d_nread<=0) return;
        write(1,buf1->d_buf,buf1->d_nread);
        semop(semid,&p1,1);
        semop(semid,&v2,1);

        if(buf2->d_nread <=0) return;
        write(1,buf2->d_buf, buf2->d_nread);
    }
}