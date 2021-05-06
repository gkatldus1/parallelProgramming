#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //1. 자신의 짝을 찾도록 구현. 짝수 - r +1, 홀수 = r-1
    int pair;
    if (rank % 2 == 0) pair = rank + 1;
    else pair = rank -1;
    
    
    //2. 나의 짝이 < size
    if (pair < size)
    {
	// 2-1. 짝수에서 send, 홀수 recv
	if (rank %2 == 0)
	{
		char * message = "Hello World!\n";
		printf("Send message from %d to %d \n", rank, pair);
		MPI_Send(message, 16, MPI_CHAR, pair, 0, MPI_COMM_WORLD);
	}
	else
	{
		char msg[16];
		MPI_Status status;
		MPI_Recv(msg, 16, MPI_CHAR, pair, 0, MPI_COMM_WORLD, &status);
		
		printf("Recv message me: %d from: %d msg = %s\n", rank, pair, msg);



	}	
	
    }





    
    

    // Finalize the MPI environment.
    MPI_Finalize();
}
