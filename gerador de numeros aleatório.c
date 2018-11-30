

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	int i;
	srand( (unsigned)time(NULL) );
    FILE *pont_arq; // cria variável ponteiro para o arquivo
    pont_arq = fopen("dados.txt", "w");
    if(pont_arq == NULL){
    	printf("Erro na abertura do arquivo!");
    	return 1;
    }
    for(i=1 ; i <= 10000 ; i++)
    	fprintf(pont_arq, "%d, %d\n", rand()%1000, rand()%1000);
    fclose(pont_arq);
    return(0);
}
