all: task2Project
task2Project: task2.o
		gcc -m32 -g -Wall -o task2 task2.o
task2.o: task2.c
		gcc -g -Wall -m32 -c -o task2.o task2.c
#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task2Project