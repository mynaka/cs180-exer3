#define _GNU_SOURCE
#include "threadmatrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>         //for getting system info like number of CPUs
#include <time.h>

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
    INITARGS args[p];
    for(int i = 0; i < p; i++){
        args[i].cpu = i;
        args[i].arr = arr;
        args[i].size = size;
        args[i].index = array_index;        //set fields

        if(i < excess_size) args[i].add = subarray_size + 1;   //argument add depnding on the index
        else args[i].add = subarray_size;                  //excess elements will be redistributed
        pthread_create(&tid[i], NULL, fillSubmatrices, (void *)&args[i]);
        array_index += args[i].add;    //change array starting index
    }
    for(int i = 0; i < p; i++){
		//joins the threads so that the thread will wait for the others
		pthread_join(tid[i], NULL);
	}
    free(tid);
    return arr;
}

void* getRows(void* args){
    MULTARGS* temp;
    temp = (MULTARGS*) args;
    cpu_set_t cpuset;
    CPU_SET(temp->cpu, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);    //set core affinity

    int start = temp->row;
    int end = (temp->row)+(temp->add);
    int size = temp->size;
    int column;
    MATRIX** matA = temp->matA;
    MATRIX** matB = temp->matB;
    MATRIX** matC = temp->matC;

    for(int i = start; i < end; i++){
        column = 0;
	    for(int j = 0; j < size; j++){
            for(int k = 0; k < size; k++){
                matC[i][j].element += (matA[i][k].element * matB[j][k].element);
	        }
        }
    }
    pthread_exit(NULL);
}
MATRIX** parallel_rows_cached(MATRIX** matA, MATRIX** matB, MATRIX** matC, int size){
    int p = get_cpu_count();
    int submatrixsize = size / p;
    int excess_size = size % p;
    int row_index = 0;
    pthread_t* tid = (pthread_t*)malloc(p * sizeof(pthread_t));
    MATRIX** BTransposed = (MATRIX**)malloc(size * sizeof(MATRIX*));
    for(int i = 0; i < size; i++){
        BTransposed[i] = (MATRIX*)malloc(size * sizeof(MATRIX));
        for (int j = 0; j < size; j++){
            BTransposed[i][j].row = i;
            BTransposed[i][j].column = j;
            BTransposed[i][j].element = matB[j][i].element;            
        }
    }

    MULTARGS args[p];
    for(int i = 0; i < p; i++){
        args[i].size = size;
        args[i].row = row_index;
        args[i].cpu = i;
        args[i].matA = matA;
        args[i].matB = BTransposed;
        args[i].matC = matC;

        if(i < excess_size) args[i].add = submatrixsize + 1;   //argument add depnding on the index
        else args[i].add = submatrixsize;                  //excess elements will be redistributed
		pthread_create(&tid[i], NULL, getRows, (void *)&args[i]);
        row_index += args[i].add;    //change array starting index
	}

	//join the threads
	for(int i = 0; i < p; i++){
		//joins the threads so that the thread will wait for the others
		pthread_join(tid[i], NULL);
	}
    for(int i = 0; i < size; i++){
        free(BTransposed[i]);
    }
    free(BTransposed);
    free(tid);  //free pthreads
}