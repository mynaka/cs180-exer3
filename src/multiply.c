#define _GNU_SOURCE
#include "threadmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

int main(){
    srand(time(0));
    int size;
    scanf("%d", &size);
    MATRIX** matA = array_init_random(size);
    MATRIX** matB = array_init_random(size);
    MATRIX** matC = array_init_zero_parallel(size);

    //set up timer and run function
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    parallel_rows_cached(matA, matB, matC, size);
    gettimeofday(&stop, NULL);
    double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
    /*    
    printf("\n");
    for(int i = 0; i < size; i++){               //show results
        for (int j = 0; j < size; j++){
            printf("%3i ",matA[i][j].element);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < size; i++){               //show results
        for (int j = 0; j < size; j++){
            printf("%3i ",matB[i][j].element);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i < size; i++){               //show results
        for (int j = 0; j < size; j++){
            printf("%3i ",matC[i][j].element);
        }
        printf("\n");
    }
    */
    printf("N = %d, Process took %f seconds\n",size, t);
    for(int i = 0; i < size; i++){
        free(matA[i]);
        free(matB[i]);
        free(matC[i]);
    }
    free(matA);
    free(matB);
    free(matC);
}