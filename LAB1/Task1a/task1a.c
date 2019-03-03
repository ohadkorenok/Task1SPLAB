#include <stdio.h>
#include <string.h>

int toLower(int a);

int main(int argc ,char ** argv){
	int a;
	while ((a = fgetc(stdin)) != EOF){
	fprintf(stdout, "%c", (toLower(a)));
}
}

int toLower(int a) {
	if ((a <= 90) & (a >=65)){
		return a+32;
	}
	else{
		return a;
	}
}