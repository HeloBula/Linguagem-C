#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

//Lista pra guardar os filmes na RAM
typedef struct dados{
    char codigo[6];
    char tituloPT[45];
    char tituloOriginal[45];
    char diretor[45];
    char anoLancado[5];
    char paisOrigem[45];
    char nota[2];
}Filme;

//Lista pra guardar os índices na RAM
typedef struct campos{
    char cod[6];
    int RRN;
    struct campos *prox;
}IndicePrimario;

//Lista pra guardar os índices na RAM
typedef struct registro{
    char cod[6];
    char tituloBusca[46];//Titulo que será gravado, e é ele que será buscado
    struct registro *prox;
}IndiceSecundario;

//Flag dos arquivos
int flagP, flagS;

//Crio o indice Primário que será inserido
IndicePrimario *criarIndiceP(int RRN, char *cod) {
    IndicePrimario *indAux = malloc(sizeof(IndicePrimario));
    strcpy(indAux->cod, cod);
    indAux->RRN = RRN;
    indAux->prox = NULL;

    return indAux;

}

//Crio o indice Secundário que será inserido
IndiceSecundario *criarIndiceS(char *cod, char *titulo) {
    IndiceSecundario *indAux = malloc(sizeof(IndiceSecundario));
    strcpy(indAux->cod, cod);
    strcpy(indAux->tituloBusca,titulo);
    indAux->prox = NULL;

    return indAux;

}

//Insiro ordenado na lista de indice primário na RAM
IndicePrimario *inserirLista(IndicePrimario *h, IndicePrimario *add)
{
    
  IndicePrimario *anterior, *aux;
  if (!h) // Cabeça nula
    return add;
    aux = anterior = h;
  while (aux->prox && strcmp(add->cod, aux->cod) > 0)
  {
    anterior = aux;
    aux = aux->prox;
  }
  if (aux == h) // Se for igual a cabeça
  {
    if (strcmp(add->cod, h->cod) < 0)
    {
      add->prox = h;
      return add;
    }
    else if (strcmp(add->cod, h->cod) > 0)
    {
      add->prox = h->prox;
      h->prox = add;
      return h;
    }
    else
    { // Se forem o mesmo
      return h;
    }
  }
  // Quando for diferente da cabeça
  if (strcmp(add->cod, aux->cod) < 0)
  {
    anterior->prox = add;
    add->prox = aux;
    return h;
  }
  else if (strcmp(add->cod, aux->cod) > 0)
  {
    add->prox = aux->prox;
    aux->prox = add;
    return h;
  }
  else
  {
    return h;
  }
}

//Insiro ordenado na lista de indice secundário na RAM
IndiceSecundario *inserirListaS(IndiceSecundario *h, IndiceSecundario *add)
{
    
  IndiceSecundario *anterior, *aux;
  if (!h) // Cabeça nula
    return add;
    aux = anterior = h;
  while (aux->prox && strcmp(add->tituloBusca, aux->tituloBusca) > 0)
  {
    anterior = aux;
    aux = aux->prox;
  }
  if (aux == h) // Se for igual a cabeça
  {
    if (strcmp(add->tituloBusca, h->tituloBusca) < 0)
    {
      add->prox = h;
      return add;
    }
    else if (strcmp(add->tituloBusca, h->tituloBusca) >= 0)
    {
      add->prox = h->prox;
      h->prox = add;
      return h;
    }
  }
  // Quando for diferente da cabeça
  if (strcmp(add->tituloBusca, aux->tituloBusca) < 0)
  {
    anterior->prox = add;
    add->prox = aux;
    return h;
  }
  else if (strcmp(add->tituloBusca, aux->tituloBusca) >= 0)
  {
    add->prox = aux->prox;
    aux->prox = add;
    return h;
  }
}

