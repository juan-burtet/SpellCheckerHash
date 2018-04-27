#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Nome de no máximo 99 caracteres
typedef struct{
  char palavra[100];
}Palavra;

// Celula da tabela
typedef struct Celula{
  Palavra palavra;
  struct Celula *prox;
}Celula;

// Tabela Hash;
typedef struct{
  Celula **tabela;
  float fatorCarga;
  unsigned int size;
  unsigned int n;
}Tabela;

// Trie
typedef struct Trie{
  Celula *info;
  struct Trie *letra[27]; // 0 - A; 1 - B; ... 25 - Z; 26 - '-'
}Trie;

//Funções da Tabela
Tabela *RESET(Tabela *tabela, unsigned size, unsigned n);
Tabela *adiciona_palavra(Tabela *tabela, Palavra *word);
Tabela *deleta_palavra(Tabela *tabela, Palavra *word);
Tabela *aumenta_tabela(Tabela *tabela);
Tabela *diminui_tabela(Tabela *tabela);
void free_tabela(Tabela *tabela);

//Funções da Trie
Trie *cria_trie(Trie *trie);
Trie *adiciona_trie(Trie *trie, Celula *info);
void lista_trie(Trie *trie);
void free_trie(Trie *trie);

//Função de Leitura de String
Palavra ler_string(void);

//Funções para testar palavras parecidas
int teste_palavras(Tabela *tabela, Palavra *word);
Trie *teste_letra_mais(Tabela *tabela, Trie *trie, Palavra *word);
Trie *teste_letra_menos(Tabela *tabela, Trie *trie, Palavra *word);
Trie *teste_letra_trocada(Tabela *tabela, Trie *trie, Palavra *word);
Trie *teste_letra_errada(Tabela *tabela, Trie *trie, Palavra *word);
Tabela *confere_palavra(Tabela *tabela, Palavra *word);

//Funções Hash e Rehash
unsigned long long int hash(char *word);
Tabela *rehash(Tabela *tabela);

//----------------------------------------------------------------------------------------------------
//---------------------------------------MAIN---------------------------------------------------------
//----------------------------------------------------------------------------------------------------

int main(void){
  Tabela *tabela = NULL;
  Palavra atual, armazenada;

  tabela = RESET(tabela, 50, 0);
  atual.palavra[0] = 0;
  armazenada.palavra[0] = 0;

  do{
    atual = ler_string();
    switch(atual.palavra[0]){
      // adiciona ultima palavra digitada ao dicionário
      case '+': tabela = adiciona_palavra(tabela, (Palavra *) armazenada.palavra);
                break;

      // remove ultima palavra digitada ao dicionário
      case '-': tabela = deleta_palavra(tabela, (Palavra *) armazenada.palavra);
                break;

      // sai do programa
      case '*':
                break;

      // consulta a palavra
      default:  armazenada = atual;
                tabela = confere_palavra(tabela, (Palavra *) armazenada.palavra);
                break;
    }//switch
  }while(atual.palavra[0] != '*');

  // Impressão da Tabela. (Exclui comentários caso queira imprimir);
  /*
  Celula *info = NULL;
  for(int i = 0; i < tabela->size; i++){
    printf("%d - ", i);
    if(tabela->tabela[i] != NULL){
      info = tabela->tabela[i];
      do{
        printf("%s ", info->palavra.palavra);
        printf("-> ");
        info = info->prox;
      }while(info != NULL);
    }
    printf("NULL\n");
  }*/

  free_tabela(tabela);
  return 0;
}//main

//----------------------------------------------------------------------------------------------------
//---------------------------------------FUNÇÕES------------------------------------------------------
//----------------------------------------------------------------------------------------------------

// Leitura de String
Palavra ler_string(void){
  Palavra word;

  scanf("%s", word.palavra);
  for(int i = 0; word.palavra[i] != 0; i++){
    if(word.palavra[i] >= 65 && word.palavra[i] <= 90)
      word.palavra[i] += 32;
  }
  return word;
}//ler_string

