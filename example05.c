#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Defining BUFFER size
#define SIZE 26
char buffer[SIZE];

// Initialize Variables
int count = 0;                                      // Tracks number of character in the buffer
int putIndex =0;                                    // Tracks the buffer iundex for next write
int getIndex = 0;                                   // Tracks the buffer uindex for next read

void* inputThread(void* input) {
    char c = 65;                                    // ASCII character 'A'
    
    /* Write ASCII from 'A':65 bto 'z':122 */
    while (count <= 122){

        // Wait before next write until the buffer has space
        while (count == SIZE){
        }
        
        // write to buffer and update related variables
        buffer[putIndex] = c;
        putIndex++ ;
        count++;

        // If the buffer reach its end, the next character will be written at the start of buffer
        if(putIndex == SIZE){
            putIndex = 0;
        }
        
        // Updating character for next write
        c += 1;
    }
}

void* outputThread(void* input){
    char c = 0;                                     // initializing using NULL character

    /* Print all character till 'z' */
    while(c <= 122){

        // Wait if the buffer is empty
        while(count == 0){
        }

        // read the character from buffer, update variables and print the character to terminal
        count--;
        c = buffer[getIndex];
        getIndex++;
        printf("%s\n", c);

        // return index to zero after reading the last character in the buffer
        if (getIndex == SIZE){
            getIndex = 0;
        }

        // exit the thread after printing last character
        if(c == 122){
            break;
        }
    }
}

int main(){

    /* Initialization */
    int data = 6;
    pthread_t thread_id[2];
    printf("Before Thread \n");

    // Function
    pthread_create(&(thread_id[1]), NULL, outputThread, (void*)(&data));
    pthread_create(&(thread_id[0]), NULL, inputThread, (void*)(&data));

    // blocks the main function (thread) until the thread function is finished its execution
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);

    printf("After Thread \n");
    pthread_exit(0);
}


