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

#define SEMPERM 0600 /* permission for semaphore */

key_t mutex_key;

union {
	int val;

	struct semid_ds *buffer;

	ushort *array;
}semnum;

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

    // getting the file from virtual memory
    char *res_file = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    // printing file values
    for(int i = 0; i < sb.st_size; i++)
    {
        printf("%c", res_file[i]);
    }
    
    int sem_id_mutex;
    struct sembuf sem_buffer;
    
    mutex_key = 22;
    sem_id_mutex = semget(mutex_key,1,SEMPERM|IPC_CREAT|IPC_EXCL);
    semnum.val = 1;
	semctl(sem_id_mutex,0,SETVAL, semnum);
    
    int unit_type = 0;
    int units_needed = 0;
    while(1)
    {
        // asking for unit type and number of units 
        printf("\nPlease enter unit type. ");
        scanf("%d", &unit_type);
        
        printf("\nHow many units are needed? ");
        scanf("%d", &units_needed);
        
        //wait
        sem_buffer.sem_op = -1;
        semop(sem_id_mutex, &sem_buffer, 1);
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
        else if(unit_type == 1)
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
        else if(unit_type == 2)
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
        
        else {
        	printf("No unit of that type.\n");
        }
        
        msync(res_file, sb.st_size, MS_SYNC);
        
        //signal
        sem_buffer.sem_op = 1;
        semop(sem_id_mutex, &sem_buffer, 1);
        
        // resetting the variables for the next iteration
        unit_type = 0;
        units_needed = 0;
    }
    
    semctl(sem_id_mutex,0,IPC_RMID,NULL);
    return 0;
}
