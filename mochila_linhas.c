#include <stdio.h>
#include <mpi.h>
 //Compiler version gcc 6.3.0
#define N 4
#define C 10


int max(int x, int y){
 	if(x>y){
 		return x;
 	}
 	return y;
}

void parser(int* valor, int* peso){
	int i, j;
	FILE *pont_arq; 
	pont_arq = fopen("dados.txt", "r");
	
	if(pont_arq == NULL){
		printf("Erro na abertura do arquivo!");
		return;
	}

	for(i=0 ; i <= N ; i++)
		fscanf(pont_arq, "%d, %d\n", &valor[i], &peso[i]);
	fclose(pont_arq);
	/*	
	// populando matriz com valor -1
	for(i=0;i<MAXN;i++){
		for(j=0;j<MAXS;j++){
			t[i][j] = -1;
		}
		respostas[i] = 0;
	}
	*/
	return;
}

int main(int argc , char *argv[]){
	//int tam = atoi(argv[1]);
	
	//int **a, **b, **c;
	MPI_Init(&argc, &argv);
	
	int size,rank;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

 	int n = N;
 	int c = C;
 	int p[N+1];
 	int v[N+1];
 	int t[N+1][C+1];
 	
 	

 	
 	//se for rank 0
 	if(rank == 0){
 		parser(p,v);
 		p[0] =v[0]= 4;
 	p[1] =v[1]= 5;
 	p[2] =v[2]= 3;
 	p[3] =v[3]= 1;
 	p[4] =v[4]= 2;
 	}
 	MPI_Bcast(p, N+1, MPI_INT, 0, MPI_COMM_WORLD);
 	//printf("%d\n", p[2]);

 	int b,i;
 	for(b=0;b<=c;b++)
 		t[0][b]=0;
 			
 	for(i=rank;i<=n;i = i+size){
 		if(i == 0){
 			MPI_Send(&t[i][0], C+1, MPI_INT, (rank+1)%size, 10, MPI_COMM_WORLD);
 			MPI_Recv(&t[0][0], C+1, MPI_INT, (rank-1)%size, 10, MPI_COMM_WORLD, &status);
 		}
 		if(i !=0){
 			//t[i][0] = 444444;
	 		MPI_Send(&t[i][0], C+1, MPI_INT, (rank+1)%size, 10, MPI_COMM_WORLD);
	 		MPI_Recv(&t[i-1][0], C+1, MPI_INT, (rank-1)%size, 10, MPI_COMM_WORLD, &status);
	 		//printf("%d rank: %d\n", t[i-1][0], rank );
	 		for(b=0;b<=c;b++){
	 			if(p[i-1]>b){
	 				t[i][b] = t[i-1][b];
	 			}else{
	 				t[i][b] = max(t[i-1][b],t[i-1][b-p[i-1]] + v[i-1]);
	 			}	
	 		}
	 	}
 	}
 	if (rank == 0){
 		for (i = 1; i <= n; ++i)
 		{
 			MPI_Recv(&t[i+size][0], C+1, MPI_INT, (rank-1)%size, 10, MPI_COMM_WORLD, &status);
 		}
 	}
 	MPI_Barrier(MPI_COMM_WORLD);
 	if(rank == 0){
	 	int itens[N+1];
	 	b=c;
	 	for(i=n; i>0;i--){
	 		if(t[i][b] != t[i-1][b]){
	 			itens[i-1]=1;
	 			b = b - p[i - 1];
	 			printf("%d ... \n",p[i-1]);
	 		}
	 	}
 	}
 	//printf("%d rank: %d \n",t[n][c], rank);	
 	
 	return 0;
 }

 //lamboot 
 //mpicc mochila_linhas.c -o exe.o && mpirun -np 4 exe.o
