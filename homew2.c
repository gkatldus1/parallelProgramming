#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    
	MPI_Init(NULL, NULL);

	FILE *fp = fopen("input.txt", "r");
	char num_size[2];
	int num;
	int sum = 0;
	int my_part;
  	int *array;
	fscanf(fp, "%d", &num);
	int size;
 	MPI_Comm_size(MPI_COMM_WORLD, &size);
       	if (size > num)
		exit(1);
    
	int rank;
 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   
	
	if (rank == 0)
		{
			
			int recv[size];
			my_part = num /size;
			MPI_Status status[size];
			array = malloc(my_part*sizeof(int));
				
			for(int i = 0; i < my_part; i++)
			{
				fscanf(fp, "%d", &array[i]);
				
			}

			for(int i = 0; i< my_part; i++){
				sum += array[i];
			}
			
			for (int i = 1; i < size; i++){
				MPI_Recv(&recv[i], 4, MPI_INT,i, i, MPI_COMM_WORLD, &status[i]);
				sum += recv[i];
			} 
			
			
			printf("sum : %d \n", sum);
		
			free(array);
		
			

		}	
	else if (rank == (size -1))
		{
			
			int offset;
			int other_size = num / size;
			
			my_part = (num/size) + (num%size);
			
			array = malloc(my_part*sizeof(int));
			for(int i = 0; i < other_size * (size-1); i++)
			{
				fscanf(fp,"%d", &offset);
			}

			
			for(int i = 0; i < my_part; i++)
			{
				fscanf(fp, "%d", &array[i]);
				
			}

			for(int i = 0; i < my_part; i++){
				sum += array[i];
			}
			
			
			MPI_Send(&sum, 4, MPI_INT, 0, rank, MPI_COMM_WORLD);


			MPI_Finalize();
			free(array);
			exit(0);
			


		}	
	else
	{
		
		int offset;
		my_part = num / size;
		
		array = malloc(my_part*sizeof(int));
		for(int i = 0; i < my_part *rank; i++)
		{
			fscanf(fp, "%d", &offset);
		}

		

		for(int i = 0; i < my_part; i++){
			fscanf(fp, "%d", &array[i]);
			
		}
		for(int i = 0; i < my_part; i++){
			sum += array[i];
		}
	

		MPI_Send(&sum, 4, MPI_INT, 0, rank, MPI_COMM_WORLD);

		MPI_Finalize();
		free(array);
		exit(0);
	
	    }





    
    

	    // Finalize the MPI environment.
	    MPI_Finalize();
}
