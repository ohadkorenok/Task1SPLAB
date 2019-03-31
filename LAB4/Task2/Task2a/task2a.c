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
int main (int argc , char* argv[], char* envp[])
{
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
system_call(SYS_WRITE,STDOUT,"The file name is: ",18);
system_call(SYS_WRITE, STDOUT, d->d_name, strlen(d->d_name));
system_call(SYS_WRITE,STDOUT,"   and the size is: ",20);
system_call(SYS_WRITE, STDOUT, itoa(d->d_reclen), strlen(itoa(d->d_reclen)));

system_call(SYS_WRITE,STDOUT,"\n",1);



b=b+d->d_reclen;
}
system_call(SYS_CLOSE,a);
  return 0;
}
