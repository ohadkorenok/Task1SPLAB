#include "util.h"

#define SYS_GETDENTS 141
#define SYS_LSEEK 19
#define SYS_CLOSE 6
#define SYS_OPEN 5
#define SYS_WRITE 4
#define STDOUT 1

struct linux_dirent {
 unsigned  long   d_ino;
   unsigned long  d_off;
   unsigned short d_reclen;
   char           d_name[];
};
int debug = 0;
int prefix_flag =0;
char* prefix;
int prefix_leng;
int main (int argc , char* argv[], char* envp[])
{

	int i=0;
for (;i < argc; ++i) {
    if (strcmp(argv[i], "-D") == 0) {
      debug = 1;
    }

        if (strncmp(argv[i], "-p", 2) == 0) {
      prefix_flag = 1;
      prefix = argv[i]+2;
      prefix_leng = strlen(prefix);
system_call(SYS_WRITE,STDOUT,prefix,prefix_leng);

    }	
    }

char buf[8192];
struct linux_dirent *d;
int curr_dir = system_call(SYS_OPEN,".", 0,0644);
if(curr_dir == -1){
	system_call(1,0,0,0);
}
int a = system_call(SYS_GETDENTS,curr_dir, buf,8192);
if(a == -1){
	system_call(1,0,0,0);
}
int b;
for (b = 0; b<a;){
d = (struct linux_dirent *) (buf + b);
if(prefix_flag == 1 && strncmp(d->d_name, prefix, prefix_leng) ==0){
system_call(SYS_WRITE,STDOUT,"The file name is: ",18);
system_call(SYS_WRITE, STDOUT, d->d_name, strlen(d->d_name));
system_call(SYS_WRITE,STDOUT,"   and the size is: ",20);
system_call(SYS_WRITE, STDOUT, itoa(d->d_reclen), strlen(itoa(d->d_reclen)));

system_call(SYS_WRITE,STDOUT,"\n",1);
}


b=b+d->d_reclen;
}
system_call(SYS_CLOSE,a);
  return 0;
}
