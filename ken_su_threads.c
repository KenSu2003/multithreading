#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/epoll.h>

// Define constants
//                  (0 —> 10) x 100                //
#define SIZE 10                                    // samples per period
#define START 0                                    // START = 0
#define TARGET 100                                 // TARGET = 100 

// Initialize variables
int count = 0;                                          // Tracks number of character in the buffer
long timestamps[SIZE];                                  // Store recored times
long periods[TARGET];                                   // Store periods
int periods_index = 0;                          

// Vairable to identify lock
pthread_mutex_t lock;


void* inputThread(void* input) {

    int tests = 0;
    while(tests<TARGET){
        
        /* ————————————————————————————————————————Record Time ————————————————————————————————————— */

        /* This code prints and save time taken by the sleep function; Time is measured using MONOTONIC CLOCK */
        struct timespec stamp;
        long time_nanosec;
        long time_microsec;

        // Open a file to write time values
        // FILE *fptr;
        // fptr = fopen("timefile.txt", "w"); 
        

        /* ————————————————————————————————— Configure to Interrupt ———————————————————————————————— */
        
        /* Configure */
        int gpio_number = 67;                           // GPIO P8_08

        // the following code can be used to receive interrupts on the registered pin
        char InterruptPath[40];
        sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_number);    // recieve the interrupts
        int epfd;
        struct epoll_event events, ev;

        // (Step 1) open the interrupt file
        FILE* fp = fopen(InterruptPath, "r");
        int fd = fileno(fp);
        // (Step 2) create epoll instance to monitor I/O events on interrupt file
        epfd = epoll_create(1);
        // (Step 3) register events that will be monitored 
        ev.events = EPOLLIN | EPOLLET;
        // (Step 4) register interrupt file with epoll interface for monitoring
        ev.data.fd = fd;
        epoll_ctl(epfd, EPOLL_CTL_ADD,fd,&ev);   


        // Wait if the buffer is empty
        pthread_mutex_lock(&lock);

        // If buffer is full: release the lock so that other thread can empty the buffer
        while (count == SIZE){
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);
        }

        for(int i=0; i<SIZE; i++){

             // (STEP 5) wait for epoll interface to signal the change in available 
            int interrupt = epoll_wait(epfd, &events, 1, -1);     // if an interrupt is detected, interrupt=1
            clock_gettime(CLOCK_MONOTONIC, &stamp); // record start time

            // Print if signal was interrupted
            // printf("\nInterrupt received: %d\n", interrupt);

            // Time Conversion
            time_nanosec = (stamp.tv_nsec);
            time_microsec = time_nanosec/1000;

            // Store time in timestamps
            timestamps[i] = time_microsec; //getIndex      
            count++;   
        }

        // (STEP 6) close the epoll interface
        close(epfd);

        tests++;
        
        pthread_mutex_unlock(&lock);
        
      
    }
    
}


void* outputThread(void* input){

    // Open a file to write period values
    FILE *fptr;
    fptr = fopen("ken_su_pwmperiods.txt", "w");

    int tests = 0;
    while(tests<TARGET){

        // Wait if the buffer is empty
        pthread_mutex_lock(&lock);

        // if buffer is empty; release the lock so that other thread can fill the buffer
        while(count == 0){
            pthread_mutex_unlock(&lock);
            pthread_mutex_lock(&lock);        
        }


        // Calculate and Store the Period
        long periods = 0;
        for (int i=0; i<SIZE-1; i++){
            if(timestamps[i+1] - timestamps[i] < 0){
                periods += 1000000 - (timestamps[i] - timestamps[i+1]);
            } else {
                periods += (timestamps[i+1] - timestamps[i]);
            }
            count--;
        }
        count--;
        long avg_period = periods/(SIZE-1);
        printf("Period = %ld\n",avg_period);
        fprintf(fptr,"%ld\n", avg_period);

        tests++;

        // releasing the lock; marking the end of critical section
        pthread_mutex_unlock(&lock);

     
        if(tests == TARGET){
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