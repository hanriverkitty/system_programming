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
// void erase_space(char *a);
char *dir;
void history(char buf[]);
int is_pipe = 0;
int leng = 1;
int fd;
char num[255];

int main(void)
{
  int g;
  char *user = get_usr_name();
  int his_exe = 1;
  int back = 0;

  while (1)
  {
    int cn_semi = 0;
    back = 0;
    char *buf1[20];
    int history1 = 1;
    int cd1 = 1;
    int pid;
    leng = 0;
    char buf[255];
    is_pipe = 0;
    int p_index;
    // printf("%s", num);
    if (his_exe == 1)
    {
      printf("%s:%s$ ", user, getcwd(NULL, 0));
      fgets(buf, 255, stdin);
    }
    else
    {

      his_exe = 1;
    }

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

    // if (strcmp(buf, "history") == 0)
    // {
    //   int his_fd1;
    //   int n;
    //   int i = 1;
    //   char read_buf[255];

    //   his_fd1 = open("history", O_RDONLY);
    //   while (n = read(his_fd1, read_buf, 255) > 0)
    //   {
    //     char *ptr = strtok(read_buf, "\n");
    //     while (ptr != NULL)
    //     {
    //       printf("\t\t%d\t%s\n", i, ptr);
    //       ptr = strtok(NULL, "\n");
    //       i++;
    //     }
    //   }
    //   close(his_fd1);
    // }

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
          is_pipe = 1;
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
        back = 1;
        break;
      case ';':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], ";");
        leng++;
        cn_semi++;
        break;
      case '!':
        if (strcmp(buf1[leng], "") != 0)
          leng++;
        strcat(buf1[leng], "!");
        leng++;
        break;
      case '2':
        if (a[i + 1] == '>')
        {
          if (strcmp(buf1[leng], "") != 0)
            leng++;
          strcat(buf1[leng], "2>");
          leng++;
          i++;
        }
        else
        {
          strncat(buf1[leng], &buf[i], 1);
        }
        break;
      default:
        strncat(buf1[leng], &buf[i], 1);
        break;
      }
    }
    g = 0;
    int temp;
    // printf("%s\n", buf1[0]);
    // printf("%s\n", buf1[1]);
    // for (int i = 0; i < 3; i++)
    // {

    // printf("%d\n", temp);
    // printf("%d\t\ti\n", i);
    switch (is_pipe)
    {
    case 1:
    {

      int p_fd[2];
      pipe(p_fd);
      printf("1\n");
      int index = 0;
      int s_index = 0;
      p_index = 0;
      char *exe[10];
      char *s_exe[10];
      for (int i = 0; i < 10; i++)
      {
        exe[i] = '\0';
        s_exe[i] = '\0';
      }
      char first_exe[10];
      char second_exe[10];
      int p_cmd_c = 0;
      /* 명령문 나누기 */
      while (g <= leng)
      {
        if (strcmp(buf1[g], "|") == 0)
        {
          p_cmd_c = 1;
          g++;
        }
        else
        {
          if (p_cmd_c == 0)
          {
            if (index == 0)
            {
              strcpy(first_exe, buf1[g]);
              exe[index] = buf1[g];
              index++;
            }
            else
            {
              exe[index] = buf1[g];
              index++;
            }
          }
          else
          {
            if (s_index == 0)
            {
              strcpy(second_exe, buf1[g]);
              s_exe[s_index] = buf1[g];
              s_index++;
            }
            else
            {
              s_exe[index] = buf1[g];
              s_index++;
            }
          }
          g++;
        }
      }
      printf("first\t%s\t%s\n", first_exe, exe[0]);
      printf("second\t%s\t%s\n", second_exe, s_exe[0]);
      if (fork() == 0)
      {
        dup2(p_fd[1], 1);
        close(p_fd[0]);
        close(p_fd[1]);
        execvp(first_exe, exe);
      }
      if (fork() == 0)
      {
        dup2(p_fd[0], 0);
        close(p_fd[0]);
        close(p_fd[1]);
        execvp(second_exe, s_exe);
      }
      close(p_fd[0]);
      close(p_fd[1]);
      wait(NULL);
      wait(NULL);

      break;
    }
    /* 파이프가 아닐때 */
    default:
      /* fork 및 명령어 실행*/
      switch (pid = fork())
      {
      case -1:
        perror("fork error");
        break;
        exit(1);
      case 0:
      {
        /* 세미콜론 개수에 따른 for 문 */
        for (int i = 0; i <= cn_semi; i++)
        {
          int index = 0;
          char *exe[20];
          for (int i = 0; i < 20; i++)
          {
            exe[i] = '\0';
          }
          char first_exe[10];
          /* 명령문 나누기 */
          while (g <= leng)
          {
            if (strcmp(buf1[g], ">") == 0)
            {
              if ((fd = open(buf1[g + 1], O_CREAT | O_RDWR, 0666)) < 0)
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
            else if (strcmp(buf1[g], ">|") == 0)
            {
              if ((fd = open(buf1[g + 1], O_CREAT | O_RDWR | O_TRUNC, 0666)) < 0)
              {
                perror("open error");
                exit(1);
              }
              dup2(fd, 1);
              close(fd);
              g += 2;
            }
            /* history */
            else if (strcmp(buf1[g], "history") == 0)
            {
              // printf("%s\n", buf1[g]);
              history(buf1[g]);
              history1 = 0;
              g++;
            }
            else if (strcmp(buf1[g], ";") == 0)
            {
              printf("semicolon\n");
              g++;
              temp++;
              break;
            }
            /* ! */
            else if (strcmp(buf1[g], "!") == 0)
            {
              // printf("%s\n", buf1[g + 1]);
              char line[255];
              char *pLine;
              int c_line = 1;
              FILE *in = fopen("history", "r");
              while (!feof(in))
              {

                pLine = fgets(line, 255, in);
                char c_buf[10];
                sprintf(c_buf, "%d", c_line);
                if (strcmp(buf1[g + 1], c_buf) == 0)
                {
                  strcpy(buf, pLine);
                  // printf("%s", buf);
                  his_exe = 0;
                  break;
                }
                c_line++;
              }
              fclose(in);
              g += 2;
              if (his_exe == 0)
              {
                break;
              }
            }
            /* cd */
            else if (strcmp(buf1[g], "cd") == 0)
            {
              chdir(buf1[g + 1]);
              g += 2;
              cd1 = 0;
            }

            /* < */
            else if (strcmp(buf1[g], "<") == 0)
            {
              if ((fd = open(buf1[g + 1], O_RDWR, 0666)) < 0)
              {
                perror("open error");
                exit(1);
              }
              dup2(fd, 0);
              close(fd);
              g += 2;
            }
            /* 2> */
            else if (strcmp(buf1[g], "2>") == 0)
            {
              if ((fd = open(buf1[g + 1], O_CREAT | O_RDWR | O_APPEND, 0666)) < 0)
              {
                perror("open error");
                exit(1);
              }
              dup2(fd, 2);
              close(fd);
              g += 2;
            }
            else if (strcmp(buf1[g], "&") == 0)
            {
              back = 1;
              break;
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
          if (his_exe == 0 || history1 == 0 || cd1 == 0)
          {
            break;
          }
          // printf("%s\t\t%s\n", first_exe, exe[0]);
          // printf("%s\t\t%s\n", first_exe, exe[1]);
          // printf("%s\t\t%s\n", first_exe, exe[2]);
          /* 세미콜론이 있을 때 */
          if (temp > 0)
          {
            switch (fork())
            {
            case 0:
              execvp(first_exe, exe);

            default:
              wait(NULL);
              temp = 0;
            }
          }
          else
          {
            execvp(first_exe, exe);
          }
          // execvp(first_exe, exe);
          if (temp == cn_semi)
          {
            exit(1);
          }
        }
        break;
        exit(1);
      }
      default:
        if (back)
        {

          printf("%d\n", getpid());
          back = 0;
          break;
        }

        else
        {
          int st, st1;
          st1 = waitpid(pid, &st, 0);
          break;
          // printf("front\n");
        }
      }
    }
    //}
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
//  void erase_space(char *buf) {
//    char *a = buf;
//    int b = 0;

//    while (*buf != 0) {
//      if (*buf != ' ') {
//        *a = *buf;
//        a++;
//      }
//      buf++;
//    }
//    *a = 0;
//  }

/* 토큰화 실패 */
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