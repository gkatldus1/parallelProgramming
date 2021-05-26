#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
	
	int rank, n_ranks;
	int my_first, my_last;
	int numbers = 10;
	MPI_Request request;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char data[16] = {0};
	if (rank == 0) sprintf(data, "Hello world %d\n", rank);
	printf("rank : %d, message: %s \n", rank, data);
	
	MPI_Bcast(data, 16, MPI_CHAR, 0, MPI_COMM_WORLD);

	printf("After Bcast => rank: %d message : %s\n", rank, data);

	return MPI_Finalize();





}
