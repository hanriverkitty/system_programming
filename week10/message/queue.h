#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

extern int errno;

#define QKEY (key_t)0150 /*identifying key for queue*/
#define QPERM 0660 /*permissions for queue*/
#define MAXOBN 50 /*maximum length obj. name*/
#define MAXPRIOR 10 /*maximum priority level*/

struct q_entry { /*struct we well use for message*/
    long mtype;
    char mtext[MAXOBN+1];
};

/* function prototypes */
int enter(char *objname, int priority);
int init_queue(void);
int serve(void);
