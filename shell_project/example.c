#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define EOL 1
#define ARG 2
#define AMPERSAND 3
#define SEMICOLON 4
#define MAXARG 512
#define MAXBUF 512
#define FOREGROUND 0
#define BACKGROUND 1
int userin (char* p);
void procline();
int gettok (char** outptr);
int inarg (char c);
int runcommand (char **cline, int where);

char *prompt = "Command> ";
int
main() {
    while(userin (prompt) != EOF)
        procline();
    return 0;
}

static char inpbuf [MAXBUF];
static char tokbuf [2*MAXBUF];
static char *ptr = inpbuf;
static char *tok = tokbuf;
static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

int userin (char* p){
int c, count;
ptr= inpbuf;
tok= tokbuf;
printf("%s",p);
count = 0;
while(1) {
    if ((c =getchar ()) ==EOF)
        return EOF;
    if (count < MAXBUF)
        inpbuf[count++] = c;
    if (c == '\n' && count < MAXBUF) {
        inpbuf[count] ='\0';
        return count;
    }
    if (c == '\n' || count >= MAXBUF){
        printf("smallsh : input line too long \n");
        count = 0;
        printf("%s",p);
        }
    }
}

int gettok (char** outptr ){
int type;
*outptr = tok;
while (*ptr == ' ' || ptr == '\t')
        ptr++;
*tok ++ = ptr;
switch (*ptr ++){
    case '\n':
        type = EOL;
        break;
    case '&':
        type = AMPERSAND;
        break;
    case ';':
        type = SEMICOLON;
        break;
    default:
        type = ARG;
        while(inarg(*ptr))
            *tok ++ = *ptr++;
}
*tok ++ = '\0';
return type;
}

static char special [] = {' ', '\t', '&', ';', '\n', '\0'};
int inarg (char c){
    char *wrk;

    for (wrk = special; *wrk ; wrk ++){
        if (c == *wrk)
            return 0;
    }
    return 1;
}

void procline (){
    char *arg [MAXARG +1];
    int toktype ,type;
    int narg = 0;
    for (;;) {
        switch (toktype = gettok(&arg[narg])){
            case ARG:
                if (narg <MAXARG)
                    narg++;
                    break;
            case EOL:
            case SEMICOLON:
            case AMPERSAND:
                if (toktype == AMPERSAND) type = BACKGROUND;
                else type = FOREGROUND;
                if (narg != 0){
                    arg[narg] = NULL;
                    runcommand(arg ,type);
                }
                if (toktype == EOL) return
                narg=0;
                break;
}
}
}