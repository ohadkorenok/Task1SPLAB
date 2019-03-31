all: task1Project
task1Project: task1.o start.o util.o
		ld -m elf_i386 start.o task1.o util.o -o task1
task1.o: task1.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task1.c -o task1.o
util.o: util.h util.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
start.o: start.s
		nasm -f elf start.s -o start.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task1Project