#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <pthread.h>
#include <signal.h>

#define row 20
#define column 20

pthread_mutex_t lockCreate;
pthread_mutex_t lockSum;

int matrix[row][column] = {0};
int matrixNumber;

void* thread_matrix_creator(){
 
    int i,j,k;
    // it will create and print by number of matrices
    for (k = 1; k <= matrixNumber; k++) {
        pthread_mutex_lock(&lockCreate);
        printf("-------- %d.matrix --------\n",k);
        // create a random matrix
        for (i = 0; i < row; i++) {
            for (j = 0; j < column; j++){
                matrix[i][j] = rand() % 100;
                printf("%d\t",matrix[i][j]);
            }
            printf("\n");
        }
        pthread_mutex_unlock(&lockSum);
    }
    
}

void* thread_matrix_sum(){ 
    int result = 0;
    int i,j,k;
    // summation of matrices
    for (k = 1; k <= matrixNumber; k++){
        pthread_mutex_lock(&lockSum);
        for (i = 0; i < row; i++) {
            for (j = 0; j < column; j++){  
                result += matrix[i][j];   
            }
        } 
        // printing result matrix
        printf("Sum of numbers: %d\n",result);
        pthread_mutex_unlock(&lockCreate);
    }
}

// ctrl_c send a signal
void ctrl_c(){
    printf("Pressed CTRL+C..\n");
    signal(SIGINT, ctrl_c);
    pthread_mutex_destroy(&lockCreate); 
    pthread_mutex_destroy(&lockSum);
    exit(0);
}
// ctrl_z send a signal
void ctrl_z(){
    printf("Pressed CTRL+Z..\n");
    signal(SIGTSTP, ctrl_c);
    pthread_mutex_destroy(&lockCreate); 
    pthread_mutex_destroy(&lockSum);
    exit(0);
}


int main(int argc, const char * argv[]) {

    pthread_t threadCreate;
    pthread_t threadSum;
    // initializing locks for each thread
    pthread_mutex_init(&lockSum, NULL);
    pthread_mutex_init(&lockCreate, NULL);
    pthread_mutex_lock(&lockSum);
    while (1) {
	// running signals in main
        signal(SIGINT, ctrl_c);
        signal(SIGTSTP, ctrl_z);
        printf("Enter a matrix number: ");
        scanf("%d",&matrixNumber);
        if (matrixNumber <= 0) {
            perror("Thread is end..\n");
            exit(1);
        }

        void *status;
	// creating threads with functions
        pthread_create(&threadCreate, NULL, thread_matrix_creator, NULL);
        pthread_create(&threadSum, NULL, thread_matrix_sum, NULL);
        
        pthread_join(threadSum, &status);
        pthread_join(threadCreate, &status);
        
    }
    return 0;

}
