#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/errno.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(void)
{
    // creating a file descriptor
    char file_descriptor = open("./res.txt", O_RDWR, S_IRUSR | S_IWUSR);
    struct stat sb;

    
    
    char path_name[200];
    
    while(1)
    {
    	printf("Please enter a file or directory.\n");
        scanf("%s", path_name);
        
        // getting the size of the file 
	    if(stat(path_name,&sb) == -1)
	    {
			perror("Encountered an error");
	    } else {
			printf("\nmode:\t\t\t%o \
					\nnumber of links:\t%ju \
					\nowner name:\t\t%s \
					\ngroup name:\t\t%s \
					\nsize in bytes:\t\t%jd \
					\nsize in blocks:\t\t%jd \
					\nlast modification time:\t%s \
					\nname:\t%s\n\n", sb.st_mode, sb.st_nlink, \
					getpwuid(sb.st_uid) -> pw_name, getgrgid(sb.st_gid) -> gr_name, sb.st_size, sb.st_blksize, ctime(&sb.st_mtime), path_name);
	    }
    }
    
    return 0;
}
