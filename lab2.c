#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main (void) {
	int fd[2], fd1[2], nbytes, input, sum;
	pid_t childpid;
	char string[] = "We love 3 SH3 !\n";
	int readbuffer;
	
	sum = 0;
	
	pipe(fd);
	pipe(fd1);
	
	if (( childpid = fork() ) == -1) {
		perror("fork");
		exit(0) ;
	}
	
	if(childpid == 0) {
		close(fd[0]);
		close(fd1[1]);
		
		while(1){
			printf("Please enter an integer, or -1 to exit: \n");
			scanf("%d", &input);
			
			write(fd[1] , &input, sizeof(input));
			if(input == -1) break;
		}
		
		nbytes = read(fd1[0], &readbuffer, sizeof(readbuffer));
		printf("Sum: %d\n", readbuffer);
		
		
		exit(0);
	}
	
	else {
		close(fd[1]);
		close(fd1[0]);
		
		while (1){
			nbytes = read(fd[0], &readbuffer, sizeof(readbuffer));
			if (readbuffer == -1) break;
			else sum += readbuffer;
		}
		
		write(fd1[1] , &sum, sizeof(sum));
	}
	
	return 0;
}
