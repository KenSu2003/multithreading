#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Defining BUFFER size
#define SIZE 10
char buffer[SIZE];

// Initialize Variables
int count = 0;                                      // Tracks number of character in the buffer
int putIndex =0;                                    // Tracks the buffer iundex for next write
int getIndex = 0;                                   // Tracks the buffer uindex for next read

// Vairable to identify lock
pthread_mutex_t lock;

void* inputThread(void* input) {
    char c = 65;                                    // ASCII character 'A'
    
    /* Write ASCII from 'A':65 bto 'z':122 */
    while (count <= 122){

        // Wait before next write until the buffer has space
        pthread_mutex_lock(&lock);
        while (count == SIZE){

            // If buffer is full: release the lock so that other thread can empty the buffer
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);

        }
        
        // write to buffer and update related variables
        count++;
        buffer[putIndex] = c;
        putIndex++;
        printf("filling buffer: %c\n", c);

        // If the buffer reach its end, the next character will be written at the start of buffer
        if(putIndex == SIZE){
            putIndex = 0;
        }

        // // exit when reaching target
        // if (c > 122) {
        //     printf("break\n");
        //     break;
        // }
        
        // Updating character for next write
        c += 1;
        pthread_mutex_unlock(&lock);
    } 
}


void* outputThread(void* input){
    char c = 0;                                     // initializing using NULL character

    /* Print all character till 'z' */
    while(c <= 122){

        // Wait if the buffer is empty
        pthread_mutex_lock(&lock);

        while(count == 0){

            // if buffer is empty; release the lock so that other thread can fill the buffer
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);
        }

        // read the character from buffer, update variables and print the character to terminal
        count--;
        c = buffer[getIndex]; 
        getIndex++;
        printf("Emptying buffer: %c, %d\n", c, c);

        // return index to zero after reading the last character in the buffer
        if (getIndex == SIZE){
            printf("returning index to zero\n");
            getIndex = 0;
        }

        // releasing the lock; marking the end of critical section
        pthread_mutex_unlock(&lock);

        // exit the thread after printing last character
        if(c == 122){
            printf("break\n");
            break;
        }
    }
}

int main(){

    /* Initialization */
    int data = 6;
    pthread_t thread_id[2];
    printf("Before Thread \n");

    // Lock
    if (pthread_mutex_init(&lock, NULL) != 0){
        printf("\n mutex init has failed \n");
        return 1;
    }

    // Function
    pthread_create(&(thread_id[0]), NULL, outputThread, (void*)(&data));
    pthread_create(&(thread_id[1]), NULL, inputThread, (void*)(&data));

    // blocks the main function (thread) until the thread function is finished its execution
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    // Uninitialize lock
    pthread_mutex_destroy(&lock);

    printf("After Thread \n");
    pthread_exit(0);

}