//Função para escrever o arquivo 'movies.dat'
void escreverArquivo(FILE *arquivo,Filme filmeAdd,IndicePrimario *armazena){
    char formato[193];//Tamanho máximo desejado
    int numCaracter = 192;
    int i;
    arquivo = fopen("movies.dat","a+");
     
    //Formata o código porque ele tem o tamanho fixo e inclue o separador @ no final
    snprintf(formato, sizeof(formato), "%s@", filmeAdd.codigo);//essa função - snprintf - permite formatar a entrada de texto
    //Vou colocando tudo no formato a ser escrito, separando por @. Para isso vou concatenando cada campo da struct
    strcat(formato,filmeAdd.tituloPT);
    strcat(formato,"@");//Coloco o separador
    if(strcmp(filmeAdd.tituloOriginal,filmeAdd.tituloPT)== 0){//Se o título em português é igual ao original, eu salvo como idem
        strcat(formato,"Idem");
    }else//Se não eu só adiciono o titulo
    {
        strcat(formato,filmeAdd.tituloOriginal);
    }   
    strcat(formato,"@");//Coloco o separador
    strcat(formato,filmeAdd.diretor);
    strcat(formato,"@");//Coloco o separador

    strcat(formato,filmeAdd.anoLancado);
    strcat(formato,"@");//Coloco o separador
    strcat(formato,filmeAdd.paisOrigem);
    strcat(formato,"@");//Coloco o separador
    strcat(formato,filmeAdd.nota);
    strcat(formato,"@");//Finalizo com o código

    //Removo a quantidade de espaço usado e preencho com # o restante
    numCaracter = numCaracter - strlen(formato);
    for(i=0;i<numCaracter;i++){
        strcat(formato,"#");
    }
    
    //Escrevo o filme formatado no arquivo
    fprintf(arquivo,"%s",formato);

    fclose(arquivo);//Fecho o arquivo
    printf("Filme adicionado com sucesso!\n");
}

//Calcular o RRN do último filme adicionado para gravar na lista
int acharRRN(FILE *arqDado){
    arqDado = fopen("movies.dat","r");
    fseek(arqDado,0,SEEK_END);//Coloco no fim do arquivo
    int tam;
    tam = ftell(arqDado);//Conto a qtd de bytes que tem
    fclose(arqDado);

    return (tam/192);//Divido a quantidade de bytes pelo tamanho fixo da struct e a retorno

}

//Escreve o arquivo de indice primário ao final do programa
void GravaPrimaria(FILE *arquivo,IndicePrimario *ind){
    IndicePrimario *aux;
    aux = ind;
 int flagGravada;
    fseek(arquivo, 0, SEEK_SET); //Move o ponteiro para o início do arquivo
    if (fscanf(arquivo, "%d", &flagGravada) != 1&&fscanf(arquivo, "%d", &flagGravada) != -1) {//Gravo o valor de alteração da flag nos arquivos de indice
        fprintf(arquivo, "%d", 1);
    }

    // Volta ao final do arquivo
    fseek(arquivo, 0, SEEK_END);

    while(aux != NULL){//Roda a lista e vai gravando no arquivo
      char codAux[6];
      strcpy(codAux,aux->cod);
      if(codAux != "-1"){//Se tiver o indice e ele não foi removido
        fprintf(arquivo,"%s@%d#",aux->cod,aux->RRN);
        aux = aux->prox;
      }
    }
}

//Escreve o arquivo de indice secundário
void GravaSecundaria(FILE *arquivo,IndiceSecundario *ind){
    IndiceSecundario *aux;
    aux = ind;

    int flagGravada;
    fseek(arquivo, 0, SEEK_SET); // Move o ponteiro para o início do arquivo
    if (fscanf(arquivo, "%d", &flagGravada) != 1) {//Gravo o valor de alteração da flag nos arquivos de indice
        fprintf(arquivo, "%d", 1);
    }

    // Volta ao final do arquivo
    fseek(arquivo, 0, SEEK_END);

    while(aux != NULL){
        fprintf(arquivo,"%s@%s#",aux->tituloBusca, aux->cod);
        aux = aux->prox;
    }
}

