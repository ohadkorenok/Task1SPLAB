#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>       
#include <unistd.h>



typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc {
  char *name;
  void (*fun)(state* s); /* setting the state as a pointer allows the functions to change it.*/

};

state *progState = NULL;
void printDebug(char * string){
    if(progState->debug_mode=='1'){
        if(strncmp(string, "builtin", 7) ==0){
            printf("Unit size: %d\tFileName: %s\tMemCount: %d\n",progState->unit_size, progState->file_name, (int)progState->mem_count);

        }
        else{
            printf("%s",string);
        }
    }
}

void quit(state* s){
    printDebug("quitting\n");
	exit(0);
}

void TDM(state* s){
	printf("%c\n",s->debug_mode);
	if(s->debug_mode == '1'){
		s->debug_mode = '0';
		printf("debug mode is now off\n");
 }
 else{
		s->debug_mode = '1';
		printf("debug mode is now on\n");
 }
 
}
void EEF(state *s){
  int fd;
  Elf32_Ehdr *header; /* this will point to the header structure */
  void *map_start; /* will point to the start of the memory mapped file */
  int num_of_section_headers; /* will hold the number of the section headers */
  struct stat fd_stat; /* this is needed to  the size of the file */
  char name[100];
  printf("Please Enter an ELF file name:");
  fgets(name,100,stdin);
  strtok(name,"\n");
  if( (fd = open(name, O_RDWR)) < 0 ) {
      perror("error in open");
      exit(-1);
   }
  if( fstat(fd, &fd_stat) != 0 ) {
      perror("stat failed");
      exit(-1);
   }
    if ( (map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0)) == MAP_FAILED ) {
      perror("mmap failed");
      exit(-4);
   }
   
   header = (Elf32_Ehdr *) map_start;
   printf("%c",header->e_ident[EI_MAG1]);
   printf("%c",header->e_ident[EI_MAG2]);
   printf("%c\n",header->e_ident[EI_MAG3]);
   switch(header->e_ident[EI_DATA]){
    case 0:
      printf("ELFDATANONE\n");
      break;
    case 1:
      printf("ELFDATA2LSB\n");
      break;
    case 2:
      printf("ELFDATA2MSB\n");
      break;
    default:
      printf("ERROR WHILE PRINTING ENCODING SCHEME\n");
      break;    
   }

   printf("0x%0X\n",header->e_entry);
   
   printf("0x%0X\n",header->e_shoff);
   printf("0x%0X\n",header->e_shentsize);
   printf("0x%0X\n",header->e_shnum);
   printf("0x%0X\n",header->e_phoff);
   printf("0x%0X\n",header->e_phentsize);
   printf("0x%0X\n",header->e_phnum);

}


struct fun_desc menu[] = {{"Toggle Debug Mode",TDM},{"Examine ELF File",EEF},{"quit",quit},{NULL,NULL}};



int main(int argc, char **argv){
	 progState = malloc(sizeof(state));
	 progState->debug_mode = '0';
	 char name[1];
	 int chosenFun;
	 int menuSize = sizeof(menu)/(sizeof (struct fun_desc));
	 while(1){
     printf("Please choose a function:\n");
     printDebug("builtin");
     int i;
     for(i=0;i<menuSize-1;i++){
    	printf("%d) ",i);
    	printf("%s\n",menu[i].name);
	   }
	    printf("type:");
    	fgets(name,100,stdin);
   		chosenFun = atoi(name);
   		if((chosenFun>=0)&& (chosenFun<menuSize))
   			((menu[chosenFun]).fun)(progState);
   		else{
   			perror("ERROR: ARguemnts are not Valid \n");
   		}	
	  }
}



