# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

// user - defined signal handler for alarm .
void alarm_handler ( int signo ) {
	if (signo == SIGALRM) {
		printf("Alarm\n");
	} else if (signo == SIGINT) {
		printf("Ctrl+C pressed!\n");
	} else if (signo == SIGTSTP) {
		printf("Ctrl+Z pressed!\n");
		exit(1);
	}
}

int main ( void ) {
	// register the signal handlers
	if ( signal ( SIGALRM , alarm_handler ) == SIG_ERR ) {
		printf ("failed to register alarm handler 1.");
		exit(1);
	}
	
	if ( signal ( SIGINT , alarm_handler ) == SIG_ERR ) {
		printf ("failed to register alarm handler 2.");
		exit(1);
	}
	
	if ( signal ( SIGTSTP , alarm_handler ) == SIG_ERR ) {
		printf ("failed to register alarm handler 3.");
		exit(1);
	}
	
	while (1) {
		sleep(2);
		if ( raise(SIGALRM) != 0 ) {	// check alarm response for error
			printf ("failed to raise alarm.");
			exit(1);
		}
	} // raise alarm every 2 seconds
}

