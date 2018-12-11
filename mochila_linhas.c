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
	for(i=1 ; i <= N ; i++)
		fscanf(pont_arq, "%d, %d\n", &valor[i], &peso[i]);
	fclose(pont_arq);
	return;
}

void enviar(int *p, int t[N+1][C+1], int b, int i, int size){
	if(i == N){
		return;
	}
	int pacote[2];
	pacote[0] = t[i][b];
	if(p[i] > b){
		pacote[1] = -99;
	}else{
		pacote[1] = t[i][b-p[i]];
	}
	MPI_Send(pacote, 2, MPI_INT, (i+1)%size, i+1, MPI_COMM_WORLD);
	return;
}

void receber(int *p, int t[N+1][C+1], int b, int i, int size, MPI_Status* status){
	if(i == 0){
		return;
	}
	if(i == 1){
		t[0][b] = 0;
		return;
	}
	int pacote[2];
	MPI_Recv(pacote, 2, MPI_INT, (i-1)%size, i, MPI_COMM_WORLD, status);
	t[i-1][b] = pacote[0];
	t[i-1][b-p[i-1]] = pacote[1];
}


int main(int argc , char *argv[]){
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
	int b,i,j;

	for(i=0;i<N+1;i++){
		for(j=0;j<C+1;j++){
			t[i][j] = 0;
		}
	}
	if(rank == 0){
		parser(p,v);
		p[0] =v[0]= 4;
		p[1] =v[1]= 5;
		p[2] =v[2]= 3;
		p[3] =v[3]= 1;
		p[4] =v[4]= 2;
	}
	MPI_Bcast(p, N+1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(v, N+1, MPI_INT, 0, MPI_COMM_WORLD);

	for(i=rank;i<=n;i = i+size){
		if (i != 0){
			for(b=0;b<=c;b++){
				
				receber(p, t, b, i, size, &status);	
				if(p[i-1]>b){
					t[i][b] = t[i-1][b];
				}else{
					t[i][b] = max(t[i-1][b],t[i-1][b-p[i-1]] + v[i-1]);
				}
				enviar(p, t, b, i, size);
			}
		}
	}

//comunicação de volta funcionando corretamente
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank != 0){
		for(i=rank;i<=n;i= i + size){
			if(i%size != 0){
				MPI_Send(&t[i][0], C+1, MPI_INT, 0, i, MPI_COMM_WORLD);
				for (int d = 0; d <= C; ++d){
				}		
			}
		}
	}

	if(rank == 0){
		for(i=1;i<=n;i++){
			if(i%size != 0){
				MPI_Recv(&t[i][0], C+1, MPI_INT, (i)%size, i, MPI_COMM_WORLD, &status);
			}
		}
		int itens[N+1];
		b=c;
		for(i=n; i>0;i--){
			if(t[i][b] != t[i-1][b]){
				itens[i-1]=1;
				b = b - p[i - 1];
				printf("%d ... \n",p[i-1]);
			}
		}
		for (int i = 0; i < N+1; ++i){
			for (j = 0; j < C+1; ++j){
				printf("%d ",t[i][j] );
			}
			printf("\n");
		}
	}
//-------------------------------------------------
	return 0;
}

 //lamboot 
 //mpicc mochila_linhas.c -o exe.o && mpirun -np 4 exe.o
