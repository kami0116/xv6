#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


void 
find(char *dir, char *pattern){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

	if((fd = open(dir, 0)) < 0){
		fprintf(2, "find: cannot open %s\n", dir);
		return;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "find: cannot stat %s\n", dir);
		close(fd);
		return;
	}
	
	if (st.type!=T_DIR){
		fprintf(2, "find: not a directory %s\n", dir);
	}

	if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
		printf("find: path too long\n");
	}


	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if (de.name[0]=='.')
			continue;
		if(de.inum == 0)
			continue;
		strcpy(buf, dir);
		p = buf+strlen(buf);
		*p++ = '/';
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if (strcmp(de.name, pattern)==0){
			printf("%s\n", buf);
		}
		if(stat(buf, &st) < 0){
			printf("find: cannot stat %s\n", buf);
			continue;
		}
		if (st.type==T_DIR){
			find(buf, pattern);
		}
	}
	close(fd);
}

int
main(int argc, char *argv[])
{   
    if (argc != 3) {
        printf("语法错误, 请输入find directory filename\n");
        exit(1);
    }
	// printf("进入find程序, dir=%s, pattern=%s\n", argv[1], argv[2]);
	find(argv[1], argv[2]);
	exit(0);
}