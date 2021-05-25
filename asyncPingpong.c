#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
int main(int argc, char** argv){
	

	int *send_msg;
	int *recv_msg;
	int num = 10;
	int n;
	int i;
	MPI_Status status;
	MPI_Request request;
	

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
	
	MPI_Isend(send_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &request);	


	MPI_Irecv(recv_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &request);
		
	MPI_Wait(&request, &status);

	printf("Message received by rank %d \n", rank);

	
	

	free(send_msg);
	free(recv_msg);


	MPI_Finalize();



}
