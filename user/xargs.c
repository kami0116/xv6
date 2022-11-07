#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"
/*
xargs语法：xargs [-n max-args] cmd [cmdarg...]
*/
#define _SC_ARG_MAX 0X1000

void xargs(int argc, char *argv[])
{
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (pid) { // parent
        wait(0);
    } else { // child
        exec(argv[0], argv);
        exit(0);
    }
}

int main(int argc, char *argv[])
{

    char *argv2[MAXARG];
    char *bbp, *ebp, *p, *param;
    bbp = malloc(_SC_ARG_MAX);
    p = bbp;
    param=p;
    ebp = bbp + _SC_ARG_MAX;
    int argv_i = 1, argc2 = 0;
    int n = MAXARG;
    if (strcmp(argv[1], "-n") == 0) {
        n = atoi(argv[2]);
        if (n < 1) {
            printf("xargs: value %s for -n option should be >= 1", argv[2]);
            exit(1);
        }
        argv_i = 3;
    }
    // 从xargs的参数中获得子任务的命令和参数
    for (; argv_i < argc; argv_i++) {
        argv2[argc2++] = argv[argv_i];
    }
    int argc2_bk = argc2;
    // for (int i=0;i<argc2;i++)
    //   printf("%s ", argv2[i]);
    // printf("\nargc2=%d\n", argc2);
    // 从stdin解析参数
    char ch;
    char quote = 0;
    while (1) {
        int size=read(0,&ch, 1);
        if (size<0)
          exit(1);
        else if (size==0)
          goto over;
        
        switch (ch) {
        case '\'':
          if (quote == '"') 
            goto addch;
          if (quote=='\'')
            quote=0;
          else
            quote='\''; 
          break; 
        case '"':
          if (quote=='\'')
            goto addch;
          if (quote=='"')
            quote=0;
          else
            quote='"';
          break;
        case ' ':
        case '\n':
        case '\t':
          if (quote)
            goto addch;
        over:
          if (*param){
            *p++=0;
            argv2[argc2++]=param;
            if ((argc2-argc2_bk)==n || !size){
              xargs(argc2, argv2);
              p=bbp;
              argc2=argc2_bk;
            }
          }else if (!size){
            xargs(argc2, argv2);
            exit(0);
          }

          if (p<ebp){
            *p=0;
            param=p;
          } else{
            exit(1);
          }
          break;
        default:
          addch:
          if (p < ebp-1) {
              *p++ = ch;
              break;
          }else{
            exit(1);
          }
          break;
        }
    }
}