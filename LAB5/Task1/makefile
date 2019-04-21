all: task0project
task0project: myshell.o LineParser.o
		gcc -m32 -Wall -g -o myshell myshell.o LineParser.o 
myshell.o: myshell.c
		gcc -m32 -Wall -g -c -o myshell.o myshell.c 
LineParser.o: LineParser.c LineParser.h
		gcc -m32 -Wall -g -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o myshell

