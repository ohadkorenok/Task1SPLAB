#include "util.h"

#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19



int encrypta(int a, int b, int sum); /* b>0 add, b <0 substract b=0 nothing at all */
char* toLower(char* a);
  int outputStream = STDOUT;
int main(int argc ,char ** argv) {
  char a[1];
  char *t = a;
  int b = 0;
  int c = 0;
  int d = 0;
  int debug = 0;
  char *toPrint = a;
  int fp = STDIN;

  int i=0;  
  int systemCall;
  for (;i < argc; ++i) {
    if (strcmp(argv[i], "-D") == 0) {
      debug = 1;
    } else {

      if (strncmp(argv[i], "-i", 2) == 0) {
        c=1;
        char *filename = argv[i] + 2;

                  if(debug==1){
      system_call(SYS_WRITE, STDERR, "The action: \n", 13);
      system_call(SYS_WRITE, STDERR, itoa(SYS_OPEN), 1);
      system_call(SYS_WRITE, STDERR, filename, 15);
        fp = system_call(SYS_OPEN, filename, 2, 0644);
      system_call(SYS_WRITE, STDERR, "\n The Response: \n", 17);
      system_call(SYS_WRITE, STDERR, itoa(fp), 1);
          }
          else{
        fp = system_call(SYS_OPEN, filename, 2, 0644);

          }
        if (!fp) {
          systemCall = system_call(SYS_WRITE, STDOUT, "Error! cannot open file for reading! \n ", 38);
          system_call(1, 0, 0, 0);
          if(debug==1){
      system_call(SYS_WRITE, STDERR, "The action: ", 13);
      system_call(SYS_WRITE, STDERR, SYS_WRITE, 1);
      system_call(SYS_WRITE, STDERR, "The Response: ", 17);
      system_call(SYS_WRITE, STDERR, systemCall, 1);
          }
        }
      } else {
        if (strncmp(argv[i], "-o", 2) == 0) {
          d=1;
          char *output = argv[i] + 2;

 if(debug==1){
      system_call(SYS_WRITE, STDERR, "The action: ", 13);
      system_call(SYS_WRITE, STDERR, itoa(SYS_OPEN), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
      system_call(SYS_WRITE, STDERR, output, 15);
        outputStream = system_call(SYS_OPEN, output, 64, 0644);
      system_call(SYS_WRITE, STDERR, "\n The Response: \n", 17);
      system_call(SYS_WRITE, STDERR, itoa(outputStream), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);


system_call(SYS_WRITE, STDERR, "The action: ", 13);
      system_call(SYS_WRITE, STDERR, itoa(SYS_OPEN), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
      system_call(SYS_WRITE, STDERR, output, 15);
        outputStream = system_call(SYS_OPEN, output, 2, 0644);
      system_call(SYS_WRITE, STDERR, "\n The Response: \n", 17);
      system_call(SYS_WRITE, STDERR, itoa(outputStream), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
          }
          else{
        outputStream = system_call(SYS_OPEN, output, 64, 0644);
        outputStream = system_call(SYS_OPEN, output, 1, 0644);

          }
          if (outputStream==STDOUT) {
          systemCall = system_call(SYS_WRITE, STDOUT, "Error! cannot open file for reading! \n ", 38);
          system_call(1, 0, 0, 0);
          if(debug==1){
      system_call(SYS_WRITE, STDERR, "output file Opened \n", 13);
      system_call(SYS_WRITE, STDERR, "The action: \n", 13);
      system_call(SYS_WRITE, STDERR, SYS_WRITE, 1);
      system_call(SYS_WRITE, STDERR, "\n The Response: \n", 17);
      system_call(SYS_WRITE, STDERR, systemCall, 1);
         }
        }
          }
        }
      }
    }
    if (debug == 1) {
      system_call(SYS_WRITE, STDERR, "The action: ", 13);
      system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
  systemCall = system_call(SYS_READ, fp, a, 1);
      system_call(SYS_WRITE, STDERR, "The Response:", 15);
      system_call(SYS_WRITE, STDERR, itoa(systemCall), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
    else{
  systemCall = system_call(SYS_READ, fp, a, 1);
    }
  while ((systemCall == 1)) {
    *toPrint = toLower(*a);
    systemCall = system_call(SYS_WRITE, outputStream, toPrint, 1);
    if (debug == 1) {
      system_call(SYS_WRITE, STDERR, "The action: ", 12);
      system_call(SYS_WRITE, STDERR, itoa(SYS_WRITE), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
      system_call(SYS_WRITE, STDERR, "The Response: ", 13);
      system_call(SYS_WRITE, STDERR, itoa(systemCall), 1);
      system_call(SYS_WRITE, STDERR, "\n", 1);
    }
    systemCall = system_call(SYS_READ, fp, a, 1);
        if (debug == 1) {
    system_call(SYS_WRITE, STDERR, "The action: ", 12);
    system_call(SYS_WRITE, STDERR, itoa(SYS_READ), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);
    system_call(SYS_WRITE, STDERR, "The Response: ", 13);
    system_call(SYS_WRITE, STDERR, itoa(systemCall), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);

    }

  }
  if(c==1){
       systemCall = system_call(SYS_CLOSE,a);
    if(debug == 1){
    system_call(SYS_WRITE, STDERR, "The action: ", 12);
    system_call(SYS_WRITE, STDERR, itoa(SYS_CLOSE), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);
    system_call(SYS_WRITE, STDERR, "The Response: ", 13);
    system_call(SYS_WRITE, STDERR, itoa(systemCall), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);

      }  
  }
    if(d==1){
       systemCall = system_call(SYS_CLOSE,outputStream);
    if(debug == 1){
    system_call(SYS_WRITE, STDERR, "Closing the output file!  ", 12);
    system_call(SYS_WRITE, STDERR, "The action: ", 12);
    system_call(SYS_WRITE, STDERR, itoa(SYS_CLOSE), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);
    system_call(SYS_WRITE, STDERR, "The Response: ", 13);
    system_call(SYS_WRITE, STDERR, itoa(systemCall), 1);
    system_call(SYS_WRITE, STDERR, "\n", 1);
    system_call(SYS_WRITE, STDERR, "Output file closed! ", 12);

      }  
  }


  return 0;
}
int encrypta(int a, int b, int sum) {
  if(b > 0){
    return (a + sum) % 128;
  }
  else{
    return (a - sum) % 128;
  }
}

char* toLower(char* a) {
  if ((a <= 90) & (a >=65)){
    return a+32;
  }
  else{
    return a;
  }
}
