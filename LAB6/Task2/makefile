all: task0bProject
task0bProject: main.o
		gcc -m32 -Wall -g -o main main.o 
main.o: main.c
		gcc -m32 -Wall -g -c -o main.o main.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o main

