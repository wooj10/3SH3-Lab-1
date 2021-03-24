/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void)
{
    // creating a file descriptor
    char file_descriptor = open("./res.txt", O_RDWR, S_IRUSR | S_IWUSR);
    struct stat sb;

    // getting the size of the file 
    if(fstat(file_descriptor,&sb) == -1)
    {
        perror("Couldn't get file size.\n");
    }
    printf("File size is %ld\n", sb.st_size);
    
    // checking that the file size is smaller than the region size
    if(sb.st_size >= 10)
    {
        printf("File size is smaller than region size.\n");
    }

    // getting the file from virtual memory
    char *res_file = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    // printing file values
    for(int i = 0; i < sb.st_size; i++)
    {
        printf("%c", res_file[i]);
        
    }
    
    int unit_type = 0;
    int units_needed = 0;
    while(1)
    {
        // asking for unit type and number of units 
        printf("\nPlease enter unit type, or -1 to exit. ");
        scanf("%d", &unit_type);
        
        if (unit_type == -1){
        	break;
        }
        
        printf("\nHow many units are needed? ");
        scanf("%d", &units_needed);
        
        // if the unit type is zero, subtract from 0 type units in file
        if(unit_type == 0)
        {
            if(((int)(res_file[2] - 48) - units_needed) < 0)
            {
                printf("Not enough units.\n");
            }
            else
            {
                res_file[2] = (char)((int)(res_file[2] - 48) - units_needed + 48);
                printf("You have %c units of type %d left.\n", res_file[2], unit_type);
            }
        }
        
        // if the unit type is one, subtract from 1 type units in file    
        if(unit_type == 1)
        {
            if(((int)(res_file[6] - 48) - units_needed) < 0)
            {
                printf("Not enough units.\n");
            }
            else
            {
                res_file[6] = (char)((int)(res_file[6] - 48) - units_needed + 48);
                printf("You have %c units of type %d left.\n", res_file[6], unit_type);
            }
        }
        
        // if the unit type is two, subtract from 2 type units in file
        if(unit_type == 2)
        {
            if(((int)(res_file[10] - 48) - units_needed) < 0)
            {
                printf("Not enough units.\n");
            }
            else
            {
                res_file[10] = (char)((int)(res_file[10] - 48) - units_needed + 48);
                printf("You have %c units of type %d left.\n", res_file[10], unit_type);
            }            
        }
        
        msync(res_file, sb.st_size, MS_SYNC);
        
        // resetting the variables for the next iteration
        unit_type = 0;
        units_needed = 0;
    }
    
    return 0;
}
