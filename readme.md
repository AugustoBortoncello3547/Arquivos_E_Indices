# Trabalho de Algoritmos e Estrutura de Dados II: Organizações de arquivos e Índices

#### Integrantes: Augusto Bortoncello e Gabriel Moscone 

##### Link DataSet: https://www.kaggle.com/datasets/max-mind/world-cities-database?select=worldcitiespop.csv
---
Foi encontrado um Dataset com uma lista de todas cidades do mundo, podendo utilizar ele como base para o trabalho, contendo informações sobre o país e população, por exemplo. Como perguntas, inicialmente pensamos nas seguintes: 
*  Quantas cidades tem em um país? 
*  Quantas cidades tem mais que X habitantes? 
* Lista das cidades que começam com X letra? 
* Lista das cidades de um país? 

Vale ressaltar que, por se tratar de um Dataset com milhões de registros, vamos selecionar cerca de cem mil cidades para realizar as consultas.
Durante o desenvolvimento do trabalho, pudemos perceber que a maioria das perguntas poderia ser respondida com tranquilidade, exceto a segunda, pois seria muito custoso para o processsador. Então decidimos trocar por:
*  Quantas cidades tem mais que X habitantes? -> Quantos países o dataset possui?

Para responder a maoria das perguntas utilizamos o índice de chave 3, criado na memória.

#### Segunda parte do trabalho: NoSQL

Para a segunda parte do trabalho, escolhemos o SGBD NoSQL MongoDB e implementamos o método de criptografia chamado cifra de XOR para criptografar o campo "idAlfaNumerico", assim geramos uma nova planilha com uma nova coluna o qual possuia o valor criptografado para cada linha do arquivo de dados original.

O arquivo chamado NoSQL.docx dentro da pasta Descricao possui todos os testes descritos realizados no MongoDB, juntamente com uma explicação do método de criptografia escolhido.