// inicializa a tabela hash
Tabela *RESET(Tabela *tabela, unsigned size, unsigned n){
  tabela = malloc(sizeof(Tabela));
  tabela->tabela = malloc(size*sizeof(int*));
  for(int i = 0; i < size; i++)
    tabela->tabela[i] = NULL;
  tabela->fatorCarga = (float) n/size;
  tabela->size = size;
  tabela->n = n;

  return tabela;
}//RESET

// Confere se a Palavra esta na tabela, em caso afirmativo, retorna seu indice. Em caso negativo, procura palavras parecidas
Tabela *confere_palavra(Tabela *tabela, Palavra *word){
  Celula *confere;
  unsigned long long int index;

  index = hash(word->palavra);
  index = index % tabela->size;

  confere = tabela->tabela[index];

  while(confere != NULL){
    if(strcmp(confere->palavra.palavra, word->palavra) == 0){
      printf("ok %s\n", word->palavra);
      return tabela;
    }//if

    confere = confere->prox;
  }//while

  if(teste_palavras(tabela,word) == 0)
    printf("not found\n");

  return tabela;
}//confere_palavra

//---------------------------------------------------------------
// Função que faz todos os testes
int teste_palavras(Tabela *tabela, Palavra *word){
  Trie *trie = NULL;

  trie = cria_trie(trie);
  trie = teste_letra_mais(tabela, trie, word);
  trie = teste_letra_menos(tabela, trie, word);
  trie = teste_letra_trocada(tabela, trie, word);
  trie = teste_letra_errada(tabela, trie, word);

  lista_trie(trie);

  for(int i = 0; i < 27; i++){
    if(trie->letra[i] != NULL){
      free_trie(trie);
      return 1;
    }//if
  }//for

  free_trie(trie);
  return 0;
}//teste_palavras

// Cria nó da Trie
Trie *cria_trie(Trie *trie){
  trie = malloc(sizeof(Trie));
  trie->info = NULL;
  for(int i = 0; i < 27; i++)
    trie->letra[i] = NULL;

  return trie;
}//cria_trie

// Adiciona palavra na trie;
Trie *adiciona_trie(Trie *trie, Celula *info){
  unsigned short int c;
  Trie *raiz = trie;
  Trie *no = NULL;

  for(int i = 0; info->palavra.palavra[i] != 0; i++){
    //Caso seja um hifen
    if(info->palavra.palavra[i] == '-'){

      if(trie->letra[26] == NULL){
        no = cria_trie(no);
        trie->letra[26] = no;
        no = NULL;
      }
      trie = trie->letra[26];
    }//if
    else{
      c = info->palavra.palavra[i];
      if(trie->letra[c - 97] == NULL){
        no = cria_trie(no);
        trie->letra[c - 97] = no;
        no = NULL;
      }
      trie = trie->letra[c - 97];
    }//else
  }//for

  trie->info = info;
  return raiz;
}//adiciona_trie

// Lista palavras da Trie
void lista_trie(Trie *trie){
  if(trie->info != NULL)
      printf("%s\n", trie->info->palavra.palavra);

  for(int i = 0; i < 27; i++){
    if(trie->letra[i] != NULL)
      lista_trie(trie->letra[i]);
  }//for
}//lista_trie

// Desaloca a trie;
void free_trie(Trie *trie){

  if(trie == NULL)
    return;

  for(int i = 0; i < 27; i++)
    free_trie(trie->letra[i]);

  free(trie);
}//free_trie

