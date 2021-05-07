#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char** argv){
	

	int *send_msg;
	int *recv_msg;
	int num = 1000000000;
	int n;
	int i;
	MPI_Status status;
	clock_t start;
	clock_t end;
	long *time;
	int loop = 1;
	long sum = 0;
	time = malloc(loop*sizeof(long));
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
	
	for(i = 0; i< loop; i++)
	{	
		if (rank == 0)
		{	
			start = clock();	
	
			MPI_Send(send_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD);	
			free(send_msg);
			MPI_Recv(recv_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &status);
			end = clock();
			free(recv_msg);
			time[i] = end - start;
			free(time);
		}
	
		if(rank == 1)
		{	
			MPI_Recv(recv_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD, &status);
			free(recv_msg);
			MPI_Send(send_msg, num, MPI_INT, n, 0, MPI_COMM_WORLD);
			free(send_msg);
		}
//		for(int i = 0; i < num; i++){
//			printf("rank:%d, recvmessage: %d\n", rank, recv_msg[i]);
//		}

	}
	if(rank == 0)
	{	
		for(i=0; i< loop; i++)
		{	
			sum += time[i];
		}
		printf("average: %ld\n", sum/loop);
	}

//	free(send_msg);
//	free(recv_msg);
//	free(time);

	MPI_Finalize();



}
