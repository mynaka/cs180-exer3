#include "threadmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<sys/time.h>

void main(){
    srand(time(0));
    int cpuCount = get_cpu_count();
    MATRIX **arr;
    int size;
    scanf("%d", &size);
    //set up timer and run function
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    arr = array_init_zero_parallel(size);
    gettimeofday(&stop, NULL);
    double t = (double)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000;
    printf("Time elapsed: %f seconds for size %d\n", t, size);
    for(int i = 0; i < size; i++){
        free(arr[i]);
    }
    free(arr);
}