#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<sys/stat.h>

struct dados{
    int idRegistro;
    char idAlfaNumerico[5];
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
    char chave[5];
    int endereco;
};
typedef struct indice_chave2 IND2;

struct indice_chave3{
    char country[4];
    int enderecoInicial;
    struct indice_chave3 *prox;
};
typedef struct indice_chave3 IND3;

// Auxilia saber se já existe uma lista encadeada para o indice
IND3 *inicio;

struct indice_chave4{
    float key;
    int enderecoInicial;
    struct indice_chave4 *left;
    struct indice_chave4 *right;
    int height;
};
typedef struct indice_chave4 IND4;

IND4 *inicio4 = NULL;;

int verificarArquivoExiste (char filename[]){
    struct stat buffer;
    int exist = stat(filename,&buffer);
    if(exist == 0)
        return 1;
    else
        return 0;
}

void printarNaTelaDados(DADOS dado){
    printf("Dados:\n");
    printf("Id: %d\n", dado.idRegistro);
    printf("Id2: %s\n",dado.idAlfaNumerico);
    printf("Country: %s\n",dado.country);
    printf("City: %s\n",dado.city);
    printf("AccentCity: %s\n",dado.accentCity);
    printf("Region: %s\n",dado.region);
    printf("Population: %f\n",dado.population);
    printf("Latitude: %f\n",dado.latitude);
    printf("Longitude: %f\n",dado.longitude);
}

void parseMaiusculo(char string[]){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = toupper(string[i]);
        i++;
    }
}