//Função que faz os testes de palavras com uma letra a mais
Trie *teste_letra_mais(Tabela *tabela, Trie *trie, Palavra *word){
  Palavra teste;
  unsigned int size;
  unsigned long long int index;
  Celula *confere;

  size = strlen(word->palavra);

  if(size == 99)
    return trie;

  for(int i = 0; i <= size; i++){
    teste = *word;
    for(int j = size; j >= i; j--)
      teste.palavra[j+1] = teste.palavra[j];

    for(int k = 97; k <= 123; k++){

      if(k == 123)
        teste.palavra[i] = '-';
      else
        teste.palavra[i] = k;

      index = hash(teste.palavra);
      index = index % tabela->size;
      confere = tabela->tabela[index];
      while(confere != NULL){
        if(strcmp(confere->palavra.palavra, teste.palavra) == 0){
          trie = adiciona_trie(trie, (Celula *) confere);
          break;
        }//if
        confere = confere->prox;
      }//while
    }//for
  }//for

  return trie;
}//teste_letra_mais

//Função que faz os testes de palavras com uma letra a menos
Trie *teste_letra_menos(Tabela *tabela, Trie *trie, Palavra *word){
  Palavra teste;
  unsigned int size;
  unsigned long long int index;
  Celula *confere;

  size = strlen(word->palavra);

  if(size == 1)
    return trie;

  for(int i = 0; word->palavra[i] != 0; i++){
    teste = *word;
    for(int j = i; j < size; j++)
      teste.palavra[j] = teste.palavra[j+1];

    index = hash(teste.palavra);
    index = index % tabela->size;
    confere = tabela->tabela[index];
    while(confere != NULL){
      if(strcmp(confere->palavra.palavra, teste.palavra) == 0){
        trie = adiciona_trie(trie, (Celula *) confere);
        break;
      }//if
      confere = confere->prox;
    }//while
  }//for

  return trie;
}//teste_letra_menos

//Função que faz os testes de palavras com letras trocadas
Trie *teste_letra_trocada(Tabela *tabela, Trie *trie, Palavra *word){
  Palavra teste;
  unsigned int size;
  unsigned long long int index;
  char aux;
  Celula *confere;

  size = strlen(word->palavra);

  if(size == 1)
    return trie;

  for(int i = 0; i < size - 1; i++){
    teste = *word;

    aux = teste.palavra[i];
    teste.palavra[i] = teste.palavra[i+1];
    teste.palavra[i+1] = aux;

    index = hash(teste.palavra);
    index = index % tabela->size;
    confere = tabela->tabela[index];
    while(confere != NULL){
      if(strcmp(confere->palavra.palavra, teste.palavra) == 0){
        trie = adiciona_trie(trie, (Celula *) confere);
        break;
      }//if
      confere = confere->prox;
    }//while
  }//for

  return trie;
}//teste_letra_trocada

//Função que faz os testes de palavras com letras erradas
Trie *teste_letra_errada(Tabela *tabela, Trie *trie, Palavra *word){
  Palavra teste;
  unsigned long long int index;
  unsigned int aux;
  Celula *confere;

  for(int i = 0; word->palavra[i] != 0; i++){
    teste = *word;
    aux = teste.palavra[i];
    for(int j = 97; j <= 123; j++){
      if(j == 123)
        j = 45;//'-'

      if(j != aux){
        teste.palavra[i] = j;

        index = hash(teste.palavra);
        index = index % tabela->size;
        confere = tabela->tabela[index];
        while(confere != NULL){
          if(strcmp(confere->palavra.palavra, teste.palavra) == 0){
            trie = adiciona_trie(trie, (Celula *) confere);
            break;
          }//if
          confere = confere->prox;
        }//while
      }//if

      if(j == 45)
        j = 123;
    }//for
  }//for

  return trie;
}//teste_letra_errada
//--------------------------------------------------------------