//Função para imprimir os dados dos filmes
void leFilmes(FILE *arq){
    char linha[193]; //Tamanho máximo da struct
    char *filme;
    int count = 0, numeroFilme = 0, stt = 0;

    arq = fopen("movies.dat", "r");
    
    while (fread(linha, 192, 1, arq) > 0) {//Leio cada struct
      if (linha[0] == '*') { // Verifica se o registro foi marcado como removido, ai eu pulo
            continue;
        }else{ //Se ele ainda estiver, eu imprimo os dados
          stt = 1;
          numeroFilme++;
          printf("Filme %d:\n", numeroFilme);
          int count = 0;
          filme = strtok(linha,"@");//Cada repetição vai ser lido um campo da struct
        while (filme != NULL && count < 7) {
            switch (count) {
                case 0:
                    printf("Codigo: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 1:
                    printf("Titulo em Portugues: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 2:
                    printf("Titulo Original: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 3:
                    printf("Diretor(a): %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 4:
                    printf("Ano de lancamento: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 5:
                    printf("Pais: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 6:
                    printf("Nota: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                default:
                    break;
                
            }  
        
        printf("\n");
       }
     }
    } 
    fclose(arq);

    if(stt == 0)//Se não tiver filmes cadastrados
    printf("Nao ha filmes cadastrados\n");
 }

//Essa função vai ser usada caso o arquivo de indice primário esteja atualizado, vou pegar e colocar em RAM
IndicePrimario *carregaFilmes(FILE *dado, IndicePrimario *ind){
    char linha[193];
    dado = fopen("movies.dat","r");
    IndicePrimario *aux;
    int count = 0;

     while (fread(linha, 192, 1, dado) == 1) {
        char *filme = strtok(linha, "@");
        aux = malloc(sizeof(IndicePrimario));
        strcpy(aux->cod,filme);
        aux->RRN = count;
        aux->prox = NULL;
        ind = inserirLista(ind,aux);
        count++;
     }
     fclose(dado);
     return ind;
}

//Essa função vai ser usada caso o arquivo de indice secundário esteja atualizado, vou pegar e colocar em RAM
IndiceSecundario *carregaFilmeSecundaria(FILE *dado, IndiceSecundario *ind){
    char linha[193];
    dado = fopen("movies.dat","r");
    IndiceSecundario *aux;

     while (fread(linha, 192, 1, dado) == 1) {
        char *filme = strtok(linha, "@");
        char *titulo = strtok(NULL, "@");//Uma vez que o titulo em portugues será o segundo campo
        aux = malloc(sizeof(IndiceSecundario));
        strcpy(aux->cod,filme);
        strcpy(aux->tituloBusca,titulo);
        aux->prox = NULL;
        ind = inserirListaS(ind,aux);
     }
     fclose(dado);
     return ind;
}

//Atualizar a flag dos arquivos durante a execução do programa
void atualizaFlag(FILE *arq,char *nome_arq){
  char flag;

  arq = fopen(nome_arq,"r+");
  flag = fgetc(arq);

//Volto pro inicio
  fseek(arq,0,SEEK_SET);
//Reinsere como nova flag o zero - foi atualizado
  fputc('0',arq);

  fclose(arq);

}

//Função lê o arquivo de indice primario e coloca na RAM
IndicePrimario *lePrimario(FILE *arq,IndicePrimario *aux){
   char leitura[15], *separador;
   int flagInutilizada;
   IndicePrimario *ajuda;

   arq  = fopen("iprimary.idx","r");
   fscanf(arq, "%d", &flagInutilizada);

  while(fscanf(arq," %[^#]s", leitura)>0){
    //Vai pegar a # para não dar erro
    fgetc(arq);
    separador = strtok(leitura,"@");//Pega os campos separados
    ajuda = malloc(sizeof(IndicePrimario));
    strcpy(ajuda->cod,separador);
    separador = strtok(NULL,"@");
    ajuda->RRN = atoi(separador);
    ajuda->prox = NULL;
    aux = inserirLista(aux,ajuda);
  }
  fclose(arq);
  return aux;
}

//Função lê o arquivo de indice secundário e coloca na RAM
IndiceSecundario *leSecundario(FILE *arq,IndiceSecundario *aux){
   char leitura[200], *separador;
   int flagInutilizada;
   IndiceSecundario *ajuda;

   arq  = fopen("ititle.idx","r");
   fscanf(arq, "%d", &flagInutilizada);

  while(fscanf(arq," %[^#]s",leitura)>0){
    //Vai pegar a # para não dar erro
    fgetc(arq);
    separador = strtok(leitura,"@");
    ajuda = malloc(sizeof(IndiceSecundario));
    strcpy(ajuda->tituloBusca,separador);
    separador = strtok(NULL,"@");
    strcpy(ajuda->cod,separador);
    ajuda->prox = NULL;
    
    aux = inserirListaS(aux,ajuda);
  }
  fclose(arq);
  return aux;
}

//Função realiza a busca usando o código do filme, printando as informações quando achar ou informando se não achar
void buscaCodigo(char *codBusca, IndicePrimario *indice,FILE *dados){
  IndicePrimario *aux;
  int RRN,encontrou = 0;
  char linha[193];
 
  aux = indice;
  dados = fopen("movies.dat","r");//Abro o arquivo para leitura
//Enquanto houver indice
  while(aux != NULL){
    if(strcmp(aux->cod,codBusca) == 0){
    //Se for o filme desejado
      RRN = aux->RRN;//Pego o RRN
      
      //Posiciono o ponteiro na posição da struct que deve começar a ser inserido
      fseek(dados,RRN*192,SEEK_CUR);
      fread(linha, 192, 1, dados);
      
       char *filme = strtok(linha, "@");
       int count = 0;

        while (filme != NULL && count < 7) {
            switch (count) {
                case 0:
                    printf("Codigo: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 1:
                    printf("Titulo em Portugues: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 2:
                    printf("Titulo Original: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 3:
                    printf("Diretor(a): %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 4:
                    printf("Ano de lancamento: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 5:
                    printf("Pais: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                case 6:
                    printf("Nota: %s\n", filme);
                    filme = strtok(NULL, "@");
                    count++;
                    break;
                default:
                    break;
                
            }  
        
        fclose(dados);
        printf("\n");
        encontrou = 1;

        }
        
      }
      aux = aux->prox;
      }
      if(encontrou != 1){
        printf("Filme nao encontrado!\n");
  }
}

//Função que busca o filme utilizando o titulo em português, se achar eu pego o código e chamo a outra função
void buscaTitulo(char *titulo,IndiceSecundario *sec,IndicePrimario *prim,FILE *dados){
  IndiceSecundario *aux;
  char cod[6];
  int contador = 0;//Contador para ver se achou ao menos algum filme com o titulo


  aux = sec;

  while (aux != NULL) {
        if (strcmp(aux->tituloBusca, titulo) == 0) {
            // Se encontrar o titulo desejado, eu pego o código associado
            printf("Filme %d\n", contador+1);
            buscaCodigo(aux->cod, prim, dados);//Chamo a função para buscar pelo código
            contador++;
        }
        aux = aux->prox;
    }

    if (contador == 0) { // Não tiver filme com esse titulo
        printf("Filme nao encontrado!\n");
    }
}

//Busco o filme e altero a nota
void alteraNota(char *codBusca, IndicePrimario *indice,FILE *dados){
  IndicePrimario *aux;
  int RRN;
  char linha[193];
  char fraseAux[193];
  char notaNova[2];
 
  aux = indice;

//Enquanto houver indice
  while(aux != NULL && strcmp(aux->cod, codBusca) != 0){
    aux = aux->prox;
  }

  if( aux == NULL)//Se não achar o filme para trocar a nota
  {
    printf("Filme nao encontrado!\n");
  }
  else
  {
      RRN = aux->RRN;
      dados = fopen("movies.dat","r+");
      //Posiciono o ponteiro na posição do filme que vou alterar a nota
      fseek(dados,RRN*192,SEEK_SET);
      fscanf(dados," %[^#]s",fraseAux);
      fseek(dados,-2,SEEK_CUR);//Tiro 2 posições de onde se encontra, voltando para a posição de alterar a nota
      printf("Insira a nova nota:");
      scanf(" %s",&notaNova);
      fprintf(dados,"%s",notaNova);//Printa a nova nota no arquivo
     
     fclose(dados);
  }      
}   

//Função para buscar o código do filme que será removido
int buscaRemove(char *codBusca, IndicePrimario *indice){
  IndicePrimario *aux;
  int RRN,encontrou = 0;
 
  aux = indice;
 
//Enquanto houver indice
  while(aux != NULL){
    if(strcmp(aux->cod,codBusca) == 0){
    //Se for o filme desejado, eu retorno o RRN
      RRN = aux->RRN;
      return RRN;
    }
     aux = aux->prox;  
  }
   return -1;//Caso não ache o filme a ser removido
}

//Função para marcar como removido no arquivo de dados
void removerFilme(char *codRemove,IndicePrimario *prim,FILE *dados){
  IndicePrimario *aux;
  int RRNremove;
  char linha[193];
    dados = fopen("movies.dat","r+");
    RRNremove = buscaRemove(codRemove,prim);//Chamo para achar o RRN

    if(RRNremove == -1){//Se eu não achar, eu informo ao usuário
      printf("Filme nao encontrado!\n");
    }else{//Posiciono no inicio do filme que quero remover
      dados = fopen("movies.dat","r+");
      fseek(dados,RRNremove*192,SEEK_SET);
      fgetc(dados);//Leio as duas posições que quero substituir
      fgetc(dados);
      fseek(dados,-2,SEEK_CUR);//Volto e escrevo o *|
      fprintf(dados,"*|");
      fclose(dados);//Fecho o arquivo
      printf("Filme removido com sucesso!");
    }
}

//Removo da lista e retorno a cabeça
IndicePrimario *removeFilme(IndicePrimario *h,char *cod){
  IndicePrimario *aux;
  aux = h;
  IndicePrimario *anterior;
 
    // Se o filme estiver na cabeça da lista
    if (aux != NULL && strcmp(aux->cod,cod)==0) {
        h = aux->prox; //Troco a cabeça
        free(aux); //Excluo a antiga
        return h;
    }
 
    while (aux != NULL && strcmp(aux->cod,cod)!=0) { //Elemento no meio da lista, roda até achar um igual
        anterior = aux;
        aux = aux->prox;
    }
 
    //Se não achar o filme
    if (aux == NULL){
        return aux;
    }
    // Retira o do meio e linka os outros dois
    anterior->prox = aux->prox;
 
    free(aux); //Libero a memória alocada para aux
    return h;
}

IndiceSecundario *removeFilmeSecundaria(IndiceSecundario *h,char *cod){
  IndiceSecundario *aux;
  aux = h;
  IndiceSecundario *anterior;
 
    // Se o filme estiver na cabeça da lista
    if (aux != NULL && strcmp(aux->cod,cod)==0) {
        h = aux->prox; //Troco a cabeça
        free(aux); //Excluo a antiga
        return h;
    }
 
    while (aux != NULL && strcmp(aux->cod,cod)!=0) { //Elemento no meio da lista, roda até achar um igual
        anterior = aux;
        aux = aux->prox;
    }
 
    //Se não achar o filme
    if (aux == NULL){
        return aux;
    }
    // Retira o do meio e linka os outros dois
    anterior->prox = aux->prox;
 
    free(aux); //Libero a memória alocada para aux
    return h;
}

int main(){
 Filme filme;
 FILE *fi,*fp,*fs;//fp para arquivo de dados, fi para indice primário, fs para indice secundário
 int op, auxRRN;
 IndicePrimario *indiceP = NULL, *aux;
 IndiceSecundario *indiceS = NULL,*auxS;
 char filmeBuscado[200], codAux[6];

if((fp = fopen("movies.dat","r")) == NULL){
  //Forneço só a opção de adicionar ou sair na primeria vez que executar
   printf("Bem-vindo ao Gestor de Filmes!\nEssa e a primeira vez que voce roda o programa, voce pode:\n");
  do{
  printf("Menu:\n");
  printf("1 - Inserir novo filme\n2 - Sair\n");
  scanf("%d",&op);
  if(op == 1){
   
    printf("Titulo do Filme em Portugues: ");
            scanf(" %44[^\n]",filme.tituloPT);//Garanto que pegue strings com espaço e sem atrapalhar o buffer da próxima leitura
            printf("Titulo do Filme Original: ");
            scanf(" %44[^\n]",filme.tituloOriginal);
            printf("Insira o nome do Diretor(formato SOBRENOME,nome): ");
            scanf(" %44[^\n]",filme.diretor);//Parando antes do /0
             printf("Ano de lancamento: ");
            scanf(" %4[^\n]", filme.anoLancado);
            while (getchar() != '\n');

            printf("Insira o pais onde foi produzido: ");
            scanf(" %44[^\n]", filme.paisOrigem);
            while (getchar() != '\n');

            printf("Nota (0 a 9): ");
            scanf("%s",filme.nota);
            
            // Concatenando o código do filme
            strncpy(codAux, filme.diretor, 3); //A função 'strncpy' realiza a cópia de alguns caracteres especificados da string -  no caso as 3 primeiras letras do diretor
            strncpy(codAux + 3, filme.anoLancado + 2, 2);
            codAux[5] = '\0'; //Terminador do código

            strcpy(filme.codigo,codAux);
        
            auxRRN = acharRRN(fp);
            escreverArquivo(fp,filme,indiceP);
            
            aux = criarIndiceP(auxRRN,filme.codigo);
            indiceP = inserirLista(indiceP,aux);

            auxS = criarIndiceS(filme.codigo,filme.tituloPT);
            indiceS = inserirListaS(indiceS,auxS);

   }else if(op == 2){
      fi = fopen("iprimary.idx","w+");
       GravaPrimaria(fi,indiceP);
       fclose(fi);
       fs = fopen("ititle.idx","w+");
       GravaSecundaria(fs,indiceS);
       fclose(fs);
   }
  }while(op != 2);
}else //Se não for a primeira execução do programa
{
 fclose(fp);//Fecho o arquivo de Dados, uma vez que eu verifiquei que ele existe
 fi = fopen("iprimary.idx","r");
 char c = fgetc(fi);
 fclose(fi);
    if(c =='1'){
      //Leio o arquivo de indice primario e carrego na ram
      indiceP = lePrimario(fi,indiceP);
    }
    else{
      //Se tiver desatualizado eu pego do arquivo de palavras
      indiceP = carregaFilmes(fp,indiceP);
    }
fs = fopen("ititle.idx","r");
c = fgetc(fs);
fclose(fs);
    if(c == '1'){//Leio o arquivo de indice secundario e carrego na ram
      indiceS = leSecundario(fs,indiceS);
    }else{
      indiceS = carregaFilmeSecundaria(fp,indiceS);
    }
 fclose(fi);

//Menu para ver o que o usuário deseja fazer - caso não seja a primeira vez que ele rode o programa
do{
    printf("\nAtencao! Para funcionamento correto do programa, nao utilize caracteres especiais(ex: acentos,cedilha)\n");
    printf("Menu Filmes\n");
    printf("1 - Inserir novo filme\n2 - Buscar pelo titulo em Portugues\n3 - Buscar pelo codigo\n4 - Alterar avaliacao\n5 - Remover Filme\n6 - Listar Filmes\n7 - Sair\n");
    scanf("%d",&op);
    if(op<1||op>7){
        printf("Opçao invalida!\n");
    }
    switch (op){
        case 1://Altero a flag quando eu insiro um filme
             if(flagP == 1){
              flagP = 0;
              fi = fopen("iprimary.idx","w");
              fprintf(fi,"0@");
              fclose(fi);
            }
            if(flagS == 1){
              flagS = 0;
              fs = fopen("ititle.idx","w");
              fprintf(fs,"0@");
              fclose(fs);
            }

            printf("Titulo do Filme em Portugues: ");
            scanf(" %44[^\n]",filme.tituloPT);//Garanto que pegue strings com espaço e sem atrapalhar o buffer da próxima leitura
            printf("Titulo do Filme Original: ");
            scanf(" %44[^\n]",filme.tituloOriginal);
            printf("Insira o nome do Diretor(formato SOBRENOME,nome): ");
            scanf(" %44[^\n]",filme.diretor);//Parando antes do /0
             printf("Ano de lancamento: ");
            scanf(" %4[^\n]", filme.anoLancado);
            while (getchar() != '\n');

            printf("Insira o pais onde foi produzido: ");
            scanf(" %44[^\n]", filme.paisOrigem);
            while (getchar() != '\n');

            printf("Nota (0 a 9): ");
            scanf("%s",filme.nota);
            
            // Concatenando o código do filme
            strncpy(codAux, filme.diretor, 3); //A função 'strncpy' realiza a cópia de alguns caracteres especificados da string -  no caso as 3 primeiras letras do diretor
            strncpy(codAux + 3, filme.anoLancado + 2, 2);
            codAux[5] = '\0'; //Terminador do código

            strcpy(filme.codigo,codAux);
        
            auxRRN = acharRRN(fp);
            escreverArquivo(fp,filme,indiceP);
            
            aux = criarIndiceP(auxRRN,filme.codigo);
            indiceP = inserirLista(indiceP,aux);

            auxS = criarIndiceS(filme.codigo,filme.tituloPT);
            indiceS = inserirListaS(indiceS,auxS);

            if(flagP == 1){
              flagP = 0;
              flagS = 0;
              atualizaFlag(fi,"iprimary.idx");
              atualizaFlag(fs,"ititle.idx");
            }

            break;
        case 2:
            printf("Insira o titulo buscado: ");
            scanf(" %[^\n]s\n", filmeBuscado);
            buscaTitulo(filmeBuscado,indiceS,indiceP,fp);
              break;
        case 3:
            printf("Insira o codigo do filme buscado(3 letras maiuscula e 2 numeros): ");
            scanf(" %s", filmeBuscado);
            buscaCodigo(filmeBuscado,indiceP,fp);
            break;
        case 4:
            printf("Insira o codigo do filme que deseja alterar a avaliacao: ");
            scanf(" %s", filmeBuscado);
            alteraNota(filmeBuscado,indiceP,fp);
            break;
        case 5://Mexo na flag quando eu removo um filme
          if(flagP == 1){
              flagP = 0;
              fi = fopen("iprimary.idx","w");
              fprintf(fi,"0@");
              fclose(fi);
            }
            if(flagS == 1){
              flagS = 0;
              fs = fopen("ititle.idx","w");
              fprintf(fs,"0@");
              fclose(fs);
            }

          printf("Insira o codigo do filme que deseja remover: ");
          scanf(" %s",filmeBuscado);
          removerFilme(filmeBuscado,indiceP,fp);//Chamo as 3 funções que removem do arquivo e das 2 listas de indice
          removeFilme(indiceP,filmeBuscado);
          removeFilmeSecundaria(indiceS,filmeBuscado);
          break;               
        case 6:
            leFilmes(fp);
            break;
        case 7://Para sair do programa eu salvo o arquivo de indice atualizado
                fi = fopen("iprimary.idx","w+");
                GravaPrimaria(fi,indiceP);
                fclose(fi);
                fs = fopen("ititle.idx","w+");
                GravaSecundaria(fs,indiceS);
                fclose(fs);
                return 0 ;  
    }
  
  }while(!(op<1&&op>7));
}//Fim do else


}