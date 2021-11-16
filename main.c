#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Sizes for the buffers
#define MAX_SIZE 50

#define MAX_LENGTH 1000

int quit = 0;

/* This code came from the example program from class */
// Buffer 1, shared resource between input thread and newLine thread
char buffer_1[MAX_SIZE][MAX_LENGTH];
// Number of items in the buffer
int count_1 = 0;
// Index where the input thread will put the next item
int prod_idx_1 = 0;
// Index where the newLine thread will pick up the next item
int con_idx_1 = 0;
// Initialize the mutex for buffer 1
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

// Buffer 2, shared resource between square root thread and output thread
char buffer_2[MAX_SIZE][MAX_LENGTH];
// Number of items in the buffer
int count_2 = 0;
// Index where the newLine thread will put the next item
int prod_idx_2 = 0;
// Index where the plus thread will pick up the next item
int con_idx_2 = 0;
// Initialize the mutex for buffer 2
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

// Buffer 2, shared resource between square root thread and output thread
char buffer_3[MAX_SIZE][MAX_LENGTH];
// Number of items in the buffer
int count_3 = 0;
// Index where the plus thread will put the next item
int prod_idx_3 = 0;
// Index where the output thread will pick up the next item
int con_idx_3 = 0;
// Initialize the mutex for buffer 3
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer 3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

char *getStr() {
    char tempBuff[1000] = {"\0"};
    char *temp;
    while(!fgets(tempBuff, MAX_LENGTH, stdin)) {
        temp = calloc(strlen(tempBuff) + 1, sizeof(char));
        strcpy(temp, tempBuff);
	printf("%s", temp);
    }
    return temp;
}

void *getInput(void *args) {
    printf("HELLO");
    for(int i = 0; i < MAX_SIZE; i++) {
        char *str = getStr();
        //printf("%s", str);
	break;
    }
    return NULL;
}

void *getUserInput(void *args) {
    return NULL;
}

void *filterNewLine(void *args) {
    return NULL; 
}

void *filterPlus(void *args) {
    return NULL;
}

void *writeOutput(void *args) {
    return NULL;
}



int main()
{
    pthread_t input_t, newlines_t, plus_t, output_t;

    //Create files
    pthread_create(&input_t, NULL, getInput, NULL);
    pthread_create(&newlines_t, NULL, filterNewLine, NULL);
    pthread_create(&plus_t, NULL, filterPlus, NULL);
    pthread_create(&output_t, NULL, writeOutput, NULL);
    //Wait for the threads to terminate
    pthread_join(input_t, NULL);
    pthread_join(newlines_t, NULL);
    pthread_join(plus_t, NULL);
    pthread_join(output_t, NULL);
    return EXIT_SUCCESS;
}
