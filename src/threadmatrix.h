#define _GNU_SOURCE
#include <stdlib.h>
#include <pthread.h>
//MATRIX CELL
typedef struct matrix{
	int row;
	int column;
	int element;
}MATRIX;

//PTHREAD INIT ARGUMENTS
typedef struct arguments{
	MATRIX **arr;   //array
    int size;       //width and height of the matrix
    int index;      //what index does the subarray start
    int add;        //how many elements to add
    cpu_set_t cpu;  //which cpu is being used
}INITARGS;

/*
*GET CPU COUNT
*requirements: none
*results: returns the number of logical cpus within the computer
*source/s: https://man7.org/linux/man-pages/man3/sysconf.3.html
*/
int get_cpu_count();

/*
*INITIALIZE ZERO ARRAY
*requirements: an integer size determining both the number of rows and columns of the matrix
*result: returns a size by size zero matrix
*/
MATRIX **array_init_zero(int size);

/*
*INITIALIZE ZERO ARRAY
*requirements: fully initialized argument of type INITARGS typecasted to void*
*result: creates fields for submatrices with core affinity
*/
void *fillSubmatrices(void* args);

/*
*INITIALIZE ZERO ARRAY(PARALLEL)
*requirements: an integer 'size' determining both the number of rows and columns of the matrix
*result: returns a 'size' by 'size' zero matrix(implemented through parallelism)
*/
MATRIX **array_init_zero_parallel(int size);