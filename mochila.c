#include <stdio.h>
 //Compiler version gcc 6.3.0
#define N 5
#define C 5



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

 int main(void)
 {
 	int n = N;
 	int c = C;
 	int p[N+1];
 	int v[N+1];
 	int t[N+1][C+1];
 	int itens[N+1];

 	parser(p,v);
 	p[0] =v[0]= 4;
 	p[1] =v[1]= 5;
 	p[2] =v[2]= 3;
 	p[3] =v[3]= 1;
 	p[4] =v[4]= 2;
 	
 	int b,i;
 	for(b=0;b<=c;b++){
 		t[0][b]=0;
 		for(i=1;i<=n;i++){
 			if(p[i]>b){
 				t[i][b] = t[i-1][b];
 			}else{
 				t[i][b] = max(t[i-1][b],t[i-1][b-p[i]] + v[i]);
 				printf("%d i == %d, pi = %d\n",b-p[i], i, p[i] );
 			}
 		}
 	}
 	b=c;
 	for(i=n; i>0;i--){
 		if(t[i][b] != t[i-1][b]){
 			itens[i-1]=1;
 			b = b - p[i - 1];
 			printf("%d ... \n",p[i-1]);
 		}
 	}
 	printf("%d",t[n][c]);
 	return 0;
 }