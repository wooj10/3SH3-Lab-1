#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

//one semaphore per thread
sem_t s0, s1, s2, s3;

#define N 4
int mat[N][N];

//bubble sort
void sort_row_forward(){
	
}

void sort_row_reverse(){
	
}

void sort_col(){
	
}

void *shear_sort_thread(void *vargp){
	int *tid = (int *)vargp;	//thread id
	printf("In thread %d\n", *tid);
	
	//probably need a loop in here somewhere
	//use semaphores to synchronize phases across rows
	if (){
		sort_row_forward();
	} else if (){
		sort_row_reverse();
	} else {
		sort_col();
	}
	
	return NULL;
}

int main(void) {
	FILE *f = fopen("input.txt", "r");
	
	//read input.txt as matrix
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++){
			fscanf(f, "%d", &mat[i][j]);
			printf("%d\n", mat[i][j]);
		}
	}
	
	//initialize semaphores
	sem_init(&sem0, 0, 0);
	sem_init(&sem1, 0, 0);
	sem_init(&sem2, 0, 0);
	sem_init(&sem3, 0, 0);
	
	pthread_t threads[N];
	int rc;
	
	for(int i=0; i<N; i++){
       printf("In main: creating thread %d\n", i);
       rc = pthread_create(&threads[i], NULL, sort_thread, (void *)&threads[i]);
       //last arg is what you're passing to thread
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
       pthread_join(threads[i], NULL);
       printf("In main: after thread %d\n\n", i);
    }
    
    //after each phase, print current state of matrix
	
	pthread_exit(NULL);
	return 0;
}
