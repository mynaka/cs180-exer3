CC = gcc
CFLAGS = -0
default: naive

init:					#run zero matrix initialization program
	$(CC) -pthread -o a initialize.c threadmatrix.c
	./a
	$(RM) a

init_parallel:					#run parallel zero matrix initialization program
	$(CC) -pthread -o a initialize_parallel.c threadmatrix.c
	./a
	$(RM) a

rows:					#run parallel threading by rows
	$(CC) -pthread -o a multiply.c threadmatrix.c
	./a
	$(RM) a

