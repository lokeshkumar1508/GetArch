#ifndef __LIBAPP_H__
#define __LIBAPP_H__

#define ELF_MAG0 0x7f
#define ELF_MAG1 'E'
#define ELF_MAG2 'L'
#define ELF_MAG3 'F'

#define ELFCLASS32 1
#define ELFCLASS64 2

/*Data Encoding*/
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#ifdef DEBUG
#define ENDEBUG 1
#else
#define ENDEBUG 0
#endif

#define  dbgprintf(...) \
		do { if( ENDEBUG) fprintf(stderr,__VA_ARGS__);}while (0)

/*
* below structure contain info  we extracted from databuffer(flestream)
* we only support machine type info extraction(we also extract endianness, we just dont save it for now)
*/
typedef struct _elf_info{
    unsigned char cmachine[2];
}elf_info;


/*below structure contains partial ELF headr, complete header has much more information.*/

typedef struct elf_header_partial{
    unsigned char e_ident[4]; //4 byte magic number
    unsigned char format;//1: 32 bit , 2:64 bit
    unsigned char endian;/*1:litlle, 2:big*/
    unsigned char version; /*set 1*/
    unsigned char os_abi;/*check elf header wiki for more info*/
    unsigned char abi_version;
    unsigned char pad[7];/*not used, reserved*/
    unsigned char e_type[2];/*object file type*/
    unsigned char e_machine[2];/*machine arch type*/
    unsigned char e_version[4];/*set to 1 for original version*/
}elf_header_t;

/*
 * Functions & Function pointers to get byte data based on endianness
*/
long long (*get_byte_data)(unsigned char *,int);
long long get_litle_endian(unsigned char *,int);
long long get_big_endian(unsigned char *,int);

/*
*Parses file and if file is elf file(library) retreives arch info
*
*/
int check_elf_header(char *filename);

/*
* traverse directory with 0 Depth 
*i.e. no Sub-Dirs, also handles  case where user passes filename instead of directory name
*/
int get_dir_files(char *dirPATH);
#endif
