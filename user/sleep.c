#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
  if(argc != 2){
      printf("请用正确的格式, sleep n\n");
      exit(1);
  }
  int sec = atoi(argv[1]);
  return sleep(sec);
}
