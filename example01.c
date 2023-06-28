/* Threading application */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Header file for sleep()
#include <pthread.h> // Header file for thread library

// A normal C function that will be executed as a thread
// note that both the input and output are pointers of void data-type
void* threadFunction(void *var){
    int* input = (int *) var;               // casting input argument type to integer
    sleep(1);                               // pause the program for 1 second
    printf("Received %d inside thread argument.\n", *input);
    return NULL;
}

int main() {
    int data = 6;
    pthread_t thread_id;
    printf("Before Thread\n");
    
    // this function creates the thread, OS may or may not immediately start executing this thread will terminate the whole program
    pthread_create(&thread_id, NULL, threadFunction, (void*)(&data) );

    //blocks the main function (thread) until the thread function is finished its execution
    pthread_join(thread_id, NULL);
    printf("After Thread\n");

    // note: always finish main thgread using this funtion otherwise default thread exit will terminate the whole program 
    // whether or not other threads of the program are finished
    pthread_exit(0);
}