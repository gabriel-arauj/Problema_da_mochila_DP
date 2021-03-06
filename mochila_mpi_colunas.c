#include <stdio.h>
#include <mpi.h>
#define N 1000
#define C 1000



void printT(int t[N+1][C+1]){
	for (int i = 0; i < N+1; ++i){
			for (int j = 0; j < C+1; ++j){
				printf("%d ",t[i][j] );
			}
			printf("\n");
		}
}

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
	valor[0] = peso[0] = -1;
	for(i=1 ; i <= N ; i++)
		fscanf(pont_arq, "%d, %d\n", &valor[i], &peso[i]);
	fclose(pont_arq);
	return;
}

int main(void){
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
 	int itens[N+1];

 	if(rank == 0){
		for(j=0;j<C+1;j++){
			t[0][j] = 0;
		}
		parser(p,v);
		p[0] =v[0]= 4;
		p[1] =v[1]= 5;
		p[2] =v[2]= 3;
		p[3] =v[3]= 1;
		p[4] =v[4]= 2;
		p[5] =v[5]= 3;
	}
	MPI_Bcast(p, N+1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(v, N+1, MPI_INT, 0, MPI_COMM_WORLD);
	int bufsize = MPI_BSEND_OVERHEAD*(N+1)*(C+1)*sizeof(int);
    int* buf = (int *)malloc( bufsize );
    MPI_Buffer_attach( buf, bufsize );
 	
 	int b,i;
 	for(b=rank;b<=c;b += size){
 		t[0][b]=0;
 		for(i=1;i<=n;i++){
 			if(p[i]>b){
 				t[i][b] = t[i-1][b];
 			}else{
 				
 				t[i][b] = max(t[i-1][b],t[i-1][b-p[i]] + v[i]);
 			}
 		}
 	}
 	//comunicação de volta funcionando corretamente mesmo!!!!!!
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank != 0){
		for(i=rank;i<=n;i= i + size){
			if(i%size != 0){
				MPI_Send(&t[i][0], C+1, MPI_INT, 0, i, MPI_COMM_WORLD);
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
		//printT(t);
	}
 	return 0;
 }