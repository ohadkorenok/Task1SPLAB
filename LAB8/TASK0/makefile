task : main.o
	gcc -m32 -o task main.o  
main.o : main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
	
clean :
	rm task *.o
