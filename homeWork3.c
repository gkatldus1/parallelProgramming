#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

	/*
	과제 공고의 예제만 코드가 정상적으로 돌아갑니다.	
	*/

int main(void){

	
	MPI_Init(NULL, NULL);
	int *recv = malloc(4 * sizeof(int)); //양 옆 프로세스들과 통신 때 주고 받은 값을 넣을 배열
	int *array;	//루트 프로세스가 데이터를 모든 프로세스에 할당할때 데이터를 저장할 배열
	int *answer;	//계산 완료한 값을 저장할 배열
	int size;	//전체 프로세스의 수
	MPI_Comm_size(MPI_COMM_WORLD, &size);	//전체 프로세스의 수 구하기
	int rank;	//해당 프로세스의 id
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	//현재 내 프로세스 아이디 구하기
	FILE *fp = fopen("input.txt", "r");	//데이터를 읽어올 파일 오픈
	int num = 0;	//input파일의 전체 데이터의 개수
	int my_size = 0;//내 프로세스가 할당받을 데이터의 개수
	int temp = 0;	//알고리즘에 임시로 사용할 변수
	
	fscanf(fp, "%d", &num);
	
	if((num%size) == 0){	//나누어 떨어질때의 각 프로세스에 나눠줄 배열 사이즈
		my_size = num /size;
		array = malloc(my_size * sizeof(int));
		answer = malloc(my_size * sizeof(int));	
	}	
	else if((num%size)!=0){	//나누어 떨어지지 않을때의 각 프로세스의 배열 사이즈 
		my_size = (num / (size-1));
		array = malloc(my_size * sizeof(int));
		answer = malloc(my_size * sizeof(int));
		
		
	}
	
	for(int i = 0; i < 4; i ++){	//프로세스 양방향 통신으로 받은 값을 저장할 배열 초기
		recv[i] = 0;
	}
	for(int i = 0; i < my_size; i++){//scatter로 받기 전 각 프로세스의 배열 초기화
		array[i] = -1;
	}
	
	

	int *allNum = NULL;	
	int sendNum = my_size;
	
	if (rank == 0){//파일에서 숫자들 받아옴
		
		allNum = malloc(num*sizeof(int));
	

		for(int i = 0; i < num; i++){
			allNum[i] = -1;
		}
		for(int i = 0; i < num; i++){
			fscanf(fp, "%d", &allNum[i]);		
		}
		
	}


	MPI_Scatter(allNum, sendNum, MPI_INT, array, sendNum, MPI_INT, 0, MPI_COMM_WORLD);
	//받아온 숫자 프로세스들로 나눠줌

	if(rank == 0){	//루트프로세스에서 계산
		MPI_Status status[2];
		int point = 0;		
		int temp = 0;
		int remain = 0;
		MPI_Request request[4];
		//calculating section
		temp = my_size-1;
		
		for(int i = 0; i <2; i++){
			MPI_Send(&array[temp], 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
			//printf("sending values: ");
			//printf("%d,",array[temp]); 
			temp--;
		}
		temp = 3;
		for(int i = 0; i < 2; i++){		
			MPI_Recv(&recv[temp], 1, MPI_INT, rank-1, rank, MPI_COMM_WORLD,&status[i]);
			temp--;
		}
		
		for(int i = 0; i< my_size; i++){
			if(i == my_size-1){
				answer[i] = array[i-2] + array[i-1] + recv[2]+ recv[3];
				continue;
			}
			else if(i == my_size-2){
				answer[i] = array[i-1] + array[i-2] + array[i+1] + recv[2];
				continue;
			}
			else if(i == 0 || i==1){
				answer[i] = 0;
				continue;
			}
			else{
				answer[i] = array[i-1] + array[i-2] + array[i+1] + array[i+2];
				continue;
			}
		}		
		
		
		
	}
	
	if((0<rank)&&(rank<(size-1))){//루트와 마지막 이외의 프로세스에서 계산
		MPI_Status status1[4];
		MPI_Request request;
		
		for(int i = 1; i >= 0; i--){			
			MPI_Recv(&recv[i], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status1[i]);
		}
		for(int i = 1; i >=0; i--){
			MPI_Send(&array[i], 1, MPI_INT, 0, 0, MPI_COMM_WORLD);		
		}
		temp = my_size-2;
		for(int i = 0; i < 2; i++){
			MPI_Send(&array[temp], 1, MPI_INT, rank+1, rank+1, MPI_COMM_WORLD);
			temp++;
		}
		for(int i = 3; i >= 2; i--){
			MPI_Recv(&recv[i], 1, MPI_INT, rank+1, rank, MPI_COMM_WORLD, &status1[i]);
		}
		if((rank==size-2) && (recv[3]==-1)){//마지막 프로세스에 값이 분배되지 않을 때 마지막 이전 프로세스가 마지막 프로세스처럼 계산
			for(int i=0; i < my_size; i++){
					if(i==(my_size-1)){ 
						answer[i] = 0;
						continue;
					}
					else if(i==(my_size-2)){
						answer[i] = 0;
						continue;
					}
					else if(i==0){
						answer[i] = recv[0]+recv[1]+array[i+1]+array[i+2];
						continue;
					}
					else if(i==1){
						answer[i] = recv[1]+array[i-1]+array[i+1]+array[i+2];
						continue;
					}
					else{
						answer[i] = array[i-1]+array[i-2] + array[i+1] + array[i+2];
						continue;
					}
				}
		}
		else{
			for(int i = 0; i < my_size; i++){
				if(i==0){
					answer[i] = recv[0] + recv[1] + array[i+1] + array[i+2];
					continue;		
				}
				else if(i==1){
					answer[i] = recv[1]+array[i-1]+array[i+1]+array[i+2];
					continue;					
				}
				else if(i == my_size-1){
						answer[i] = array[i-2] + array[i-1] + recv[2]+ recv[3];
						continue;
				}
				else if(i == my_size-2){
					answer[i] = array[i-1] + array[i-2] + array[i+1] + recv[2];
					continue;	
				}
			
				else{
					answer[i] = array[i-1]+array[i-2] + array[i+1] + array[i+2];
					continue;
				}
				
			}
		}
		
	}
	//마지막 프로세스가 계산
	if(rank == (size-1)){
			MPI_Status status1[2];
			MPI_Request request;
			
			for(int i = 0; i <= 1; i++){			
				MPI_Recv(&recv[i], 1, MPI_INT, rank-1, rank, MPI_COMM_WORLD, &status1[i]);
			}
			for(int i = 1; i >=0; i--){
				MPI_Send(&array[i], 1, MPI_INT, rank-1, rank-1, MPI_COMM_WORLD);		
			}
			
			if(array[0] != -1){
				for(int i=0; i < my_size; i++){
					if(i==(my_size-1)){ 
						answer[i] = 0;
						continue;
					}
					else if(i==(my_size-2)){
						answer[i] = 0;
						continue;
					}
					else if(i==0){
						answer[i] = recv[0]+recv[1]+array[i+1]+array[i+2];
						continue;
					}
					else if(i==1){
						answer[i] = recv[1]+array[i-1]+array[i+1]+array[i+2];
						continue;
					}
					else{
						answer[i] = array[i-1]+array[i-2] + array[i+1] + array[i+2];
						continue;
					}
				}
				
			}
			else{}				
		
	}
	


	MPI_Gather(answer, my_size, MPI_INT, allNum, my_size, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank == 0){
		for(int i = 0; i < num; i++){
			printf("%d ",allNum[i]);
		}
	}


	MPI_Finalize();
	exit(0);

	
}
