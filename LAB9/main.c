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
struct stat fd_stat; /* this is needed to  the size of the file */

struct fun_desc {
    char *name;

    void (*fun)(state *s); /* setting the state as a pointer allows the functions to change it.*/

};

int debug = 0;
state *progState = NULL;
void *map_start = NULL; /* will point to the start of the memory mapped file */


void printDebug(char *string) {
    if (debug == 1) {
        printf(string);
    }
}


void quit(state *s) {
    printDebug("quitting\n");
    munmap(map_start, fd_stat.st_size);
    exit(0);
}

void TDM(state *s) {
    printf("%d\n", debug);
    if (debug == 1) {
        debug = 0;
        printf("debug mode is now off\n");
    } else {
        debug = 1;
        printf("debug mode is now on\n");
    }
}

void EEF(state *s) {
    int fd;
    Elf32_Ehdr *header; /* this will point to the header structure */
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


char *intoToSegType(int n) {
    char *toReturn = "";
    switch (n) {
        case 0:
            toReturn = "PT_NULL";
            break;
        case 1:
            toReturn = "PT_LOAD";
            break;
        case 2:
            toReturn = "PT_DYNAMIC";
            break;
        case 3:
            toReturn = "PT_INTERP";
            break;
        case 4:
            toReturn = "PT_NOTE";
            break;
        case 5:
            toReturn = "PT_SHLIB";
            break;
        case 6:
            toReturn = "PT_PHDR";
            break;
        case 0x70000000:
            toReturn = "PT_LOPROC";
            break;
        case 0x7fffffff:
            toReturn = "PT_HIPROC";
            break;
        default:
            toReturn = "noType";
            break;
    }

    return toReturn;
}

char *intToSecType(int n) {
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

        case 14:
            toReturn = "SHT_INIT_ARRAY";
            break;
        case 15:
            toReturn = "SHT_FINI_ARRAY";
            break;
        case 16:
            toReturn = "SHT_PREINIT_ARRAY";
            break;
        case 17:
            toReturn = "SHT_GROUP";
            break;
        case 18:
            toReturn = "SHT_SYMTAB_SHNDX";
            break;
        case 19:
            toReturn = "SHT_NUM";
            break;
        case 0x60000000:
            toReturn = "SHT_LOOS";
            break;
        case 0x6ffffff7:
            toReturn = "SHT_GNU_LIBLIST";
            break;
        case 0x6ffffff8:
            toReturn = "SHT_CHECKSUM";
            break;
        case 0x6ffffffa:
            toReturn = "SHT_LOSUNW";
            break;
        case 0x6ffffffb:
            toReturn = "SHT_SUNW_COMDAT";
            break;
        case 0x6ffffffc:
            toReturn = "SHT_SUNW_syminfo";
            break;
        case 0x6ffffffd:
            toReturn = "SHT_GNU_verdef";
            break;
        case 0x6ffffffe:
            toReturn = "SHT_GNU_verneed";
            break;
        case 0x6fffffff:
            toReturn = "SHT_GNU_versym";
            break;
        case 0x8fffffff:
            toReturn = "SHT_HIUSER";
            break;
        case 0x70000000:
            toReturn = "SHT_LOPROC";
            break;
        case 0x7fffffff:
            toReturn = "SHT_HIPROC";
            break;
        case 0x80000000:
            toReturn = "SHT_LOUSER";
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
    int i;
    if (debug == 1) {
        printf("shstrndx: ");
        printf("%d\n", header->e_shstrndx);
    }
    for (i = 0; i < numberOfSections; ++i) {
        char *name = (sectionStringTable + (sectionHeaderTable[i].sh_name));
        printf("[%d] %s %p %0X %0X %s\n", i, name, (void *) sectionHeaderTable[i].sh_addr,
               sectionHeaderTable[i].sh_offset, sectionHeaderTable[i].sh_size,
               intToSecType(sectionHeaderTable[i].sh_type));
        if (debug == 1) {
            printf("Section Name Index: %d\n", sectionHeaderTable[i].sh_name);
        }

    }


}

void PS(state *s) {
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
    int i;
    for (i = 0; i < numberOfSections; i++) {
        if ((sectionHeaderTable[i].sh_type) == SHT_SYMTAB) {
            sectionSymbolStringTable = (char *) (map_start +
                                                 (sectionHeaderTable[sectionHeaderTable[i].sh_link].sh_offset));
            int numberOfEntries = sectionHeaderTable[i].sh_size / (sizeof(Elf32_Sym));
            Elf32_Sym *symTable = (Elf32_Sym *) (map_start + sectionHeaderTable[i].sh_offset);
            int j;
            if (debug == 1) {
                printf("number of symbol table: %d\n", numberOfEntries);
                printf("size of symbol table: %d", sectionHeaderTable[i].sh_size);
            }
            for (j = 0; j < numberOfEntries; j++) {
                char *symbolName = (sectionSymbolStringTable + (symTable[j].st_name));
                if (symTable[j].st_shndx >= 0xff00) {

                    printf("[%d] %p %d %s  %s\n", j, (void *) symTable[j].st_value, symTable[j].st_shndx, "ABS",
                           symbolName);

                } else {
                    char *sectionName = (sectionStringTable + (sectionHeaderTable[symTable[j].st_shndx].sh_name));
                    printf("[%d] %p %d %s  %s\n", j, (void *) symTable[j].st_value, symTable[j].st_shndx, sectionName,
                           symbolName);
                }
            }
        }
    }
}

void LC(state *s) {
    quit(s);
};

void T0A(state *s) {
    Elf32_Ehdr *header;
    Elf32_Phdr *programHeaderTable;
    if (map_start == NULL) {
        fprintf(stderr, "NO ELF FILE EXAMINED");
        exit(1);
    }
    header = (Elf32_Ehdr *) map_start;
    programHeaderTable = (Elf32_Phdr *) (map_start + (header->e_phoff));
    printf("Type\t\t\t\tOffset\t\tVirtAddr\t\tPhysAddr\t\tFileSiz\t\tMemSiz\t\tFlg\t\t Align\n");
    int i;
    for (i = 0; i < header->e_phnum; ++i) {

        printf("%s %.8X %.8X %.8X %.8X %.8X %d %0X \n", intoToSegType(programHeaderTable[i].p_type),
               programHeaderTable[i].p_offset, programHeaderTable[i].p_vaddr, programHeaderTable[i].p_paddr,
               programHeaderTable[i].p_filesz, programHeaderTable[i].p_flags, programHeaderTable[i].p_align);
    }

};

void RT(state *s) {
    Elf32_Ehdr *header;
    Elf32_Shdr *sectionHeaderTable;
    Elf32_Rel *sectionRel;
    Elf32_Sym *dymSymbolTable;
    char *sectionSymbolStringTable;
    header = (Elf32_Ehdr *) map_start;
    sectionHeaderTable = (Elf32_Shdr *) (map_start + (header->e_shoff));
    int numberOfSections = header->e_shnum;
    int j;
    for (j = 0; j < numberOfSections; ++j) {
        if (sectionHeaderTable[j].sh_type == SHT_DYNSYM) {
            dymSymbolTable = (Elf32_Sym *) (map_start + sectionHeaderTable[j].sh_offset);
            sectionSymbolStringTable = (char *) (map_start +
                                                 (sectionHeaderTable[sectionHeaderTable[j].sh_link].sh_offset));
        }
    }
    int i;
    for (i = 0; i < numberOfSections; i++) {
        if ((sectionHeaderTable[i].sh_type) == SHT_REL) {
            printf("OFFSET         INFO         TYPE         Sym.value         Sym.name\n");
            sectionRel = (Elf32_Rel *) (map_start + (sectionHeaderTable[i].sh_offset));
            int numberOfEntries = sectionHeaderTable[i].sh_size / (sizeof(Elf32_Rel));
            int k;
            for (k = 0; k < numberOfEntries; k++) {
                int index = ELF32_R_SYM(sectionRel[k].r_info);
                char *symbolName = (sectionSymbolStringTable + (dymSymbolTable[index].st_name));
                printf("%.8X         %.8X         %.8d         %.8d         %s\n", sectionRel[k].r_offset,
                       sectionRel[k].r_info, ELF32_R_TYPE(sectionRel[k].r_info), dymSymbolTable[index].st_value,
                       symbolName);
            }
        }
    }
};
struct fun_desc menu[] = {{"Toggle Debug Mode",   TDM},
                          {"Examine ELF File",    EEF},
                          {"Print Section Names", PSN},
                          {"Print Symblos",       PS},
                          {"Link Check",          LC},
                          {"Relocation Tables",   RT},
                          {"task0a",              T0A},
                          {"quit",                quit},
                          {NULL, NULL}};


int main(int argc, char **argv) {
    progState = malloc(sizeof(state));
    progState->debug_mode = '0';
    char name[1];
    int chosenFun;
    int menuSize = sizeof(menu) / (sizeof(struct fun_desc));
    while (1) {
        printf("Please choose a function:\n");
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



