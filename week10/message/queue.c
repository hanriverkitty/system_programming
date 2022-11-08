#include "queue.h"

int init_queue(void){
    int queue_id;

    /* attemp to create message queue*/
    if ((queue_id = msgget(QKEY, IPC_CREAT | QPERM))==-1)
        perror("msgget failed");
    return (queue_id);
}