#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 200
static sem_t connected_lock;
static sem_t operators;
static int NUM_OPERATORS = 2;
static int NUM_LINES = 5;
static int connected = 0;    // Callers that are connected

// A global variable next_id exists and is properly updated in the thread function 
// and used to set the caller’s id.
static int next_id = 0;

void* phonecall(void* vargp) {
    pthread_detach(pthread_self());
    // Print that an attempt to connect has been made.
    sem_wait(&connected_lock);
    int callerID = ++next_id;
    sem_post(&connected_lock);
    printf("\033[1;33mThread %i\033[0m is attempting to connect ...\n", callerID);
    // Check if the connection can be made:
    sem_post(&connected_lock);
    while (connected == NUM_LINES) {
    // If the line is busy, exit the critical section and try again in 1 second.
        sem_post(&connected_lock);
        sleep(1);
        sem_wait(&connected_lock);
    }
    // If the line is not busy, update connected, exit the critical section, and print a message, and continue to the next step.
    connected++;
    printf("\033[1;32mThread %i\033[0m connects to an available line, call ringing ...\n", callerID);
    sem_post(&connected_lock);
    // Wait for an operator to be available (using a counting semaphore).
    sem_wait(&operators);
    printf("\033[1;32mThread %i\033[0m is speaking to an operator.\n", callerID);
    sleep(1);
    printf("\033[1;32mThread %i\033[0m has proposed a question for candidates! The operator has left ... \n", callerID);
    sem_post(&operators);
    // Update connected (using a binary semaphore).
    sem_wait(&connected_lock);
    connected--;
    sem_post(&connected_lock);
    // Print a message that the call is over.
    printf("\033[0;31mThread %i\033[0m has hung up!\n", callerID);
}

void* timerThread (void* vargp) {
    // cast void* to int to get totalTime of debate in seconds.
    int totalTime = *((int*) (vargp));
    sleep(totalTime);
    // No detach because main thread will join.
}

int main(int argc, char* argv[]) {
    // Check that we have enough arguments.
    if (argc != 2) {
        printf("Need one input for total time.\n");
        return 0;
    }
    sem_init(&connected_lock, 0, 1);
    sem_init(&operators, 0, NUM_OPERATORS);
 
    pthread_t threadIDs[NUM_THREADS];
    pthread_t timer;
    // The total number of phone calls is 200, but the total seconds of the debate is determined 
    //     as being passed in as a command line argument. 
    int totalTime = atoi(argv[1]);
    // Your program then writes a loop calling pthread_create given the phonecall thread function 
    //     and each corresponding entry of the array of thread id’s. 
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threadIDs[i], NULL, phonecall, NULL) != 0) {
            perror("Could not create a thread");
        }
    }
    // To do that, you need to create another thread as a timer and have the main thread 
    //     use pthread_join on this thread. 
    if (pthread_create(&timer, NULL, timerThread, &totalTime) != 0) {
        perror("Could not create timer thread");
    } 
    // When the timer thread finishes, your main thread needs to terminate all threads.
    if (!pthread_join(timer, NULL)) {
        // Join is successful; destroy semaphores.
        sem_destroy(&connected_lock);
        sem_destroy(&operators);
        printf("Timer done. Debate over!\n");
        return 0;
    } else {
        perror("Could not join timer thread");
        return -1;
    }
}