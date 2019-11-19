#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <libapp.h>

/*supports name length of 100*/
typedef struct _filenames{
    char name[100];
    char arch[10]; 
    struct _filenames *next;
}filenames;

static filenames *head = NULL;
static filenames *current = NULL;
extern elf_info g_elf;



static int initialize_head()
{
    head = (filenames *) malloc(sizeof(filenames));
    if ( head == NULL )
    {
        dbgprintf("Unable to initialize Head!!!\n");
        return -1;
    }
    head->next = NULL;
    current = head;
    return 0;
}


/*
 * prints Library names and compatible archs
 *
 */
static int print_libs(int is_error)
{
    int libcount = 0;
    dbgprintf("inside print_libs\n");
    if ( head == NULL )
    {
        dbgprintf("head is null, most probably because no libs were found\n");
        goto exit;
    }

    printf("================\n");
    printf("printing libs\n");
    printf("================\n");
    filenames *temp = head;
    if ( temp != current )
    {
        do
        {
            libcount++;
            printf("%s:           Arch:[%s]\n", temp->name, temp->arch);
            temp = temp->next;
        } while ( temp != NULL );
    } else {
        libcount++;
        printf("%s:           Arch:[%s]\n", temp->name, temp->arch);
    }

exit:
    printf("================\n");
    printf("Total number of libs found:[%d]\n", libcount);
    if ( is_error )
        return -1;
    else
        return 0;

}





/*
 * Add Library names and compatible archs
 *
 */
static int add_name(char *filename, char *arch)
{
	int status = 0;
	if ( head == NULL )
        {
		status = initialize_head();
            if ( status )
            {
                dbgprintf("Error occured while initializing head\n");
                return -1;
            }
            strcpy(head->name, filename);
            strcpy(head->arch, arch);
            dbgprintf("added following name and arch!!!\n");
            dbgprintf("%s: %s\n", head->name, head->arch);
            return 0;
        }

        if ( filename == NULL || arch == NULL )
        {
            dbgprintf("Error: fiename or arch name passed is null");
            return -1;
        }
        filenames * temp = (filenames *)malloc( sizeof(filenames) );
        if ( temp == NULL )
        {
            dbgprintf("Failed to allocate memory\n");
            dbgprintf("Trying to print Lib names we found so far\n");
            print_libs(1);
            return -1;
        }
        strcpy(temp->name, filename);
        strcpy(temp->arch, arch);

        dbgprintf("added following name and arch!!!\n");
        dbgprintf("%s: %s\n", temp->name, temp->arch);

        current->next = temp;
        current =  temp;
        current->next=NULL;

        return 0;	
}






/* 
 * not traversing sub directories, support can be added if required
 */
int get_dir_files(char *dirPATH)
{
    int status = 0;
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    char *full_path = (char *) malloc( sizeof(char) *200 );
    if ( dirPATH == NULL ) {
        printf("kindly provide dir name\n");
        return -1;
    }

    if ( (dp = opendir(dirPATH)) == NULL ) {
        dbgprintf("unable to find dirname:%s\n", dirPATH);
        /*In case user has passed filename instead of directory name*/
        if ( access( dirPATH, F_OK ) != -1 ) {
            /* file exists */
            status = check_elf_header( dirPATH );
            if ( !status ) {
                add_name(dirPATH ,g_elf.cmachine);
                dbgprintf("added filename, now goto print_libs\n");
                goto print_libs;
            }
        } else {
	        /* file doesn't exist */
                fprintf(stderr,"cannot open directory/File: %s\n", dirPATH);
	        return -1;
        }
    }
    while ( (entry = readdir(dp)) != NULL ) {
        memset(full_path,0,200);
        strcpy(full_path ,dirPATH);
        strcat(full_path, "/"); /* on Linux // works as /, so not an issue */
        full_path = strcat(full_path, entry->d_name);

#ifdef __unix__ 
        lstat(full_path, &statbuf);
#elif defined(_WIN32) || defined(WIN32)  
        stat(full_path, &statbuf);
#endif
        if ( S_ISDIR(statbuf.st_mode) ) {
            /* Found a directory, but ignore*/
            dbgprintf("ignoring:%s\n", entry->d_name);
            continue;
        } else {
                dbgprintf("%s\n", entry->d_name);
                status = check_elf_header(full_path);

                if ( !status )
                    add_name(entry->d_name, g_elf.cmachine);
        }
    }
	
	chdir("..");
	closedir(dp);
print_libs:
	print_libs(0);
	free(full_path);
	return 0;
}


