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
    int cpu;  //which cpu is being used
}INITARGS;

//PTHREAD MULTIPLICATION ARGUMENTS
typedef struct multarguments{
	MATRIX **matA, **matB, **matC;   //matrices
    int size;       //width and height of the matrices
    int row;      //what index does the submatrix start
    int add;        //how many rows to find result on
    int cpu;  //which cpu is being used
}MULTARGS;

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
*INITIALIZE RANDOMIZED ARRAY
*requirements: an integer size determining both the number of rows and columns of the matrix
*result: returns a size by size matrix with random element values
*/
MATRIX **array_init_random(int size);

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

/*
*GET ROWS
*requirements: Arguments containing matrices, cpu identifier, size of matrix, starting row, and number fo rows to compute
*result: compute for values in rows of the resulting matrix specified by arguments
*/
void* getRow(void* args);

/*
*CACHED PARALLEL ROWS MULTIPLICATION
*requirements: three 2d MATRIX arrays; two operands mat A and matB and one resultant matC, integer containing matrix size
*result: multiply matrix A and B(after transposition) and return resulting matrix C
*/
MATRIX** parallel_rows_cached(MATRIX** matA, MATRIX** matB, MATRIX** matC, int size);