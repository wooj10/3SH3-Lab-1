#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

//one semaphore per thread
sem_t s0, s1, s2, s3;
//pthread_mutex
#define N 4
int mat[N][N];
int threads_finished = 0;

//bubble sort
void sort_row_forward(int rowNum){
    for(int i = (N-1); i >= 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(j == i){
                break;
            }
            if(mat[rowNum][j] > mat[rowNum][j+1])
            {
                int temp = mat[rowNum][j+1];
                mat[rowNum][j+1] = mat[rowNum][j];
                mat[rowNum][j] = temp;
            }
            else if(mat[rowNum][j] < mat[rowNum][j+1])
            {
                continue;
            }
        }
    }
}

void sort_row_reverse(int rowNum){
    for(int i = (N-1); i >= 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(j == i){
                break;
            }
            if(mat[rowNum][j] < mat[rowNum][j+1])
            {
                int temp = mat[rowNum][j+1];
                mat[rowNum][j+1] = mat[rowNum][j];
                mat[rowNum][j] = temp;
            }
            else if(mat[rowNum][j] > mat[rowNum][j+1])
            {
                continue;
            }
        }
    }
}

void sort_col(int colNum){
    for(int i = (N-1); i >= 0; i--)
    {
        for(int j = 0; j < i; j++)
        {
            if(j == i){
                break;
            }
            if(mat[j][colNum] > mat[j+1][colNum])
            {
                int temp = mat[j+1][colNum];
                mat[j+1][colNum] = mat[j][colNum];
                mat[j][colNum] = temp;
            }
            else if(mat[j][colNum] < mat[j+1][colNum])
            {
                continue;
            }
        }
    }	
}

void *shear_sort_thread(void *vargp){
	int *tid = (int *)vargp;	//thread id
	int col_flag = 0;
	printf("In thread %d\n", *tid);
	
	switch(*tid){
		case 0:
			sem_post(&s0);
			sem_wait(&s0);
			break;
		case 1:
			sem_post(&s1);
			sem_wait(&s1);
			break;
		case 2:
			sem_post(&s2);
			sem_wait(&s2);
			break;
		case 3:
			sem_post(&s3);
			sem_wait(&s3);
			break;
	}
	
	while(1){
		if (col_flag == 0 && *tid%2 == 0){
			sort_row_forward(*tid);
		} else if (col_flag == 0 && *tid%2 == 1){
			sort_row_reverse(*tid);
		} else {
			sort_col(*tid);
		}
		
		//printf("thread %d, count %d\n", *tid, threads_finished);
		threads_finished++;
		
		//update semaphores based on thread
		switch(*tid){
			case 0:
				//alert the other threads that phase is finished
				//sem_post(&s0);
				sem_post(&s1);
				sem_post(&s2);
				sem_post(&s3);
				//wait for other threads to finish
				sem_wait(&s0);
				sem_wait(&s0);
				sem_wait(&s0);
				sem_wait(&s0);
				
				break;
			case 1:
				//alert the other threads that phase is finished
				sem_post(&s0);
				//sem_post(&s1);
				sem_post(&s2);
				sem_post(&s3);
				//wait for other threads to finish
				sem_wait(&s1);
				sem_wait(&s1);
				sem_wait(&s1);
				sem_wait(&s1);
				break;
			case 2:
				//alert the other threads that phase is finished
				sem_post(&s0);
				sem_post(&s1);
				//sem_post(&s2);
				sem_post(&s3);
				//wait for other threads to finish
				sem_wait(&s2);
				sem_wait(&s2);
				sem_wait(&s2);
				sem_wait(&s2);
				break;
			case 3:
				//alert the other threads that phase is finished
				sem_post(&s0);
				sem_post(&s1);
				sem_post(&s2);
				//sem_post(&s3);
				//wait for other threads to finish
				sem_wait(&s3);
				sem_wait(&s3);
				sem_wait(&s3);
				sem_wait(&s3);
				break;
		}
		
		//flip flag
		col_flag = !col_flag;
	}
	
	return NULL;
}

int main(void) {
	FILE *f = fopen("input.txt", "r");
	
	//read input.txt as matrix
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++){
			fscanf(f, "%d", &mat[i][j]);
			printf("%d\t", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	//initialize semaphores
	sem_init(&s0, 0, 0);
	sem_init(&s1, 0, 0);
	sem_init(&s2, 0, 0);
	sem_init(&s3, 0, 0);
	
	pthread_t threads[N];
	int tids[N];
	
	for(int i=0; i<N; i++){
		tids[i] = i;
		pthread_create(&threads[i], NULL, shear_sort_thread, (void *)&tids[i]);
    }
    
    sem_wait(&s0);
	sem_wait(&s1);
	sem_wait(&s2);
	sem_wait(&s3);
	
	sem_post(&s0);
	sem_post(&s1);
	sem_post(&s2);
	sem_post(&s3);
	
    int count = 0;
    while(count < 5){
		if (threads_finished == 4){
			//after each phase, print current state of matrix
			for(int i = 0; i < N; i++) {
				for(int j = 0; j < N; j++){
					printf("%d\t", mat[i][j]);
				}
				printf("\n");
			}
			printf("\n");
			
			
			threads_finished = 0;
			count++;
			
			sem_post(&s0);
			sem_post(&s1);
			sem_post(&s2);
			sem_post(&s3);
		}
	}
	
	return 0;
}
