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

int checkSTOP(char *str) {
    if(strcmp(str, "STOP\n") == 0) {
        quit = 1;
        return 1;
    }
    return 0;
}

char *getBuff1(){
  // Lock the mutex before checking if the buffer has data
  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  char *line = calloc(strlen(buffer_1[con_idx_1] + 1), sizeof(char));
  strcpy(line, buffer_1[con_idx_1]);
  // Increment the index from which the item will be picked up
  con_idx_1 += 1;
  count_1 -= 1;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);
  // Return the item
  return line;
}

void putBuff1(char *str) {
    pthread_mutex_lock(&mutex_1);

    // Put the item in the buffer
    strcpy(buffer_1[prod_idx_1], str);
    // Increment the index where the next item will be put.
    prod_idx_1 += 1;
    count_1 += 1;
    // Signal to the consumer that the buffer is no longer empty
    pthread_cond_signal(&full_1);

    pthread_mutex_unlock(&mutex_1);
}

void putBuff2(char *str){
  // Lock the mutex before putting the item in the buffer
  pthread_mutex_lock(&mutex_2);
  // Put the item in the buffer
  strcpy(buffer_2[prod_idx_2], str);
  // Increment the index where the next item will be put.
  prod_idx_2 += 1;
  count_2 += 1;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}

//Testing purposes only
void printBuff() {
    for(int i = 0; i < count_2; i++) {
        printf("%d: %s", i, buffer_2[i]);
    }
}

void *getInput(void *args) {
    char temp[1000] = {"\0"};
    while(fgets(temp, MAX_LENGTH, stdin) != NULL) {
        putBuff1(temp);
        if(checkSTOP(temp))
            break;
    }

    return NULL;
}

void replaceNewLine(char *cmd) {
    char buffer[1000] = {"\0"};
    char *p = cmd;
    char *space = " ";
    
    while ((p = strstr(p, "\n"))) {
        strncpy(buffer, cmd, p - cmd);
        strcat(buffer, space);
        strcat(buffer, p+strlen("\n"));
        strcpy(cmd, buffer);
        p++;
    }
  
}


void *filterNewLine(void *args) {
    char *temp;
    int i = 0;
    while(1) {
        temp = getBuff1();
        if(checkSTOP(temp))
            break;
        replaceNewLine(temp);
        putBuff2(temp);
    }
    printBuff();
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
