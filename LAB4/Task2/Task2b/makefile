all: task2bProject
task2bProject: task2b.o start.o util.o
		ld -m elf_i386 start.o task2b.o util.o -o task2b
task2b.o: task2b.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2b.c -o task2b.o
util.o: util.h util.c
		gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
start.o: start.s
		nasm -f elf start.s -o start.o

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o task2bProject
