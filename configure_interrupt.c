#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/epoll.h>

//configures the gpio pin as input
void configure_gpio_input(int gpio_number){

    //converting gpio number from integer to string
    char gpio_num[10];
    sprintf(gpio_num, "export%d", gpio_number);
    const char* GPIOExport="/sys/class/gpio/export";

    // exporting the GPIO to user space
    FILE* fp = fopen(GPIOExport, "w");
    fwrite(gpio_num, sizeof(char), sizeof(gpio_num), fp);
    fclose(fp);

    // setting gpio direction as input
    char GPIODirection[40];
    sprintf(GPIODirection, "/sys/class/gpio/gpio%d/direction", gpio_number);

    // setting GPIO as input
    fp = fopen(GPIODirection, "w");
    fwrite("in", sizeof(char), 2, fp);
    fclose(fp);

}

// sets up an interrupt on the given GPIO pin
void configure_interrupt(int gpio_number){
    configure_gpio_input(gpio_number);          /// set gpio as input

    // configuring interrupts on rising, falling or both edges
    char InterruptEdge[40];
    sprintf(InterruptEdge, "/sys/class/gpio/gpio%d/edge", gpio_number);

    // configures interrupt on falling edge
    FILE* fp = fopen(InterruptEdge, "w");
    // fwrite("falling", sizeof(char), 7, fp);
    // change from "falling —–> "rising" to detect rising edge
    fwrite("rising", sizeof(char), 7, fp);                 
 
    // configures interrupt on both edges
    // fwrite("both", sizeof(char), 4, fp);

    fclose(fp);
}

int main(){

     /* Configure */
    int gpio_number = 67;
    configure_interrupt(gpio_number);   // enable interrupt
    
    // the following code can be used to receive interrupts on the registered pin
    char InterruptPath[40];
    sprintf(InterruptPath, "/sys/class/gpio/gpio%d/value", gpio_number);
    int epfd;
    struct epoll_event events, ev;

    // (Step 1) open the interrupt file
    // file pointer (C abstraction to manipulate files)
    FILE* fp = fopen(InterruptPath, "r");
    // file descriptor (Unix\Linux file identifier used by system calls)
    int fd = fileno(fp);

    // (Step 2) create epoll instance to monitor I/O events on interrupt file
    epfd = epoll_create(1);

    // (Step 3) register events that will be monitored 
    // detects whenever a new data is available for read (EPOLLIN)
    // signals the read events when the available read value has changed (EPOLLET)
    ev.events = EPOLLIN | EPOLLET;
    
    // (Step 4) register interrupt file with epoll interface for monitoring
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD,fd,&ev);   

    /* Interrupt */
    int interrupt;
    time_t seconds;
    for (int i=0; i<20; i++){

        // (STEP 5) wait for epoll interface to signal the change in available 
        interrupt = epoll_wait(epfd, &events, 1, -1);
        seconds = time(NULL);
        printf("Interrupt received: %d at %ld\n", interrupt, seconds);

    }

    // (STEP 6) close the epoll interface
    close(epfd);
    return 0;
}