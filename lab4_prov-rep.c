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
    
    char need_process;
    int unit_type = 0;
    int units_needed = 0;
    int sem_id_mutex;
    struct sembuf sem_buffer;
    pid_t childpid;
    
    mutex_key = 22;
    sem_id_mutex = semget(mutex_key,1,SEMPERM|IPC_CREAT|IPC_EXCL);
    semnum.val = 1;
	semctl(sem_id_mutex,0,SETVAL, semnum);
    
    if (( childpid = fork() ) == -1) {
		perror("fork");
		exit(0);
	}
	
	int pagesize;
	if (childpid == 0){
		while (1){
			if (need_process == 'n' || need_process == 'N'){
				break;
			}
			
			pagesize = getpagesize();
			char vec[(sb.st_size + pagesize)/pagesize];
			printf("\nPage size: %d\n", pagesize);
			printf("Current resources:\n");
			for(int i = 0; i < sb.st_size; i++){
				printf("%c", res_file[i]);
			}
			
			if (mincore(res_file, sb.st_size, vec) != -1){
				printf("mincore:\n");
				
				for (int i = 0; i < sb.st_size; i++){
					printf("%d ", vec[i]);
				}
			} else {
				printf("mincore error");
			}
			printf("\n\n");
			
			sleep(10);
		}
		exit(0);
    } else {
    	while(1)
		{
		    printf("Does a new resource need to be added? y/n\n");
		    scanf(" %c", &need_process);
		    
		    if(need_process == 'y' || need_process == 'Y')
		    {
		        printf("\nWhat is the unit type? ");
		        scanf("%d", &unit_type);
		        
		        printf("\nHow many units are needed? ");
		        scanf("%d", &units_needed);
		        
		        //wait
		        sem_buffer.sem_op = -1;
				semop(sem_id_mutex, &sem_buffer, 1);
		        
		        if(unit_type == 0){
		        	if(((int)(res_file[2] - 48) + units_needed) >= 10)
					{
						printf("Too many units for resource.\n");
					} else {
				        res_file[2] = (char)(((int)res_file[2] - 48) + units_needed + 48);
				        printf("Unit type %d now has %c units.\n", unit_type, res_file[2]);
		            }
		        }
		        
		        else if(unit_type == 1) {
		        	if(((int)(res_file[6] - 48) + units_needed) >= 10)
					{
						printf("Too many units for resource.\n");
					} else {
				        res_file[6] = (char)(((int)res_file[6] - 48) + units_needed + 48);
				        printf("Unit type %d now has %c units.\n", unit_type, res_file[6]);
		            }
		        }
		        
		        else if(unit_type == 2) {
		        	if(((int)(res_file[10] - 48) + units_needed) >= 10)
					{
						printf("Too many units for resource.\n");
					} else {
				        res_file[10] = (char)(((int)res_file[10] - 48) + units_needed + 48);
				        printf("Unit type %d now has %c units.\n", unit_type, res_file[10]);
		            }
		        }
		        
		        else {
		        	printf("No unit of that type.\n");
		        }
		        
		        msync(res_file, sb.st_size, MS_SYNC);
		        
		        //signal
		        sem_buffer.sem_op = 1;
		        semop(sem_id_mutex, &sem_buffer, 1);
		    } else if (need_process == 'n' || need_process == 'N') {
		    	break;
		    }
		}
    }
    
    semctl(sem_id_mutex,0,IPC_RMID,NULL);
    return 0;
}
