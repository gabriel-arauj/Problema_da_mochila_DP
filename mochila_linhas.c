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

void enviar(int remetente, int dest, int *p, int t[N+1][C+1], int b, int i, int size, int rank){
	if(remetente == 0){
		t[0][b] = 0;

		return;
	}
	if(dest == N + 1){
		printf("passou aq\n");
		return;
	}
	int pacote[2];
	pacote[0] = t[i][b];
	pacote[1] = -1;
	if(p[i-1] <= b){
		pacote[1] = t[i][b-p[i-1]];
	}
	printf("enviar %d COD = %d\n", i, dest);
	MPI_Send(pacote, 2, MPI_INT, dest%size, i+1, MPI_COMM_WORLD);
	return;
}

void receber(int remetente, int dest, int t[N+1][C+1], int *p, int b, int i, int size, MPI_Status* status){
	if(remetente == -1){
		t[0][b] = 0;
		return;
	}
	if(dest == 1){
		return;
	}
	int pacote[2];
	printf("receber %d cod = %d\n", i, remetente);
	MPI_Recv(pacote, 2, MPI_INT, (remetente)%size, i, MPI_COMM_WORLD, status);
	t[i-1][b] = pacote[0];
	if (pacote[1] == -1){
		return;
	}
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
	int b,i;

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
	MPI_Bcast(v, N+1, MPI_INT, 0, MPI_COMM_WORLD);
 	//printf("%d\n", p[2]);
	printf("%d\n", &t[0][0]);
	for(i=rank;i<=n;i = i+size){
		for(b=0;b<=c;b++){
			//printf("%d\n", rank);
			enviar(i, i+1, p, t, b, i, size, rank);
			
			receber(i-1, i, t, p, b, i, size, &status);
			
			if(p[i-1]>b){
				t[i][b] = t[i-1][b];
			}else{
				t[i][b] = max(t[i-1][b],t[i-1][b-p[i-1]] + v[i-1]);
			}	
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
