#include "queue.h"

int enter(char *objname, int priority){
    int len;
    struct q_entry s_entry; /* structure to hold message */
        static int s_qid = -1;

    /* validate name length, priority level */
    if ((len = strlen(objname)) > MAXOBN){
        fprintf(stderr, "name too long");
        return (-1);
    }
    if (priority > MAXPRIOR || priority <0){
        fprintf(stderr, "invalid priority level");
        return (-1);
    }

    /* initial message queue as necessary */
    if (s_qid == -1 && (s_qid = init_queue())==-1)
        return (-1);

    /* initialize s_entry */
    s_entry.mtype = (long)priority;
    strncpy(s_entry.mtext, objname, MAXOBN);

    /* send message, waiting if necessary */
    if (msgsnd(s_qid, &s_entry, len, 0)==-1){
        perror("msgsnd failed");
        return (-1);
    } else
        return (0);
}
int main(int argc, char *argv[]){
    int priority;

    if (argc != 3){
        fprintf(stderr, "usage: %s objname priority\n", argv[0]);
        exit(1);
    }
    if ((priority = atoi(argv[2]))<=0 || priority > MAXPRIOR){
        fprintf(stderr, "enter failure");
        exit(3);
    }
    if (enter(argv[1],priority)<0){
        fprintf(stderr, "enter failure");
        exit(3);
    }
    exit(0);
}