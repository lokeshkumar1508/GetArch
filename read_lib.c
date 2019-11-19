#include <getopt.h>
#include <stdio.h>
#include "libapp.h"
#include <errno.h>
#include <string.h>
/* display arch flag*/
int archFlag;


/* Supported Arguments*/
const static struct option CmdOptions[] ={
	{"a", no_argument, 0,'a'},
	{0, no_argument, 0, 0}
};

/* App Usage details*/
int print_usage(){
	printf("Usage: Getarch -a {ELF File(Library) Name}\n");
	printf("Option: -a :prints architecture\n");
	return 0;
}
int print_machine_type(int machine){
	printf("Machine architecture is:");
	switch(machine){
	case 0x02:
		printf("SPARC\n");
		break;
	case 0x03:
		printf("X86\n");
		break;
	case 0x08:
		printf("MIPS\n");
		break;
	case 0x14:
		printf("PowerPC\n");
		break;
	case 0x16:
		printf("S390\n");
		break;
	case 0x28:
		printf("arm\n");
		break;
	case 0x2A:
		printf("SuperH\n");
		break;
	case 0x32:
		printf("IA-64\n");
		break;
	case 0x3E:
		printf("x86-64\n");
		break;
	case 0xB7:
		printf("AArch64\n");
		break;
	case 0xF3:
		printf("RISC-V\n");
		break;
	default:
		printf("Unknown/Unsupported!!\n");
		break;

	}

}
/*parse arguments and enable flags corresponding to header info to be dumped*/
int parse_args(int argc, char **argv){
	int opt;
	while((opt = getopt_long(argc, argv, "a", CmdOptions, NULL))!= -1){
		switch(opt) {
			case 'a':
#if DEBUG
				printf("A selected\n");
#endif
				archFlag = 1;
				break;
			default:
#ifdef DEBUG
				printf("\nnot valid argument\n");
#endif
				print_usage();
		}
	}
	return 0;

}

/*getData = number of bytes we want to retreive from data*/
long long get_little_endian(unsigned char *data,int getData){
	switch(getData){
	case 2:// machine arch is of 2 bytes
		return ((unsigned int)data[0] | (unsigned int) (data[1] << 8));
	default:
		printf("unsupported data size!!\n");
	}
}

long long get_big_endian(unsigned char *data,int getData){
	switch(getData){
	case 2:// machine arch is of 2 bytes
		return ((unsigned int)data[1] | (unsigned int) (data[0] << 8));
	default:
		printf("unsupported data size!!\n");
	}
}

/*
 * Parse file here, read headers
 */
int parse_file(char **argv){
	FILE *file = NULL;
#ifdef DEBUG
	printf("trying to open file:%s\n",argv[2]);
#endif
	file = fopen(argv[2],"rb");	

	if(file == NULL){
		printf("Error!!! Unable to read File:%s\n",argv[2]);
		return -EAGAIN;
	}
	elf_header_t local_elf_header;
	if(!fread(&local_elf_header, 24, 1 ,file)){
		printf("unable to read header\n, try again");
		fclose(file);
		return -EAGAIN;
	}
#ifdef DEBUG
	printf("header elf?%c %c %c %c\n",local_elf_header.e_ident[0],
			local_elf_header.e_ident[1],
			local_elf_header.e_ident[2],
			local_elf_header.e_ident[3]);
/*Check endianness to understand how to read data from header!!*/
//	elf_header_t local_elf_header = (elf_header_t)*_local_elf_header;
	printf("endianness:%d\n",local_elf_header.endian);
#endif
	switch(local_elf_header.endian){
	case ELFDATA2LSB:
		get_byte_data = get_little_endian;
		break;
	case ELFDATA2MSB:	
		get_byte_data = get_big_endian;
		break;
	default:
		printf("Unable to read endianness:%d\n",local_elf_header.endian);
	}
	/*
	 *Extract machine Architecture
	 */
#ifdef DEBUG
	printf("machine?0x%x0x%x\n",local_elf_header.e_machine[0],
			local_elf_header.e_machine[1]);
#endif
	unsigned short machine = get_byte_data(local_elf_header.e_machine,2);
	printf("machine?%x\n",machine);
	if(archFlag){
		print_machine_type(machine);
	}
	return machine;
}




int main (int argc, char ** argv){
/* for now we support only Header info */
#ifdef DEBUG
	printf("number or args:%d\n",argc);
#endif

	int numCmdSupported = (sizeof(CmdOptions)/sizeof(struct option)) - 1;
#ifdef DEBUG
	printf("number of supported commands:%d\n",numCmdSupported);
#endif
	/*
	 * app needs at least two arguments
	 * 1. command
	 * 2. filename 
	 */

	if((argc > (numCmdSupported + 2)) || (argc < 3)){ 
		print_usage();
		return -1;
	}
#ifdef DEBUG
	int i =0;
	while(i < argc){
		printf("arg[%d]:%s\n",i,argv[i]);
		i++;
	}	
#endif
	parse_args(argc, argv);
	parse_file(argv);
	return 0;

}
