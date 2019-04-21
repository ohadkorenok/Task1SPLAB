all: task0bProject
task0bProject: looper.o
		gcc -m32 -Wall -g -o looper looper.o 
looper.o: looper.c
		gcc -m32 -Wall -g -c -o looper.o looper.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o looper

