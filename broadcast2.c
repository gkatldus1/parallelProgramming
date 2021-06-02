#include <stdio.h>
#include <mpi.h>
void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
	int size, rank;
	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	if(rank == root)
	{
		//다른 모든 프로세스에 send

		int i;
		for(i = 0; i < size; i++)
		{	
			if(i != root)
				MPI_Send(data, count, datatype,i, 0, comm);
		}
	}else {
	  //root에서 보낸 data를 받는다
	  MPI_Recv(data, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);

	}
	


}
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
	
//	MPI_Bcast(data, 16, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	my_bcast(data, 16, MPI_CHAR, 0, MPI_COMM_WORLD);

	printf("After Bcast => rank: %d message : %s\n", rank, data);

	return MPI_Finalize();





}
