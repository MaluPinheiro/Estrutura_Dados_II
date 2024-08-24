#include <stdio.h>
#include <stdlib.h>

/*
Crie em C a estrutura de uma árvore binária cuja informação seja um inteiro.
  • Verificar se o valor do filho direita e esquerda são nulos
  • Verificar se a altura da árvore >= 0;
  • Escreva funções que recebam um ponteiro para a raiz da árvore e façam:
  • Inserção de um elemento na árvore;
  • Pesquisa de um elemento na árvore;
  • Exclusão de um elemento na árvore.
*/

struct NO {
  int info;
  struct NO *esq;
  struct NO *dir;
};

typedef struct NO *ArvBin;

ArvBin *cria_ArvBin() {
  ArvBin *raiz = (ArvBin*)malloc(sizeof(ArvBin));
  if(raiz != NULL) {
    *raiz = NULL;
  }
  return raiz;
}

int altura_ArvBin(ArvBin* raiz){
  if(raiz == NULL){
    return -1;
  }
  else if(*raiz == NULL){
    return 0;
  }
  
  int alt_esq = altura_ArvBin(&((*raiz)->esq));
  int alt_dir = altura_ArvBin(&((*raiz)->dir));

  if(alt_esq > alt_dir){
    return (alt_esq + 1);
  }else{
    return (alt_dir + 1);
  }
}

int inserir(ArvBin *raiz, int valor) {
  if(raiz == NULL) {
    return 0;
  }

  struct NO *novo;
  novo = (struct NO*)malloc(sizeof(struct NO));
  if(novo == NULL) {
    return 0;
  }

  novo->info = valor;
  novo->dir = NULL;
  novo->esq = NULL;

  if(*raiz == NULL) {
    *raiz = novo;
  }else{
    struct NO* atual = *raiz;
    struct NO* ant = NULL;
    while(atual != NULL){
      ant = atual;
      if(valor == atual->info){
        free(novo);
        return 0;
      }
      else if (valor > atual->info){
        atual = atual->dir;
      }
      else{
        atual = atual->esq;
      }
    }
    if(valor > ant->info){
      ant->dir = novo;
    }else{
      ant->esq = novo;
    }
  }
  return 1; 
}

int pesquisar(ArvBin *raiz, int valor){
  if(*raiz == NULL){
    return 0;
  }
  struct NO *atual = *raiz;
  while(atual != NULL){
    if(valor == atual->info){
      return 1;
    }else if(valor > atual->info){
      atual = atual->dir;
    }else{
      atual = atual->esq;
    }
  }
  return 0;
}

int excluir(ArvBin *raiz, int valor){
  if(*raiz == NULL){
    return 0;
  }

  struct NO *atual = *raiz;
  struct NO *ant = NULL;
  struct NO *aux;

  if(atual == NULL){
    return 0;
  }
  
  while(atual != NULL && atual->info != valor){
    ant = atual;
    if(valor > atual->info){
      atual = atual->dir;
    }else{
      atual = atual->esq;
    }
  }

  // Nó folha
  if(atual->dir == NULL && atual->esq == NULL){
    if(ant == NULL){
      *raiz = NULL;
    }else if(ant->dir == atual){
      ant->dir = NULL;
    }else{
      ant->esq = NULL;
    }
  }
  // Nó com 1 filho
  else if(atual->esq == NULL){
    if(ant == NULL)
      *raiz = atual->dir;
    else if(ant->dir == atual)
      ant->dir = atual->dir;
    else
      ant->esq = atual->dir;
  }else if(atual->dir == NULL){
    if(atual->dir == NULL)
      *raiz = atual->esq;
    else if(ant->dir == atual)
      ant->dir = atual->esq;
    else
      ant->esq = atual->esq;
  }
  // Nó com 2 filhos
  else{
    aux = atual->dir;
    ant = atual;
    while(aux->esq != NULL){
      ant = aux;
      aux = aux->esq;
    }
    atual->info = aux->info;
    if(ant == atual)
      ant->dir = aux->dir;
    else
      ant->esq = aux->esq;
  }
  free(atual);
  return 1;
}

int main(void) {
  ArvBin* raiz = cria_ArvBin();
  int valor;
  int N = 8,dados[8] = {50,100,30,20,40,45,35,37};
  for(int i=0;i<N;i++){
    inserir(raiz, dados[i]);
  }

  int altura = altura_ArvBin(raiz);
  printf("\nAltura: %d \n",altura);

  excluir(raiz, 30);

  printf("Insira o valor que deseja procurar: "); scanf("%d", &valor);
  if(pesquisar(raiz, valor))
    printf("Valor %d encontrado na arvore\n", valor);
  else
    printf("Valor %d não encontrado na arvore\n", valor);

  return 0;
}
