#include <stdio.h>

 //Compiler version gcc 6.3.0
#define N 5
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


int main(int argc , char *argv[]){
	

	int n = N;
	int c = C;
	int p[N+1];
	int v[N+1];
	int t[N+1][C+1];
	int b,i,j;

	
	for(j=0;j<C+1;j++){
		t[0][j] = 0;
	}

 	//se for rank 0

		//parser(p,v);
		p[0] =v[0]= 4;
		p[1] =v[1]= 5;
		p[2] =v[2]= 3;
		p[3] =v[3]= 1;
		p[4] =v[4]= 2;
		p[5] =v[5]= 3;
	

	for(i=1;i<=n;i++){
		for(b=0;b<=c;b++){
			if(p[i-1]>b){
				t[i][b] = t[i-1][b];
			}else{
				t[i][b] = max(t[i-1][b],t[i-1][b-p[i-1]] + v[i-1]);
			}	
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
	
	for (int i = 0; i < N+1; ++i)
	{
		for (j = 0; j < C+1; ++j)
		{
			printf("%d ",t[i][j] );
		}
		printf("\n");
	}
 	//printf("%d rank: %d \n",t[n][c], rank);	

	return 0;
}

 //lamboot 
 //mpicc mochila_linhas.c -o exe.o && mpirun -np 4 exe.o
