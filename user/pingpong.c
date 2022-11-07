#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2];
  int p2[2];
  pipe(p1);
  pipe(p2);
  char buf[10];

  int pid = fork();
  if (pid < 0) {
      exit(1);
  } else if (pid == 0) { // child
      close(p1[0]);
      close(p2[1]);
      read(p2[0], buf, 10);
      write(p1[1], "pong", 4);
      printf("%d: received %s\n", getpid(), buf);
      close(p1[1]);
      close(p2[0]);
  } else { // parent
      close(p2[0]);
      close(p1[1]);
      write(p2[1], "ping", 4);
      read(p1[0], buf, 4);
      wait(0);
      printf("%d: received %s\n", getpid(), buf);
      close(p2[1]);
      close(p1[0]);
  }
  exit(0);
}

