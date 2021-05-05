#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv)
{

	int n_ranks = 0, rank = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
	if (n_ranks !=2)
	{
		printf("This example requires. two ranks\n");
		MPI_Finalize();
		return(1);
	}




	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		char *message = "Hello, world!\n";
		MPI_Send(message, 16, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
	}

	if (rank == 1)
	{	
		char message[16];
		MPI_Status status;
		MPI_Recv(message, 16, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		
		printf("%s\n", message);


	}


	MPI_Finalize();


}
