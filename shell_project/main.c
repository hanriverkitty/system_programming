#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *get_usr_name();
char *current_name = "";
void erase_space(char *a);
char *dir;
void history(char buf[]);
bool is_pipe = false;
char *buf1[20];
int leng = 1;
int g, fd;
int his_exe = 1;
char num[255];

int main(void)
{
  char *user = get_usr_name();

  while (1)
  {
    bool back = false;
    int pid;
    leng = 0;
    char buf[255];
    int m = 0;
    printf("%s:%s$ ", user, getcwd(NULL, 0));
    printf("%s", num);
    fgets(buf, 255, stdin);

    /* history 파일에 명령어 추가 */
    if (buf[0] != '\n')
    {
      int his_fd;
      if ((his_fd = open("history", O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0)
      {
        perror("open err");
        exit(1);
      }
      if ((write(his_fd, buf, strlen(buf))) != strlen(buf))
      {
        perror("write err");
        exit(1);
      }
      close(his_fd);
    }

    buf[strlen(buf) - 1] = '\0';

    /*공백제거*/
    // erase_space(buf);
    printf("%s\n", buf);
    char a[255];
    strcpy(a, buf);

    if (strcmp(buf, "history") == 0)
    {
      int his_fd1;
      int n;
      int i = 1;
      char read_buf[255];

      his_fd1 = open("history", O_RDONLY);
      while (n = read(his_fd1, read_buf, 255) > 0)
      {
        char *ptr = strtok(read_buf, "\n");
        while (ptr != NULL)
        {
          printf("\t\t%d\t%s\n", i, ptr);
          ptr = strtok(NULL, "\n");
          i++;
        }
      }
      close(his_fd1);
    }

    for (int i = 0; i < 20; i++)
    {
      buf1[i] = malloc(sizeof(char) * 20);
      strcpy(buf1[i], "");
    }

    for (int i = 0; i < strlen(a); i++)
    {
      switch (a[i])
      {
      case ' ':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        break;
      case '>':
        if (leng > 0 && strcmp(buf1[leng - 1], ">") == 0)
        {
          strcat(buf1[leng - 1], ">");
        }
        else
        {
          if (strcmp(buf1[leng], "") != 0)
            leng++;
          strcat(buf1[leng], ">");
          leng++;
        }
        break;
      case '|':
        if (strcmp(buf1[leng - 1], ">") == 0)
          strcat(buf1[leng - 1], "|");
        else
        {
          if (strcmp(buf1[leng], "") != 0)
            leng++;
          strcat(buf1[leng], "|");
          leng++;
          is_pipe = true;
          break;
        }
        break;
      case '<':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], "<");
        leng++;
        break;
      case '&':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], "&");
        leng++;
        back = true;
        break;
      case ';':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], ";");
        leng++;
        break;
      case '!':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], "!");
        leng++;
        break;
      default:
        strncat(buf1[leng], &buf[i], 1);
        break;
      }
    }
    /* fork 및 명령어 실행*/
    switch (pid = fork())
    {
    case -1:
      perror("fork error");
      break;
      exit(1);
    case 0:
      g = 0;
      int index = 0;
      char *exe[20];
      for (int i = 0; i < 20; i++)
      {
        exe[i] = NULL;
      }
      char first_exe[255];
      while (g <= leng)
      {

        if (strcmp(buf1[g], ">") == 0)
        {
          if ((fd = open(buf1[g + 1], O_CREAT | O_RDWR | O_TRUNC, 0666)) < 0)
          {
            perror("open error");
            his_exe = true;
            exit(1);
          }
          dup2(fd, 1);
          close(fd);
          g += 2;
        }
        else if (strcmp(buf1[g], ">>") == 0)
        {
          if ((fd = open(buf1[g + 1], O_CREAT | O_RDWR | O_APPEND, 0666)) < 0)
          {
            perror("open error");
            exit(1);
          }
          dup2(fd, 1);
          close(fd);
          g += 2;
        }
        else if (strcmp(buf1[g], "history") == 0)
        {
          history(buf1[g]);
          g++;
        }
        else
        {
          if (index == 0)
          {
            strcpy(first_exe, buf1[g]);
            // strcat(exe[index], buf1[g]);
            exe[index] = buf1[g];
            index++;
          }
          else
          {
            // strcpy(exe[index], buf1[g]);
            exe[index] = buf1[g];
            index++;
          }
          g++;
        }
      }
      printf("%sdwawdaw\n", first_exe);
      printf("%s\t\t%s\n", first_exe, *exe);
      execvp(first_exe, exe);
      break;
    default:
      if (back)
      {
        back = false;
        printf("%d\n", getpid());
      }

      else
      {
        wait(NULL);
        // printf("front\n");
      }
    }
  }
}

char *get_usr_name()
{
  int a, b, c;
  if ((a = open("get_usr_name", O_CREAT, 0600)) == -1)
  {
    perror("open");
    exit(1);
  }
  struct stat info;
  char *user_name;
  struct passwd *my_passwd;
  if ((b = stat("get_usr_name", &info)) == -1)
  {
    perror("stat error");
    exit(1);
  }
  my_passwd = getpwuid(info.st_uid);
  user_name = my_passwd->pw_name;
  if ((c = remove("get_usr_name")) == -1)
  {
    perror("remove error");
    exit(1);
  }
  return user_name;
}

void history(char buf[])
{
  if (strcmp(buf, "history") == 0)
  {
    int his_fd1;
    int n;
    int i = 1;
    char read_buf[255];

    his_fd1 = open("history", O_RDONLY);
    while (n = read(his_fd1, read_buf, 255) > 0)
    {
      char *ptr = strtok(read_buf, "\n");
      while (ptr != NULL)
      {
        printf("\t\t%d\t%s\n", i, ptr);
        ptr = strtok(NULL, "\n");
        i++;
      }
    }
    close(his_fd1);
  }
}
/* 공백제거함수 */
// void erase_space(char *buf) {
//   char *a = buf;
//   int b = 0;

//   while (*buf != 0) {
//     if (*buf != ' ') {
//       *a = *buf;
//       a++;
//     }
//     buf++;
//   }
//   *a = 0;
// }

/* 토근화 실패 */
// for(int i;i<15;i++){
//   printf("%s\n",buf1[i]);
// }
// char *sear = strchr(buf, ';');
// if (sear != NULL) {
//   printf("%s\n", sear);
//   sear = strtok(NULL, ";");
// }
//    char *mode = strtok(buf, "><|&");
//   printf("%s\n",mode);
// if(mode==NULL){
// 	m=10;
// }
// mode = strchr(buf,'>');
//    if (mode != NULL) {
// 	printf("%s\n",mode);
//      if (*(mode + 1) == '>') {
//        m = 1;
// 	char *mode = strtok(buf,">>");
//      } else if (*(mode + 1) == '|') {
//        m = 2;
// 	char *mode = strtok(buf,">|");
//      }
//    }
// mode = strchr(buf, '<');
//   if(mode !=NULL){
// 	  m=3;
//   }
// printf("%d\n",m);
// printf("%ld\n", strlen(a));
// execlp("cat","cat","history",NULL);
//  for(int i=0;i<strlen(a);i++){

//  }