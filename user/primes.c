#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int fd[2];
    pipe(fd);
    int pid=fork();
    if (pid < 0) {
      exit(1);
    }else if (pid){//parent
        close(fd[0]);
        for(int i=2;i<=35;i++)
            write(fd[1], &i, 4);
        close(fd[1]);
        wait(0);
    }else{//child
        close(fd[1]);
        int i; 
        while(read(fd[0], &i, 4)>0){
            int is_prime=1;
            for(int j=2;j<=i/2;j++){
                if (i%j==0){
                    is_prime=0;
                    break;
                }
            }
            if (is_prime)
                printf("prime %d\n", i);
        }
        
        close(fd[0]);
    }
    return 0;
}