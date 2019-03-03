#include <stdio.h>
#include <string.h>

int toLower(int a);

int main(int argc ,char ** argv){
	int a;
	int b = 0;
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-D") ==0 ){
			b=1;
		}
	}

	while ((a = fgetc(stdin)) != EOF){
	fprintf(stdout, "%c", (toLower(a)));
		if(b == 1){
	fprintf(stderr, "0x%X             0x%X\n", a, toLower(a));		
	}
	}
	fprintf(stdout, "%s", "^D");

}

int toLower(int a) {
	if ((a <= 90) & (a >=65)){
		return a+32;
	}
	else{
		return a;
	}
}