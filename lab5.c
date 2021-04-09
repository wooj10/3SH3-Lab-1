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
#include <dirent.h>
#include <string.h>
struct stat sb;

void recursive_traversal(const char *base)
{
    DIR *dir;
    struct dirent *current;

    if (!(dir = opendir(base)))
        return;

    while ((current = readdir(dir)) != NULL) {
    	if(stat(base,&sb) == -1)
		{
			perror("Encountered an error");
		}
		//if current entry is directory
        if (current->d_type == DT_DIR) {
            char path[1000];
		    if (strcmp(current->d_name, ".") != 0 && strcmp(current->d_name, "..") != 0){
		    	//set path variable to (current path)/(current entry)
				sprintf(path,  "%s/%s", base, current->d_name);
				
				//print name of current directory + stats
				printf("\n%s:\n", path);
				printf("%o %ju %s %s %jd %jd %-4.20s %s\n", sb.st_mode, sb.st_nlink, 
							getpwuid(sb.st_uid) -> pw_name, getgrgid(sb.st_gid) -> gr_name, 
							sb.st_size, sb.st_blksize, ctime(&sb.st_mtime) + 4, current->d_name);
				//enter subdirectory
				recursive_traversal(path);
            }
        } else {
        	//if file, just print stats
            printf("%o %ju %s %s %jd %jd %-4.20s %s\n", sb.st_mode, sb.st_nlink, 
					getpwuid(sb.st_uid) -> pw_name, getgrgid(sb.st_gid) -> gr_name, 
					sb.st_size, sb.st_blksize, ctime(&sb.st_mtime) + 4, current->d_name);
        }
    }
    closedir(dir);
}

int main(void)
{
    // creating a file descriptor
    char file_descriptor = open("./res.txt", O_RDWR, S_IRUSR | S_IWUSR);
    
    char path_name[200];
    
    recursive_traversal(".");
    
    while(1)
    {
    	printf("Please enter a file or directory, or q to exit.\n");
        scanf("%s", path_name);
        
	    if(strcmp(path_name, "q") == 0){
          break;
        }
        
	    if(stat(path_name,&sb) == -1)
	    {
			perror("Encountered an error");
		} else if(S_ISREG(sb.st_mode)) {
			printf("\nmode:\t\t\t%o \
					\nnumber of links:\t%ju \
					\nowner name:\t\t%s \
					\ngroup name:\t\t%s \
					\nsize in bytes:\t\t%jd \
					\nsize in blocks:\t\t%jd \
					\nlast modification time:\t%-4.20s \
					\nname:\t\t\t%s\n\n", sb.st_mode, sb.st_nlink, 
					getpwuid(sb.st_uid) -> pw_name, getgrgid(sb.st_gid) -> gr_name, 
					sb.st_size, sb.st_blksize, ctime(&sb.st_mtime) + 4, path_name);
	    }
		else{
			recursive_traversal(path_name);
		}
    }
    
    return 0;
}
