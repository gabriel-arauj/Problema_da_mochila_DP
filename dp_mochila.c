#include <algorithm>
#include <stdio.h>

#define MAXN 5
#define MAXS 5
#define N 3

int t[MAXN][MAXS];
int valor[MAXN];
int peso[MAXN];
int respostas[MAXN];
int tamVetor = N;

void parser(){
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
	
	// populando matriz com valor -1
	for(i=0;i<MAXN;i++){
		for(j=0;j<MAXS;j++){
			t[i][j] = -1;
		}
		respostas[i] = 0;
	}
	return;
}

void mochila(int n, int c){
	int b, i;
	int a, a_n;
	for(b = 0; b <= c; b++){
		t[0][b] = 0;
		for(i = 1; i <= n; i++){
			a = t[i-1][b];
			if(peso[i-1] > b)
				a_n = 0;
			else
				a_n = t[i-1][b-peso[i-1]] + valor[i-1];
			t[i][b] = std :: max(a, a_n);
		}
	}
	b = c;
	for(i = n; i>= 1; i--){
		if(t[i][b] != t[i-1][b]){
			respostas[i] = 1;
			b = b - peso[i-1];
		}else
		respostas[i] = 0;
	}
}

int main(int argc, char const *argv[]){
	parser();
	valor[0] = 1;
	valor[1] = 3;
	valor[2] = 2;

	peso[0] = 4;
	peso[1] = 2;
	peso[2] = 1;

	int resp= 0;
	mochila(3, 3);
	//printf("resposta: %d\n", resp);
	for (int i = 0; i < N; ++i){
		if(respostas[i] == 1){
			printf("%d  %d\n", peso[i], valor[i]);
		}
	}
	
	for(int k=0;k<MAXN;k++){
		for(int h=0;h<MAXS;h++){
			printf("%d  ", t[k][h]);
		}
		printf("\n");

	}
    //printf("_________________________________________________\n");
	return 0;
}
