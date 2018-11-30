
#include <algorithm>
#include <stdio.h>

using namespace std;

// defino os maiores valores de tamVetor e peso como 1010
#define MAXN 4
#define MAXS 3
#define N 3

// declaro as variáveis que a função utiliza
int tab[MAXN][MAXS];
int valor[MAXN];
int peso[MAXN];
int respostas[MAXN];
int tamVetor = N;

int knapsack(int obj, int aguenta){
	// se já calculamos esse estado da dp, retornamos o resultado salvo
	
	if(tab[obj][aguenta]>=0) return tab[obj][aguenta];
	
	// se não houver mais objetos ou espaço na mochila, retorno 0, pois não posso mais botar nada
	if(obj==tamVetor or !aguenta) return tab[obj][aguenta]=0;
	
	// não colocar avança para o estado em que tentmos o próximo, com o mesmo espaço disponível
	int nao_coloca=knapsack(obj+1, aguenta);
	
	// se for possível colocar o objeto
	if(peso[obj]<=aguenta){
		
		// o melhor atingível é o valor dele mais o melhor entre colocar ou não os próximos
		// que é o resultado do estado da dp em que olhamos o próximo objeto
		// mas agora a mpchila aguenta o que aguentava antes menos o peso que coloquei nela
		int coloca=valor[obj]+knapsack(obj+1, aguenta-peso[obj]);
		
		// e a função deve retornar o melhor entre colocar ou não colocar
		if(coloca == max(coloca, nao_coloca)){
			respostas[obj] = 1;	
		}
		return tab[obj][aguenta]=max(coloca, nao_coloca);
	}
	
	// se a função não retornou ainda, então ela não entrou no if
	// logo não era possível colocar o objeto 

	return tab[obj][aguenta]=nao_coloca; // então retorno o valor de não colocá-lo
}

void parser(){
	int i, j;
    FILE *pont_arq; // cria variável ponteiro para o arquivo
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
    		tab[i][j] = -1;
    	}
    	respostas[i] = 0;
    }
    return;
}

int main(int argc, char const *argv[]){
	parser();
	valor[0] = 2;
	valor[1] = 3;
	valor[2] = 4;
	peso[0] = 2;
	peso[1] = 5;
	peso[2] = 3;

	int resp= 0;
	resp = knapsack(0, 2);
	printf("resposta: %d\n", resp);
	for (int i = 0; i < N; ++i){
		if(respostas[i] == 1){
			//printf("%d  %d\n", peso[i], valor[i]);
		}
	}
	
	for(int i=0;i<MAXN;i++){
    	for(int j=0;j<MAXS;j++){
    		printf("%d  ", tab[i][j]);
    	}
    	printf("\n");

    }
    printf("_________________________________________________\n");
	return 0;
}