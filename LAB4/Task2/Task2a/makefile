all: task2aProject
task2aProject: task2a.o start.o util.o
		ld -m elf_i386 start.o task2a.o util.o -o task2a
task2a.o: task2a.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2a.c -o task2a.o
util.o: util.h util.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
start.o: start.s
		nasm -f elf start.s -o start.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task2aProject