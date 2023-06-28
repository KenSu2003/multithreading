#include <stdio.h>
#include <unistd.h>
#include <time.h>
int main(){
     /* This code prints and save time taken by the sleep function; Time is measured using MONOTONIC CLOCK */
    struct timespec start, end;
    unsigned int sec = 1;
    long time_nanosec;
    long time_microsec;

    // Open a file to write time values
    FILE *fptr;
    fptr = fopen("timefile.txt", "w");
    
    int i = 0;
    while(i<10) // Print and Save 10 time values
    {
    clock_gettime(CLOCK_MONOTONIC, &start); // record start time
    sleep(sec);
    clock_gettime(CLOCK_MONOTONIC, &end); // record end time
    time_nanosec = (end.tv_sec - start.tv_sec)*1000000000 + (end.tv_nsec - start.tv_nsec);
        
    // Convert time to microseconds
    time_microsec = time_nanosec/1000;

    // Print time values on terminal
    printf("\n Time taken by sleep is: %ld microseconds (%ld nanoseconds)\n", time_microsec, time_nanosec);
    fprintf(fptr,"%ld,\n", time_microsec);
    i++;
    }
    fclose(fptr);
    return 0;
}