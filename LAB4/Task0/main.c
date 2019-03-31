#include "util.h"

#define SYS_LSEEK 19
#define SYS_CLOSE 6
#define SYS_OPEN 5
#define SYS_WRITE 4
#define STDOUT 1

int main (int argc , char* argv[], char* envp[])
{
  int a = system_call(SYS_OPEN,"greeting.txt", 2,0644);
  system_call(SYS_WRITE,STDOUT, "File have been opened",19);
  int b = system_call(SYS_LSEEK,a,657, 0); 
  int c = system_call(SYS_WRITE,a, "Mika.",6 );
  int d = system_call(SYS_CLOSE,a); 
  system_call(SYS_WRITE,STDOUT, "File have been closed",19);

  return 0;
}
