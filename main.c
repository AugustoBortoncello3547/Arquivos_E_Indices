#include <stdio.h>
#include<sys/stat.h>

// Header do arquivo de Dados: Id,Country,City,AccentCity,Region,Population,Latitude,Longitude

struct dados{
    int idRegistro;
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

};
typedef struct dados DADOS;

int verificarArquivoExiste (char filename[]){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else
        return 0;
}

void CriarArquivoBinarioDados(){

    FILE *arqText;
    char linha[200];

    arqText = fopen("data.csv","rt");
    if (arqText == NULL){
        printf ("Falha ao abrir o arquivo de dados Texto\n");
        return 0;
    }

    FILE *arqBin;

    arqBin = fopen("dadosBinario.bin", "wb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario Texto\n");
        return 0;
    }

    DADOS dado;
    int idRegistro;
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

    printf("Escrevendo o arquivo binario de dados...\n");

    int cont = 0;

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){

        printf("%d\n",idRegistro);
        /*
        printf("%s\n",country);
        printf("%s\n",city);
        printf("%s\n",accentCity);
        printf("%s\n",region);
        printf("%f\n",population);
        printf("%f\n",latitude);
        printf("%f\n",longitude);
        printf("-----------\n");
        */

        dado.idRegistro = idRegistro;
        strcpy(dado.country,country);
        strcpy(dado.city,city);
        strcpy(dado.accentCity,accentCity);
        strcpy(dado.region,region);
        dado.population = population;
        dado.latitude = latitude;
        dado.longitude = longitude;

        fwrite(&dado,sizeof(DADOS),1,arqBin);
    }

    printf("Finalizado a escrita dos dados...\n");

    fclose(arqBin);
    fclose(arqText);

    return 1;
}

long tamanhoArquivo(FILE *fp){

    if (fp == NULL) {
        printf("Arquivo não encontrado!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);

    long int res = ftell(fp);

    return res;
}

void pesquisaBinaria(int chave){

    int ret = verificarArquivoExiste("dadosBinario.bin");

    printf("%d\n",ret);
    if (ret == 0){
        CriarArquivoBinarioDados();
    }

    FILE *arqBin;
    int retorno = 0;
    DADOS dado;

    arqBin = fopen("dadosBinario.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario Texto\n");
        return 0;
    }
    int inicio = 0;
    int meio = 0;
    long fim = (tamanhoArquivo(arqBin)/sizeof(DADOS))-1;

    while(inicio <= fim){
        meio = (inicio + fim)/2;
        fseek(arqBin,meio * sizeof(dado), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);

        if (chave > dado.idRegistro){
            printf("%d == %d?\n",chave,dado.idRegistro);
            inicio = meio+1;
        }else{
            if (chave < dado.idRegistro){
                printf("%d == %d?\n",chave,dado.idRegistro);
                fim = meio - 1;
            }else{
                printf("%d == %d?\n",chave,dado.idRegistro);
                retorno = 1;
                break;
            }
        }
    }

    fclose(arqBin);

    if (retorno == 1){
        printf("valor econtrado\n");
    }else{
        printf("valor nao econtrado\n");
    }

}


void lerExaustivamenteArquivoDados(){

    FILE *arq;
    char linha[150];
    char *result;

    arq = fopen("data.csv","rt");
    if (arq == NULL){
        printf ("Falha ao abrir o arquivo de dados");
        return;
    }

    while (!feof(arq)){
        result = fgets(linha, 150, arq);
        if (result){
            printf("%s",linha);
        }
    }

}

void opcoesInterface(){
    printf("--------------------------------------------------\n");
    printf("Digite a opcao desejada: \n");
    printf("0.Sair do programa\n");
    printf("1.Ler exaustivamente o arquivo de dados\n");
    printf("2.Pesquisar id do registro via Pesquisa Binária\n");
    printf("--------------------------------------------------\n");
}

void main(){

    int option;
    int idPesquisaBinaria;
    DADOS retornoPesquisaBinaria;

    opcoesInterface();
    scanf("%d",&option);
    while (option != 0){

    switch (option){
        case 1:
            lerExaustivamenteArquivoDados();
            printf("finalizando ....\n ");
        break;
        //--------
        case 2:
            printf("Digite o id para buscar: \n");
            scanf("%d",&idPesquisaBinaria);
            pesquisaBinaria(idPesquisaBinaria);
        break;
        default:
            printf("Opção não encontrada, tente novamente ...\n");
        break;
    }

    opcoesInterface();
    scanf("%d",&option);

    }
    printf("Encerrado o programa ...");
}
