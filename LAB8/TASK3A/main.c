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

    void (*fun)(state *s); /* setting the state as a pointer allows the functions to change it.*/

};

state *progState = NULL;
void *map_start = NULL; /* will point to the start of the memory mapped file */


void printDebug(char *string) {
    if (progState->debug_mode == '1') {
        if (strncmp(string, "builtin", 7) == 0) {
            printf("Unit size: %d\tFileName: %s\tMemCount: %d\n", progState->unit_size, progState->file_name,
                   (int) progState->mem_count);

        } else {
            printf("%s", string);
        }
    }
}

void quit(state *s) {
    printDebug("quitting\n");
    exit(0);
}

void TDM(state *s) {
    printf("%c\n", s->debug_mode);
    if (s->debug_mode == '1') {
        s->debug_mode = '0';
        printf("debug mode is now off\n");
    } else {
        s->debug_mode = '1';
        printf("debug mode is now on\n");
    }

}

void EEF(state *s) {
    int fd;
    Elf32_Ehdr *header; /* this will point to the header structure */
    struct stat fd_stat; /* this is needed to  the size of the file */
    char name[100];
    printf("Please Enter an ELF file name:");
    fgets(name, 100, stdin);
    strtok(name, "\n");
    if ((fd = open(name, O_RDWR)) < 0) {
        perror("error in open");
        exit(-1);
    }
    if (fstat(fd, &fd_stat) != 0) {
        perror("stat failed");
        exit(-1);
    }
    if ((map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("mmap failed");
        exit(-4);
    }
    header = (Elf32_Ehdr *) map_start;
    printf("%c", header->e_ident[EI_MAG1]);
    printf("%c", header->e_ident[EI_MAG2]);
    printf("%c\n", header->e_ident[EI_MAG3]);
    switch (header->e_ident[EI_DATA]) {
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

    printf("0x%0X\n", header->e_entry);

    printf("0x%0X\n", header->e_shoff);
    printf("0x%0X\n", header->e_shentsize);
    printf("0x%0X\n", header->e_shnum);
    printf("0x%0X\n", header->e_phoff);
    printf("0x%0X\n", header->e_phentsize);
    printf("0x%0X\n", header->e_phnum);

}

char *intToType(int n) {
    char *toReturn = "";
    switch (n) {
        case 0:
            toReturn = "SHT_NULL";
            break;
        case 1:
            toReturn = "SHT_PROGBITS";
            break;
        case 2:
            toReturn = "SHT_SYMTAB";
            break;
        case 3:
            toReturn = "SHT_STRTAB";
            break;
        case 4:
            toReturn = "SHT_RELA";
            break;
        case 5:
            toReturn = "SHT_HASH";
            break;
        case 6:
            toReturn = "SHT_DYNAMIC";
            break;
        case 7:
            toReturn = "SHT_NOTE";
            break;
        case 8:
            toReturn = "SHT_NOBITS";
            break;
        case 9:
            toReturn = "SHT_REL";
            break;
        case 10:
            toReturn = "SHT_SHLIB";
            break;
        case 11:
            toReturn = "SHT_DYNSYM";
            break;
        default:
            toReturn = "noType";
            break;

    }
    return toReturn;
};

void PSN(state *s) {
    Elf32_Ehdr *header;
    Elf32_Shdr *sectionHeaderTable;
    char *sectionStringTable;
    Elf32_Shdr *sectionStringTableHeader;
    if (map_start == NULL) {
        fprintf(stderr, "NO ELF FILE EXAMINED");
        exit(1);
    }

    header = (Elf32_Ehdr *) map_start;
    int numberOfSections = header->e_shnum;
    sectionHeaderTable = (Elf32_Shdr *) (map_start + (header->e_shoff));
    sectionStringTableHeader = (Elf32_Shdr *) (sectionHeaderTable + (header->e_shstrndx));
    sectionStringTable = (char *) (map_start + sectionStringTableHeader->sh_offset);
    for (int i = 0; i < numberOfSections; ++i) {
        char *name = (sectionStringTable + (sectionHeaderTable[i].sh_name));
        printf("[%d] %s %p %0X %0X %s\n",i,name, (void *)sectionHeaderTable[i].sh_addr, sectionHeaderTable[i].sh_offset, sectionHeaderTable[i].sh_size, intToType(sectionHeaderTable[i].sh_type));

    }
//  printf("%0x\n",sectionHeaderTable->sh_size);


}
void PS(state *s){
    Elf32_Ehdr *header;
    Elf32_Shdr *sectionHeaderTable;
    char *sectionStringTable;
    Elf32_Shdr *sectionStringTableHeader;
    char *sectionSymbolStringTable;
    if (map_start == NULL) {
        fprintf(stderr, "NO ELF FILE EXAMINED");
        exit(1);
    }
    header = (Elf32_Ehdr *) map_start;
    sectionHeaderTable = (Elf32_Shdr *) (map_start + (header->e_shoff));
    sectionStringTableHeader = (Elf32_Shdr *) (sectionHeaderTable + (header->e_shstrndx));
    sectionStringTable = (char *) (map_start + sectionStringTableHeader->sh_offset);
    int numberOfSections = header->e_shnum;
    for(int i = 0; i <numberOfSections;i++){
        if((sectionHeaderTable[i].sh_type)==SHT_SYMTAB) {
            sectionSymbolStringTable = (char *) (map_start+(sectionHeaderTable[sectionHeaderTable[i].sh_link].sh_offset));
            int numberOfEntries = sectionHeaderTable[i].sh_size/(sizeof(Elf32_Sym));
            Elf32_Sym *symTable = (Elf32_Sym *) (map_start+sectionHeaderTable[i].sh_offset);
            for(int i = 0;i<numberOfEntries;i++){
                char *symbolName = (sectionSymbolStringTable + (symTable[i].st_name));
                if(symTable[i].st_shndx >= 0xff00){

                    printf("[%d] %p %d %s  %s\n", i, (void *)symTable[i].st_value, symTable[i].st_shndx,"ABS",symbolName);
                    // printf("[%d] %p %d %s \n", i, (void *)symTable[i].st_value, symTable[i].st_shndx,"ABS");

//                    printf("[%d] %p %d %s  %s\n", i, 0, symTable[i].st_shndx,"ABS",symbolName);

                }
                else{
                    char *sectionName = (sectionStringTable + (sectionHeaderTable[symTable[i].st_shndx].sh_name));
                    printf("[%d] %p %d %s  %s\n", i, (void *)symTable[i].st_value, symTable[i].st_shndx,sectionName,symbolName);
                    //           printf("[%d] %p %d %s  \n", i, (void *)symTable[i].st_value, symTable[i].st_shndx,sectionName);

                }
            }
        }
    }
}

void LC(state * s){
quit(s);
};

void RT(state *s){
    Elf32_Ehdr *header;
    Elf32_Shdr *sectionHeaderTable;
    Elf32_Rel *sectionRel;
    header = (Elf32_Ehdr *) map_start;
    sectionHeaderTable = (Elf32_Shdr *) (map_start + (header->e_shoff));
    int numberOfSections = header->e_shnum;
    for(int i = 0; i <numberOfSections;i++){
        if((sectionHeaderTable[i].sh_type)==SHT_REL){
            printf("OFFSET         INFO\n");
            sectionRel = (Elf32_Rel *) (map_start + (sectionHeaderTable[i].sh_offset));
            int numberOfEntries = sectionHeaderTable[i].sh_size/(sizeof(Elf32_Rel));
            for(int i = 0; i < numberOfEntries; i++){

                    printf("%0X         %0X\n", sectionRel[i].r_offset,sectionRel[i].r_info);
            }
        }
    }

};
struct fun_desc menu[] = {{"Toggle Debug Mode",   TDM},
                          {"Examine ELF File",    EEF},
                          {"Print Section Names", PSN},
                          {"Print Symblos",PS},
                          {"Link Check", LC},
                          {"Relocation Tables", RT},
                          {"quit", quit},
                          {NULL, NULL}};


int main(int argc, char **argv) {
    progState = malloc(sizeof(state));
    progState->debug_mode = '0';
    char name[1];
    int chosenFun;
    int menuSize = sizeof(menu) / (sizeof(struct fun_desc));
    while (1) {
        printf("Please choose a function:\n");
        printDebug("builtin");
        int i;
        for (i = 0; i < menuSize - 1; i++) {
            printf("%d) ", i);
            printf("%s\n", menu[i].name);
        }
        printf("type:");
        fgets(name, 100, stdin);
        chosenFun = atoi(name);
        if ((chosenFun >= 0) && (chosenFun < menuSize))
            ((menu[chosenFun]).fun)(progState);
        else {
            perror("ERROR: ARguemnts are not Valid \n");
        }
    }
}



