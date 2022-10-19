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

int qtdeArquivoBinarioDados = 0;

struct indice_chave1{
    int chave;
    int endereco;
};
typedef struct indice_chave1 IND1;

struct indice_chave2{
    char chave[4];
    int endereco;
};
typedef struct indice_chave2 IND2;

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

void criarIndiceChave1(){

    FILE *arqText;
    char linha[200];

    arqText = fopen("data.csv","rt");
    if (arqText == NULL){
        printf ("Falha ao abrir o arquivo de dados Texto\n");
        return 0;
    }

    IND1 indice;
    int idRegistro;
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

    FILE *arqBin;

    arqBin = fopen("indice_chave1.bin", "wb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario Texto\n");
        return 0;
    }


    printf("Escrevendo o arquivo binario de indice...\n");

    int cont = 0;

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){
        //printf("%d, %d \n", idRegistro, cont);

        indice.chave = idRegistro;
        indice.endereco = cont;
        cont++;

        fwrite(&indice,sizeof(IND1),1,arqBin);
    }

    printf("Finalizado a escrita dos indices chave 1...\n");

    fclose(arqBin);
    fclose(arqText);

    return 1;
}

void buscarIndiceChave1(int chave){

    FILE *arqBin;
    IND1 indice;

    arqBin = fopen("indice_chave1.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario de Índices \n");
        return 0;
    }

    /*
    while (!feof(arqBin))
    {
        fread(&indice, sizeof(IND1),1, arqBin);
        printf("%d %d \n", indice.chave, indice.endereco);

        //fseek(arqBin,10 * sizeof(indice), SEEK_SET);
    }
    */

    int inicio = 0;
    int meio = 0;
    long fim = (tamanhoArquivo(arqBin)/sizeof(indice))-1;
    int retorno;

    while(inicio <= fim){
        meio = (inicio + fim)/2;
        fseek(arqBin,meio * sizeof(indice), SEEK_SET);

        fread(&indice,sizeof(indice),1,arqBin);

        if (chave > indice.chave){
            printf("%d == %d?\n",chave,indice.chave);
            inicio = meio+1;
        }else{
            if (chave < indice.chave){
                printf("%d == %d?\n",chave,indice.chave);
                fim = meio - 1;
            }else{
                printf("%d == %d?\n",chave,indice.chave);
                retorno = 1;
                break;
            }
        }
    }

    fclose(arqBin);

    if (retorno == 1){
        printf("Indice econtrado\n");
        printf("Valor achado: %d %d\n",indice.chave, indice.endereco);

        DADOS dado;

        arqBin = fopen("dadosBinario.bin", "rb");

        if (arqBin == NULL){
            printf ("Falha ao abrir o arquivo de Binario Texto\n");
            return 0;
        }

        fseek(arqBin,indice.endereco * sizeof(DADOS), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);

        printf("%d: %s\n", dado.idRegistro, dado.accentCity);

        fclose(arqBin);


    }else{
        printf("Indice nao econtrado\n");
    }

}

void criarIndiceChave2(){

    FILE *arqBin, *arqIndice;

    arqBin = fopen("dadosBinario.bin","rb");
    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de dados binario\n");
        return 0;
    }

    qtdeArquivoBinarioDados = tamanhoArquivo(arqBin)/sizeof(DADOS);
    fseek(arqBin, 0 , SEEK_SET);

    arqIndice = fopen("indice_chave2.bin", "wb");

    if (arqIndice == NULL){
        printf ("Falha ao abrir o arquivo de Binario indice\n");
        return 0;
    }

    DADOS dado;
    IND2 indices[qtdeArquivoBinarioDados];
    IND2 indice;

    printf("Escrevendo o arquivo binario de indice...\n");

    int cont = 0;

     while (!feof(arqBin))
    {
        fread(&dado, sizeof(DADOS),1, arqBin);

        strcpy(indices[cont].chave, "abcd");
        printf("%d\n", cont);
        indices[cont].endereco = cont;
        cont++;
    }

    printf("Finalizado a escrita dos indices chave 2...\n");

    fclose(arqBin);
    fclose(arqIndice);

    return 1;
}

void opcoesInterface(){
    printf("--------------------------------------------------\n");
    printf("Digite a opcao desejada: \n");
    printf("0.Sair do programa\n");
    printf("1.Ler exaustivamente o arquivo de dados\n");
    printf("2.Pesquisar id do registro via Pesquisa Binária\n");
    printf("3.Criar indice chave 1\n");
    printf("4.Pesquisar id do registro via Pesquisa Binária na Chave 1\n");
    printf("5. Criar indice chave 2\n");
    printf("--------------------------------------------------\n");
}

void main(){

    int option;
    int chavePesquisa;
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
            scanf("%d",&chavePesquisa);
            pesquisaBinaria(chavePesquisa);
        break;
        case 3:
            criarIndiceChave1();
        break;
        case 4:
            printf("Digite a chave para pesquisar no índice:\n");
            scanf("%d",&chavePesquisa);
            buscarIndiceChave1(chavePesquisa);
        break;
        case 5:
            criarIndiceChave2();
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
