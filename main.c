#include <getopt.h>
#include <stdio.h>
#include <libapp.h>


/* Arch flag: enables print of arch*/
int archFlag;

/* Supported Arguments*/
const static struct option CmdOptions[] = {
    {"a", no_argument, 0, 'a'},
    {0, no_argument, 0, 0}
};


/* App Usage details*/
int print_usage(){
    printf("Usage: Getarch -a {Directory name} or {ELF File(Library) Name}\n");
    printf("Option: -a :prints architecture\n");
    return 0;
}


/*parse arguments and enable flags corresponding to header info to be dumped*/
int parse_args(int argc, char **argv){
    int opt;
    while ( ( opt = getopt_long(argc, argv, "a", CmdOptions, NULL) ) != -1 ) {

        switch(opt) {
            case 'a':
                    archFlag = 1;
                    break;
            default:
                    print_usage();
                    return -1;
        }
    }
    return 0;
}


int main (int argc, char ** argv){
    int result = 0;
    int numCmdSupported = ( sizeof(CmdOptions) / sizeof(struct option) ) - 1;
    dbgprintf("number of supported commands:%d\n", numCmdSupported);
/*
 * app needs at least two arguments
 * 1. command options , supports "-a" only for now
 * 2. filename 
 */

    if ( (argc > (numCmdSupported + 2)) || (argc < 3) ){ 
        print_usage();
        return -1;
    }
#ifdef DEBUG
    int i = 0;
    while (i < argc) {
        printf("arg[%d]:%s\n",i,argv[i]);
        i++;
    }	
#endif
    result = parse_args(argc, argv);
    if ( !result )
/* last argument should be filename */
        result = get_dir_files(argv[argc - 1]);

        return result;

}
