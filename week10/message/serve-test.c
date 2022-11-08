#include "queue.h"

void proc_obj(struct q_entry *msg){
    printf("\npriority: %ld name: %s\n", msg->mtype, msg->mtext);
}

int serve(void){
    static int r_qid = -1;
    struct q_entry r_entry;
    int mlen;

    /* initialize queue as necessary */
    if(r_qid == -1 && (r_qid = init_queue())==-1)
        return (-1);
    
    /* get and process next message, waiting if necessary */
    for(;;){
        if ((mlen = msgrcv(r_qid, &r_entry, MAXOBN, (long)-1 * MAXPRIOR, MSG_NOERROR))==-1){
            perror("msgrcv failed");
            return (-1);
        } else{
            /* make sure we've a string */
            r_entry.mtext[mlen] = '\0';

            /* print object name */
            proc_obj(&r_entry);
        }
    }
}
int main(void){
    int pid;

    switch (pid=fork()){
    case 0: /* child */
        serve();
        break; 
    case -1:
        perror("fork to start server failed");
        break;
    default:
        printf("server process pid is %d\n",pid);
    }
    exit(pid == -1 ? 1 : 0);
}