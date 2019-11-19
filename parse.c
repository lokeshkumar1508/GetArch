#include <getopt.h>
#include <stdio.h>
#include <libapp.h>
#include <errno.h>
#include <string.h>


extern int archFlag;

elf_info g_elf;


static int get_machine_type(int machine){
	dbgprintf("Machine architecture is:");
	switch(machine){
	case 0x02:
		dbgprintf("SPARC\n");
		strcpy(g_elf.cmachine, "SPARC");
		break;
	case 0x03:
		dbgprintf("X86\n");
		strcpy(g_elf.cmachine ,"X86");
		break;
	case 0x08:
		dbgprintf("MIPS\n");
		strcpy(g_elf.cmachine , "MIPS");
		break;
	case 0x14:
		dbgprintf("PowerPC\n");
		strcpy(g_elf.cmachine , "PowerPC");
		break;
	case 0x16:
		dbgprintf("S390\n");
		strcpy(g_elf.cmachine , "S390");
		break;
	case 0x28:
		dbgprintf("arm\n");
		strcpy(g_elf.cmachine , "ARM");
		break;
	case 0x2A:
		dbgprintf("SuperH\n");
		strcpy(g_elf.cmachine , "SuperH");
		break;
	case 0x32:
		dbgprintf("IA-64\n");
		strcpy(g_elf.cmachine , "IA-64");
		break;
	case 0x3E:
		dbgprintf("x86-64\n");
		strcpy(g_elf.cmachine , "x86-64");
		break;
	case 0xB7:
		dbgprintf("AArch64\n");
		strcpy(g_elf.cmachine , "AArch64");
		break;
	case 0xF3:
		dbgprintf("RISC-V\n");
		strcpy(g_elf.cmachine , "RISC-V");
		break;
	default:
		dbgprintf("Unknown/Unsupported!!\n");
		strcpy(g_elf.cmachine , "UNKWN??");
		break;

	}
	return 0;
}


/*
 * Retreive data from litle endian library
 * getData = number of bytes we want to retreive from data buffer
 */
long long get_little_endian(unsigned char *data,int getData){
	switch(getData){
	case 2:// machine arch is of 2 bytes
		return ((unsigned int)data[0] | (unsigned int) (data[1] << 8));
	default:
		dbgprintf("unsupported data size!!\n");
	}
}

/*
 * Retreive data from big endian library
 * getData = number of bytes we want to retreive from data buffer
 */

long long get_big_endian(unsigned char *data,int getData){
	switch(getData){
	case 2:// machine arch is of 2 bytes
		return ((unsigned int)data[1] | (unsigned int) (data[0] << 8));
	default:
		dbgprintf("unsupported data size!!\n");
	}
}




/*
 * Parse file here, read headers, return machine value
 */
int check_header(char *header){
	if((header[0] == ELF_MAG0) &&
		(header[1] == ELF_MAG1) &&
		(header[2] == ELF_MAG2) &&
		(header[3] == ELF_MAG3)){
		return 0;
	}
	return -1;

}

int check_elf_header(char *filename){

	FILE *file = NULL;
	memset(&g_elf, 0, sizeof(struct _elf_info));
	dbgprintf("trying to open file:%s\n", filename);
	file = fopen(filename, "rb");	

	if(file == NULL){
		printf("Error!!! Unable to read File:%s\n",filename);
		return -1;
	}
	elf_header_t local_elf_header;
	if(!fread(&local_elf_header, 24, 1 ,file)){
		printf("unable to read header\n, try again");
		fclose(file);
		return -1;
	}

	dbgprintf("header elf?0x%x %c %c %c\n",local_elf_header.e_ident[0],
			local_elf_header.e_ident[1],
			local_elf_header.e_ident[2],
			local_elf_header.e_ident[3]);

/*Check endianness to understand how to read data from header!!*/

	if(check_header(local_elf_header.e_ident)){
		dbgprintf("%s is not an library\n",filename);
		return -1;
	}
	dbgprintf("found lib file:%s\n", filename);
	dbgprintf("endianness:%d\n", local_elf_header.endian);
	switch(local_elf_header.endian){
	case ELFDATA2LSB:
		get_byte_data = get_little_endian;
		break;
	case ELFDATA2MSB:	
		get_byte_data = get_big_endian;
		break;
	default:
		printf("Unable to read endianness:%d\n", local_elf_header.endian);
	}
	/*
	 *Extract machine Architecture
	 */
	dbgprintf("machine?0x%x0x%x\n",local_elf_header.e_machine[0],
			local_elf_header.e_machine[1]);
	unsigned short machine = get_byte_data(local_elf_header.e_machine,2);
	if(archFlag){
		get_machine_type(machine);
	}
	dbgprintf("g_elf.machine:%s\n", g_elf.cmachine);
	return 0;
}