void parseMinusculo (char string[]){
    int i = 0;
    while(string[i] != '\0'){
        string[i] = tolower(string[i]);
        i++;
    }
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
    char idAlfaNumerico[5];
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

    printf("Escrevendo o arquivo binario de dados...\n");

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,idAlfaNumerico,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){

        printf("%d\n",idRegistro);
        /*
        printf("%s\n",idAlfaNumerico);
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
        strcpy(dado.idAlfaNumerico,idAlfaNumerico);
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
        printf("Arquivo nao encontrado!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END);

    long int res = ftell(fp);

    return res;
}

void pesquisaBinaria(int chave){

    FILE *arqBin;
    int retorno = 0;
    DADOS dado;

    arqBin = fopen("dadosBinario.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo Binario de dados\n");
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
        printarNaTelaDados(dado);
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

    arqText = fopen("data.csv","rt");
    if (arqText == NULL){
        printf ("Falha ao abrir o arquivo de dados Texto\n");
        return 0;
    }

    IND1 indice;
    int idRegistro;
    char idAlfaNumerico[5];
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

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,idAlfaNumerico,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){

        indice.chave = idRegistro;
        indice.endereco = cont;
        cont++;

        fwrite(&indice,sizeof(IND1),1,arqBin);
    }

    printf("Finalizado a escrita dos indices chave 1... \n");

    fclose(arqBin);
    fclose(arqText);

    return 1;
}

void buscarIndiceChave1(int chave){

    FILE *arqBin;
    IND1 indice;

    arqBin = fopen("indice_chave1.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario de indices \n");
        return 0;
    }

    int inicio = 0;
    int meio = 0;
    long fim = (tamanhoArquivo(arqBin)/sizeof(IND1))-1;
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

        DADOS dado;

        arqBin = fopen("dadosBinario.bin", "rb");

        printf("Indice econtrado\n");

        if (arqBin == NULL){
            printf ("Falha ao abrir o arquivo de dados binario\n");
            return 0;
        }

        fseek(arqBin,indice.endereco * sizeof(DADOS), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);

        printarNaTelaDados(dado);

        fclose(arqBin);

    }else{
        printf("Indice nao econtrado\n");
    }

}

void criarIndiceChave2(){

    FILE *arqText;

    arqText = fopen("data.csv","rt");
    if (arqText == NULL){
        printf ("Falha ao abrir o arquivo de dados Texto\n");
        return 0;
    }

    IND2 indice;
    int idRegistro;
    char idAlfaNumerico[5];
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

    FILE *arqBin;

    arqBin = fopen("indice_chave2.bin", "wb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario Texto\n");
        return 0;
    }


    printf("Escrevendo o arquivo binario de indice 2...\n");

    int cont = 0;

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,idAlfaNumerico,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){

        strcpy(indice.chave,idAlfaNumerico);
        indice.endereco = cont;
        cont++;

        fwrite(&indice,sizeof(IND2),1,arqBin);
    }

    printf("Finalizado a escrita dos indices chave 2... \n");

    fclose(arqBin);
    fclose(arqText);

    return 1;
}

void buscarIndiceChave2(char chave[]){
    FILE *arqBin;
    IND2 indice;

    arqBin = fopen("indice_chave2.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario de indices \n");
        return 0;
    }

    int inicio = 0;
    int meio = 0;
    long fim = (tamanhoArquivo(arqBin)/sizeof(IND2))-1;
    int retorno;

    while(inicio <= fim){
        meio = (inicio + fim)/2;
        fseek(arqBin,meio * sizeof(indice), SEEK_SET);

        fread(&indice,sizeof(indice),1,arqBin);

        if (strcmp(chave,indice.chave) > 0){
            printf("%s == %s?\n",chave,indice.chave);
            inicio = meio+1;
        }else{
            if (strcmp(chave,indice.chave) < 0){
                printf("%s == %s?\n",chave,indice.chave);
                fim = meio - 1;
            }else{
                printf("%s == %s?\n",chave,indice.chave);
                retorno = 1;
                break;
            }
        }
    }

    fclose(arqBin);

    if (retorno == 1){

        DADOS dado;

        arqBin = fopen("dadosBinario.bin", "rb");

        printf("Indice econtrado\n");

        if (arqBin == NULL){
            printf ("Falha ao abrir o arquivo de dados binario\n");
            return 0;
        }

        fseek(arqBin,indice.endereco * sizeof(DADOS), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);

        printarNaTelaDados(dado);

        fclose(arqBin);

    }else{
        printf("Indice nao econtrado\n");
    }
}

void inserirIndice(char country[], int endereco)
{
   // printf("C: %d\n",endereco);
    IND3 *novo, *aux=inicio;
    novo = (IND3*) malloc(sizeof(IND3));
    strcpy(novo->country,country);
    novo->enderecoInicial = endereco;
    novo->prox = NULL;

    if (inicio == NULL)
    {
        inicio=novo;
    }
    else
    {
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }

}

void criarIndice3NaMemoria(){

    FILE *arqBin;
    DADOS linha;
    char ultimoCountry[4];
    int cont = 1;

    arqBin = fopen("dadosBinario.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario de dados \n");
        return 0;
    }

    printf("Criando lista de indices da chave 3 na memoria ...\n");

    while(!feof(arqBin)){
        fread(&linha,sizeof(DADOS),1,arqBin);

        if (strcmp(linha.country,ultimoCountry) != 0){
            strcpy(ultimoCountry,linha.country);

            inserirIndice(linha.country,cont);
        }

        cont++;

    }

    fclose(arqBin);

    printf("Finalizando...\n");

}

//Função para teste da lista encadeada
/*
    void percorreListaIndice3(){
    IND3 *aux=inicio;

    while (aux != NULL)
    {
        printf ("UF:%s\n", aux->country);
        printf ("EI:%d\n", aux->enderecoInicial);
        aux=aux->prox;
    }
    printf("\n");
}
*/

void buscarIndiceChave3(char chave[]){

    IND3 *aux=inicio;
    int achou = 0;
    int contadorCidades = 1;

    while (aux != NULL)
    {
        if (strcmp(chave,aux->country) == 0){
            achou = 1;
            break;
        }
        aux=aux->prox;
    }
    if (achou == 1){

        FILE *arqBin;
        DADOS dado;

        arqBin = fopen("dadosBinario.bin", "rb");

        printf("Indice econtrado\n");

        if (arqBin == NULL){
            printf ("Falha ao abrir o arquivo de dados binario\n");
            return 0;
        }

        fseek(arqBin,aux->enderecoInicial * sizeof(DADOS), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);

        while(strcmp(chave,dado.country) == 0){

            printarNaTelaDados(dado);
            printf("-------------------------------\n");
            fread(&dado,sizeof(DADOS),1,arqBin);
            contadorCidades++;
        }

        printf("Qtde cidades: %d\n", contadorCidades);

        fclose(arqBin);
    }

}

int max(int a, int b)
{
    return (a > b)? a : b;
}

int height(IND4 *N)
{
    if (N == NULL)
        return 0;
    return 1+max(height(N->left), height(N->right));
}

IND4* newNode(float key, int cont)
{
    IND4* node = (IND4*) malloc(sizeof(IND4));
    node->key   = key;
    node->enderecoInicial = cont;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 0;  // new node is initially added at leaf
    return(node);
}

IND4 *rightRotate(IND4 *y)
{
    IND4 *x = y->left;
    IND4 *T2 = x->right;

    x->right = y;
    y->left = T2;

    // Atualizar alturas
    y->height = height(y);
    x->height = height(x);

    return x;
}

// Rotacao Esquerda
IND4 *leftRotate(IND4 *x)
{
    IND4 *y = x->right;
    IND4 *T2 = y->left;

    y->left = x;
    x->right = T2;

    // Atualizar alturas
    x->height = height(x);
    y->height = height(y);

    return y;
}

// Calcular valor para balancear
int getBalance(IND4 *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Inserir
IND4* criarNodoIndice4(IND4* node, float key, int cont)
{
    /* 1.  Perform the normal BST insertion */
    if (node == NULL){
        return(newNode(key, cont));
    }


    if (key < node->key)
        node->left  = criarNodoIndice4(node->left, key, cont);
    else if (key > node->key)
        node->right = criarNodoIndice4(node->right, key, cont);
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = height(node);

    /* 3. Get the balance factor of this ancestor
          node to check whether this node became
          unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }



    /* return the (unchanged) node pointer */
    return node;
}

   /*
void preOrder(IND4 *root)
{
    if(root != NULL)
    {
        printf("%f\n", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}
*/

void criarIndice4NaMemoria(){

    FILE *arqBin;
    DADOS linha;
    int cont = 0;

    arqBin = fopen("dadosBinario.bin", "rb");

    if (arqBin == NULL){
        printf ("Falha ao abrir o arquivo de Binario de dados \n");
        return 0;
    }

    printf("Criando lista de indices da chave 4 na memoria ...\n");

    while(!feof(arqBin) && cont < 10000){
        fread(&linha,sizeof(DADOS),1,arqBin);
        inicio4 = criarNodoIndice4(inicio4, linha.population, cont);

        cont++;
    }


    fclose(arqBin);

    //preOrder(inicio4);

    printf("Finalizando...\n");

}

void buscarIndiceChave4(float chave){

    IND4 *aux=inicio4;
    int achou = 0;

    while (aux != NULL)
    {
        if (chave == aux->key){
            achou = 1;
            break;
        }else if(chave > aux->key){
            aux = aux->right;
        }else {
             aux = aux->left;
        }
    }

    if (achou == 1){

        FILE *arqBin;
        DADOS dado;

        arqBin = fopen("dadosBinario.bin", "rb");

        printf("Indice econtrado\n");

        if (arqBin == NULL){
            printf ("Falha ao abrir o arquivo de dados binario\n");
            return 0;
        }

        fseek(arqBin,aux->enderecoInicial * sizeof(DADOS), SEEK_SET);

        fread(&dado,sizeof(DADOS),1,arqBin);
        printarNaTelaDados(dado);

        fclose(arqBin);
    }

}



/*
    Header do arquivo de Dados: Id,IdAlfaNumerico,Country,City,AccentCity,Region,Population,Latitude,Longitude
    *Indices em arquivos
    {
        Chave 1: id
        Chave 2: idAlfaNumerico
    }
    *Indices em memoria
    {
        Chave 3: country
        Chave 4: population
    }
*/

void cidadesIniciamLetra (char letra[1]) {

    FILE *arqText;
    char linha[200];

    int idRegistro;
    char idAlfaNumerico[5];
    char country[4];
    char city[25];
    char accentCity[25];
    char region[4];
    float population;
    float latitude;
    float longitude;

    arqText = fopen("data.csv","rt");
    if (arqText == NULL){
        printf ("Falha ao abrir o arquivo de dados Texto\n");
        return 0;
    }

    while (fscanf(arqText,"%d,%[^,],%[^,],%[^,],%[^,],%[^,],%f,%f,%f\n",&idRegistro,idAlfaNumerico,country,city,accentCity,region,&population,&latitude,&longitude) != EOF){
        if(letra[0] == city[0]){
            printf("%s\n", city);
        }
    }

    fclose(arqText);

}

void opcoesInterface(){
    printf("--------------------------------------------------\n");
    printf("Digite a opcao desejada: \n");
    printf("0.Sair do programa\n");
    printf("1.Ler exaustivamente o arquivo de dados\n");
    printf("2.Criar arquivo binario de dados para fazer as consultas e criacoes\n");
    printf("3.Pesquisar id do registro via Pesquisa Binaria\n");
    printf("4.Criar arquivo de indice(chave 1)\n");
    printf("5.Pesquisar id do registro via Pesquisa Binaria na Chave 1\n");
    printf("6.Criar arquivo de indice(chave 2)\n");
    printf("7.Pesquisar id do registro via Pesquisa Binaria na Chave 2\n");
    printf("8.Criar indice em memoria (chave 3)\n");
    printf("9.Pesquisar indice (chave 3) na memoria\n");
    printf("10.Criar indice em memoria (chave 4)\n");
    printf("11.Pesquisar indice (chave 4) na memoria\n");
    printf("12.Cidades que começam com X letra\n");
    printf("--------------------------------------------------\n");
}


void main(){

    int option;
    int chavePesquisa;
    char chavePesquisa2[5];
    float chavePesquisa3;
    DADOS retornoPesquisaBinaria;
    char letra[1];

    inicio = NULL;

    opcoesInterface();
    scanf("%d",&option);
    while (option != 0){

    switch (option){
        case 1:
            lerExaustivamenteArquivoDados();
            printf("finalizando ....\n ");
        break;
        case 2:
        if (verificarArquivoExiste("dadosBinario.bin") == 0){
            CriarArquivoBinarioDados();
        }else{
            printf("Arquivo binario de dados ja criado\n");
        }
        break;
        case 3:
        if (verificarArquivoExiste("dadosBinario.bin") == 0){
            printf("Arquivo de dados binario nao criado, para continuar crio-o\n");
        }else{
            printf("Digite o id para buscar: \n");
            scanf("%d",&chavePesquisa);
            pesquisaBinaria(chavePesquisa);
        }
        break;
        case 4:
        if (verificarArquivoExiste("indice_chave1.bin") == 0){
            criarIndiceChave1();
        }else{
            printf("Arquivo de indice(chave 1) já criado\n");
        }
        break;
        case 5:
        if (verificarArquivoExiste("indice_chave1.bin") == 0){
            printf("Arquivo de indice 1 nao encontrado. Para buscar, crie-o\n");
         }else{
            printf("Digite a chave para pesquisar no indice:\n");
            scanf("%d",&chavePesquisa);
            buscarIndiceChave1(chavePesquisa);
         }
        break;
        case 6:
        if (verificarArquivoExiste("indice_chave2.bin") == 0){
            criarIndiceChave2();
        }else{
            printf("Arquivo de indice(chave 2) já criado\n");
        }
        break;
        case 7:
        if (verificarArquivoExiste("indice_chave2.bin") == 0){
            printf("Arquivo de indice 2 nao encontrado. Para buscar, crie-o\n");
         }else{
            printf("Digite a chave para pesquisar no indice 2 (formato AAAA):\n");
            scanf("%s",&chavePesquisa2);
            parseMaiusculo(chavePesquisa2);
            buscarIndiceChave2(chavePesquisa2);
         }
        break;
        case 8:
        if (inicio == NULL){
            criarIndice3NaMemoria();
        }else{
            printf("Indice (chave 3) ja criado na memoria\n");
        }
        break;
        case 9:
        if (inicio == NULL){
            printf("Lista de indices para a chave 3 não encontrada. Para buscar, crie-a\n");
        }else{
            printf("Digite o pais para buscar os registros (formato aa):\n");
            scanf("%s",&chavePesquisa2);
            parseMinusculo(chavePesquisa2);
            buscarIndiceChave3(chavePesquisa2);
        }
        break;
        case 10:
        if (inicio4 == NULL){
            criarIndice4NaMemoria();
        }else{
            printf("Indice (chave 4) ja criado na memoria\n");
        }
        break;
        case 11:
        if (inicio4 == NULL){
            printf("Lista de indices para a chave 4 não encontrada. Para buscar, crie-a\n");
        }else{
            printf("Digite o numero:\n");
            scanf("%f",&chavePesquisa3);

            buscarIndiceChave4(chavePesquisa3);
        }
        break;
        case 12:
            printf ("\nDigite a letra: \n");
            scanf ("%s",letra);
            cidadesIniciamLetra(letra);
        break;
        default:
            printf("Opcao nao encontrada, tente novamente ...\n");
        break;
    }

    opcoesInterface();
    scanf("%d",&option);

    }
    printf("Encerrado o programa ...");
}
