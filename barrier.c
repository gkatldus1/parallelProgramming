#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	if(rank == 0) sleep(5);

	MPI_Barrier(MPI_COMM_WORLD);
	printf("my rank = %d\n", rank);

	MPI_Finalize();



}
