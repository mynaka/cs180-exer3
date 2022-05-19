#define _GNU_SOURCE
#include "threadmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>         //for getting system info like number of CPUs

int get_cpu_count(){
    return sysconf(_SC_NPROCESSORS_ONLN); // Get the number of CPUs.
}

MATRIX **array_init_zero(int size){
    MATRIX **arr = (MATRIX**)malloc(size * sizeof(MATRIX*));
    for(int row = 0; row < size; row++){
        arr[row] = (MATRIX*)malloc(size * sizeof(MATRIX));
        for(int col = 0; col < size; col++){                    //initialize initial array fields
            arr[row][col].row = row;
            arr[row][col].column = col;
            arr[row][col].element = 0;
        }
    }
    return arr;
}

MATRIX **array_init_random(int size){
    MATRIX **arr = (MATRIX**)malloc(size * sizeof(MATRIX*));
    for(int row = 0; row < size; row++){
        arr[row] = (MATRIX*)malloc(size * sizeof(MATRIX));
        for(int col = 0; col < size; col++){                    //initialize initial array fields
            arr[row][col].row = row;
            arr[row][col].column = col;
            arr[row][col].element = (rand()%10)-5;
        }
    }
    return arr;
}

void *fillSubmatrices(void* args){
    INITARGS* temp;
    temp = (INITARGS*) args;
    cpu_set_t cpuset;
    CPU_SET(temp->cpu, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);    //set core affinity
    int start = temp->index;
    int end = (temp->index)+(temp->add);
    int tempRow, tempCol;
    for(int i = start; i < end; i++){
        tempRow = i / temp->size;                                   //set 2d matrix indices
        tempCol = i % temp->size;

        temp->arr[tempRow][tempCol].row = tempRow;                  //set matrix fields
        temp->arr[tempRow][tempCol].column = tempCol;
        temp->arr[tempRow][tempCol].element = 0;
    }
    pthread_exit(NULL);
}

MATRIX **array_init_zero_parallel(int size){
    int p = get_cpu_count();
    int array_size = size * size;
    int subarray_size = array_size / p;
    int excess_size = array_size % p;      //excess elements
    int array_index = 0;    //shows index in 2d array = (row*size)+column
    
    MATRIX **arr = (MATRIX**)malloc(size * sizeof(MATRIX*));
    for(int row = 0; row < size; row++){
        arr[row] = (MATRIX*)malloc(size * sizeof(MATRIX));
    }
    pthread_t* tid = (pthread_t*)malloc(p * sizeof(pthread_t));    //initialize POSIX Thread
    cpu_set_t cpuset;
    INITARGS argument[p];
    for(int i = 0; i < p; i++){
        argument[i].cpu = i;
        argument[i].arr = arr;
        argument[i].size = size;
        argument[i].index = array_index;        //set fields

        if(i < excess_size) argument[i].add = subarray_size + 1;   //argument add depnding on the index
        else argument[i].add = subarray_size;                  //excess elements will be redistributed
        pthread_create(&tid[i], NULL, fillSubmatrices, (void *)&argument[i]);
        array_index += argument[i].add;    //change array starting index
    }
    for(int i = 0; i < p; i++){
		//joins the threads so that the thread will wait for the others
		pthread_join(tid[i], NULL);
	}
    return arr;
}