#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
007412039 - Christopher Vargas
014628729 - Jacky Li
029346666 - Daman Patel
*/




// Mutex lock
pthread_mutex_t mutex;
// Barrier is used to stop thread's last execution.	   
pthread_barrier_t barrier; 



// Shared variable between different threads.
int SharedVariable = 0;  	

void *SimpleThread(void *args) {
	// Thread ID is passed as arguement.
	int num,val;
	// Thread number.
	long which = (long)args;	

	for(num = 0; num < 20; num++) {	
		#ifdef PTHREAD_SYNC 
		if(random() > RAND_MAX / 2)
			usleep(500);
		#endif
		// Entering critical section and SharedVariable is now locked.
		pthread_mutex_lock(&mutex);	
		val = SharedVariable;
		printf("*** thread %ld sees value %d\n", which, val);
		SharedVariable = val + 1;
		// Exiting the critical section and SharedVariable is now unlocked.
		pthread_mutex_unlock(&mutex); 
	}	
	
	// Prevent thread's last execution so thread sees final ShavedVariable value
	pthread_barrier_wait(&barrier);
	val = SharedVariable;
	// Thread's last execution.
	printf("Thread %ld sees final value %d\n", which, val);	
}

int ValidateNumber(char num[]) {
	// Checks the char array if each char is a digit between 0 to 9
	// Returns 1 if true; 0 if false
	int i;
	for(i = 0; i < strlen(num); i++) {
		if (num[i] < 48 || num[i] > 57) 
		return 0;
	}
	return 1;
}

int main (int argc, char *argv[]) {
	// Validate Input
	if(argc != 2) {	
		printf("Invalid number of arguments.\n");
		return -1;
	}
	
	if(!ValidateNumber(argv[1])) {
		printf("Not a valid positive integer.\n");
		return -1;
	}
	
	// Get the int version of char array using atoi
 	int num_threads = atoi(argv[1]); 
 	if (num_threads <= 0) { 
 		printf("Not a positive integer or value is greater than 2^31.\n");
		return -1;
	}
	
	// Create num_threads of threads
	pthread_t threads[num_threads]; 
	// return code is used to store error code.
	int return_code;
	long i;
	
	// If any error pops up when using barrier1.
	return_code = pthread_barrier_init(&barrier, NULL, num_threads);	

	// strerror is used to check string error.

	if (return_code) {
	    fprintf(stderr, "pthread_barrier_init: %s\n", strerror(return_code));  
	    return -1;
	}

	
	for (i = 0; i < num_threads; i++) {
	    //printf("creating thread %ld\n", i);
	    return_code = pthread_create(&threads[i], NULL, SimpleThread, (void* )i);
	    // If there is an error we exit the program 
	    if (return_code) {
	      printf("ERROR: return code from pthread_create() is %d\n", return_code);
				return -1;
	  	}
	}
	
	// Synchronize and join all threads. 
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}    
	
	// Destroy mutex object
	pthread_mutex_destroy(&mutex);
	return 0;
}
