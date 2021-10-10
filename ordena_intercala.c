#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 8
#pragma pack(1)

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}


int main(int argc, char**argv){
    FILE *f, *saida;
	Endereco *e;
	long posicao, qtd, subdivisao;
    char nome_arquivo[20];
	f = fopen("cep.dat","r");
	fseek(f,0,SEEK_END);
	posicao = ftell(f);
	qtd = posicao/sizeof(Endereco);
	subdivisao = qtd/N;


// qtd é a quantidade total
// subdivisao são divisões de qtd

    // Ordenando cada subdivisão de cep.dat 
    rewind(f);
    for(int i=0; i<N; i++){
        if(i != N-1){
        
            e = (Endereco*) malloc(subdivisao*sizeof(Endereco));
	    
	        if(fread(e,sizeof(Endereco),subdivisao,f) == subdivisao){
            
		        printf("%d. Lido = OK\n", i);
	        }
	        qsort(e,subdivisao,sizeof(Endereco),compara);
	        printf("%d. Ordenado = OK\n", i);
            sprintf(nome_arquivo, "cep_%d.dat", i);
            saida = fopen(nome_arquivo,"w");
	        fwrite(e,sizeof(Endereco),subdivisao,saida);
	        fclose(saida);
	        printf("Escrito = OK\n");
	        free(e);

        }else{
            e = (Endereco*) malloc(subdivisao*sizeof(Endereco));
            subdivisao = subdivisao + (qtd % N);
	        if(fread(e,sizeof(Endereco),subdivisao,f) == subdivisao){
            
		        printf("%d. Lido = OK\n", i);
	        }
	        qsort(e,subdivisao,sizeof(Endereco),compara);
	        printf("%d. Ordenado = OK\n", i);
            sprintf(nome_arquivo, "cep_%d.dat", i);
            saida = fopen(nome_arquivo,"w");  // saida = fopen("cep_a.dat","w");
	        fwrite(e,sizeof(Endereco),subdivisao,saida);
	        fclose(saida);
	        printf("Escrito = OK\n");
	        free(e); 
        }
    }


    int inicio = 0;
    int final = N - 1;
    // intercalando cada subdivisão até restar uma apenas
    while(inicio != final){
        FILE *a, *b, *saida2;
	    Endereco ea, eb;

        sprintf(nome_arquivo, "cep_%d.dat", inicio);
	    a = fopen(nome_arquivo,"r");

        sprintf(nome_arquivo, "cep_%d.dat", inicio+1);
	    b = fopen(nome_arquivo,"r");

        sprintf(nome_arquivo, "cep_%d.dat", final+1);
	    saida2 = fopen(nome_arquivo,"w");

	    fread(&ea,sizeof(Endereco),1,a);
	    fread(&eb,sizeof(Endereco),1,b);

	    while(!feof(a) && !feof(b)){

		    if(compara(&ea,&eb)<0){

			    fwrite(&ea,sizeof(Endereco),1,saida2);
			    fread(&ea,sizeof(Endereco),1,a);

		    }else{

			fwrite(&eb,sizeof(Endereco),1,saida2);
			fread(&eb,sizeof(Endereco),1,b);
		    }
	    }

	    while(!feof(a)){
		    fwrite(&ea,sizeof(Endereco),1,saida2);
		    fread(&ea,sizeof(Endereco),1,a);		
	    }
	    while(!feof(b)){

		    fwrite(&eb,sizeof(Endereco),1,saida2);
		    fread(&eb,sizeof(Endereco),1,b);		
	    }

	fclose(a);
	fclose(b);
	fclose(saida2);
    
    inicio = inicio + 2;
    final = final + 1;
    }
}