// adiciona um nome na tabela hash
Tabela *adiciona_palavra(Tabela *tabela, Palavra *word){
  Celula *celula = NULL;
  unsigned long long int index;

  if(word->palavra[0] == 0){
    printf("fail\n");
    return tabela;
  }

  index = hash(word->palavra);
  index = index % tabela->size;

  celula = tabela->tabela[index];
  while(celula != NULL){
    if(strcmp(celula->palavra.palavra, word->palavra) == 0){
      printf("fail %s\n", word->palavra);
      return tabela;
    }//if
    celula = celula->prox;
  }//while

  celula = malloc(sizeof(Celula));
  celula->palavra = *word;
  celula->prox = tabela->tabela[index];
  tabela->tabela[index] = celula;
  printf("ok %s\n", word->palavra);
  tabela = aumenta_tabela(tabela);
  if(tabela->fatorCarga > 3.0)
    tabela = rehash(tabela);

  return tabela;
}//adiciona_palavra

// exclui palavra da tabela hash
Tabela *deleta_palavra(Tabela *tabela, Palavra *word){
  Celula *celula = NULL;
  Celula *anterior = NULL;
  unsigned long long int index;

  if(word->palavra[0] == 0){
    printf("fail\n");
    return tabela;
  }

  index = hash(word->palavra);
  index = index % tabela->size;
  celula = tabela->tabela[index];

  if(celula == NULL){
    printf("fail %s\n", word->palavra);
    return tabela;
  }

  //Checar se a informação é a primeira da tabela hash
  if(strcmp(celula->palavra.palavra, word->palavra) == 0){
    tabela->tabela[index] = celula->prox;
    free(celula);
    printf("ok %s\n", word->palavra);
    tabela = diminui_tabela(tabela);
    return tabela;
  }//if

  anterior = celula;
  celula = celula->prox;
  while(celula != NULL){
    if(strcmp(celula->palavra.palavra, word->palavra) == 0){
      anterior->prox = celula->prox;
      free(celula);
      printf("ok %s\n", word->palavra);
      tabela = diminui_tabela(tabela);
      return tabela;
    }
    anterior = celula;
    celula = celula->prox;
  }//while

  printf("fail %s\n", word->palavra);
  return tabela;
}//deleta_palavra

//Aumenta o n e fator de carga
Tabela *aumenta_tabela(Tabela *tabela){
  tabela->n += 1;
  tabela->fatorCarga = (float) (tabela->n)/(tabela->size);
  return tabela;
}//aumenta_tabela

//Diminui o n e fator de carga
Tabela *diminui_tabela(Tabela *tabela){
  tabela->n -= 1;
  tabela->fatorCarga = (float) (tabela->n)/(tabela->size);
  return tabela;
}//diminui_tabela

// Libera a memória da Tabela
void free_tabela(Tabela *tabela){
  Celula *destroy = NULL;
  Celula *aux = NULL;

  for(int i = 0; i < tabela->size; i++){
    destroy = tabela->tabela[i];
    while(destroy != NULL){
      aux = destroy->prox;
      free(destroy);
      destroy = aux;
    }//while
  }//for

  free(tabela);
}//free_tabela

// Função hash -> fnv_hash_1a_32 - Fowler / Noll / Vo (FNV) Hash
unsigned long long int hash(char *word){
  unsigned long long int hash = 0xcbf29ce484222325ULL;

  for(int i = 0; word[i] != 0; i++)
    hash = (hash ^ word[i]) * 0x100000001b3ULL;

  return hash;
}//hash

// Função rehash
Tabela *rehash(Tabela *tabela){
  Tabela *novo = NULL;
  Celula *atual = NULL;
  Celula *aux = NULL;
  unsigned long long int index;

  novo = RESET(novo, 2 * tabela->size, tabela->n);

  for(int i = 0; i < tabela->size; i++){
    atual = tabela->tabela[i];
    while(atual != NULL){
      aux = atual;

      index = hash(atual->palavra.palavra);
      index = index % novo->size;

      atual = malloc(sizeof(Celula));
      atual->palavra = aux->palavra;
      atual->prox = novo->tabela[index];
      novo->tabela[index] = atual;

      atual = aux->prox;
      free(aux);
    }//while
  }//for

  free(tabela);
  return novo;
}//rehash
