/* Threading application */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Header file for sleep()
#include <pthread.h> // Header file for thread library

// A normal C function that will be executed as a thread
// note that both the input and output are pointers of void data-type
void* threadFuncOne(void *var){
        /* This function only prints the input argument along with thread id*/
        int* input = (int *) var;             // casting input argument type to integer
        pthread_t thread_id = pthread_self(); // this function takes no arguments and returns currents thread's ID
        sleep(1);                             // pause the program for 1 second
        // printf("Received %d inside thread with ID: %u\n", *input, thread_id);

        // print integers from 0-9
        for(int i=1; i<=100; i++){
                printf("Printing %d inside thread with ID: %u\n", i, thread_id);
        }

        return NULL;
}

void* threadFuncTwo(void *var){
        /* This function prints the integers from 0-9 alongwith thread id*/
        pthread_t thread_id = pthread_self(); // this function takes no arguments and returns currents thread's ID
        sleep(1);

        // print integers from 0-9
        for(int i=1; i<=100; i++){
                printf("Printing %d inside thread with ID: %u\n", i, thread_id);
        }
        
        return NULL;
}

int main() {
        int data = 6;
        pthread_t thread_id, thread_id2;// instantiating argument required for thread creation
        printf("Before Thread\n");

        pthread_create(&(thread_id), NULL, threadFuncOne, (void*)(&data) ); // starts the first thread
        pthread_create(&(thread_id2), NULL, threadFuncTwo, (void*)(&data) ); // starts the second thread

        pthread_join(thread_id, NULL);  // waits for the thread to finish
        pthread_join(thread_id2, NULL);  // waits for the thread to finish       

        // note: always finish main thread using this function, otherwise default thread exit will terminate the whole program
        // whether or not other threads of the program are finished
        printf("After Threads \n");
        pthread_exit(0);
}