#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char** argv){
	

	int *send_msg;
	int *recv_msg;
	int num = 3;
	int n;
	int i;
	MPI_Status status;



	send_msg = malloc(num*sizeof(int));
	recv_msg = malloc(num*sizeof(int));
	MPI_Init(NULL,NULL);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if (rank == 0) n = 1;
	else n = 0;
	
	for(i = 0; i < num; i++)
		send_msg[i] = i* (rank+1); 
	
	if (rank == 0)
	{
		MPI_Send(send_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD);	
		MPI_Recv(recv_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &status);
	}
	else
	{
		MPI_Recv(recv_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &status);
		MPI_Send(send_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD);
	}
	for(int i = 0; i < num; i++){
		printf("rank:%d, recvmessage: %d\n", rank, recv_msg[i]);
	}

	
	

	free(send_msg);
	free(recv_msg);


	MPI_Finalize();



}
