#include <stdio.h>
#include <stdlib.h>

struct NO {
  int info;
  int alt;
  struct NO *esq;
  struct NO *dir;
};

typedef struct NO *ArvAVL;

ArvAVL *cria_ArvAVL() {
  ArvAVL *raiz = (ArvAVL*)malloc(sizeof(ArvAVL));
  if(raiz != NULL) {
    *raiz = NULL;
  }
  return raiz;
}

int alt_No(struct NO* no){
  if(no == NULL)
    return -1;
  else
    return no->alt;
}

int fb_No(struct NO* no){
  return labs(alt_No(no->esq) - alt_No(no->dir));
}

int maior(int x, int y){
  if(x>y)
    return x;
  else
    return y;
}

void RotacaoEsquerda(ArvAVL *raiz) { // Antiga RotacaoDuplaDireita
  struct NO *no;
  no = (*raiz)->dir;
  (*raiz)->dir = no->esq;
  no->esq = *raiz;
  (*raiz)->alt = maior(alt_No((*raiz)->esq), alt_No((*raiz)->dir)) + 1;
  no->alt = maior(alt_No(no->dir), (*raiz)->alt) + 1;
  (*raiz) = no;
}

void RotacaoDireita(ArvAVL *raiz) { // Antiga RotacaoDuplaEsquerda
  struct NO *no;
  no = (*raiz)->esq;
  (*raiz)->esq = no->dir;
  no->dir = *raiz;
  (*raiz)->alt = maior(alt_No((*raiz)->esq), alt_No((*raiz)->dir)) + 1;
  no->alt = maior(alt_No(no->esq), (*raiz)->alt) + 1;
  (*raiz) = no;
}

void RotacaoEsqDir(ArvAVL *raiz) {
  RotacaoEsquerda(&(*raiz)->esq);
  RotacaoDireita(raiz);
}

void RotacaoDirEsq(ArvAVL *raiz) {
  RotacaoDireita(&(*raiz)->dir);
  RotacaoEsquerda(raiz);
}

int inserir_ArvAVL(ArvAVL *raiz, int valor) {
  int res;
  if (*raiz == NULL) {
      struct NO *novo;
      novo = (struct NO*)malloc(sizeof(struct NO));
      if (novo == NULL)
          return 0;

      novo->info = valor;
      novo->alt = 0;
      novo->esq = NULL;
      novo->dir = NULL;
      *raiz = novo;
      return 1;
  }

  struct NO *atual = *raiz;
  if (valor < atual->info) {
      if ((res = inserir_ArvAVL(&(atual->esq), valor)) == 1) {
          if (fb_No(atual) >= 2) {
              if (valor < (*raiz)->esq->info) {
                  RotacaoDireita(raiz);
              } else {
                  RotacaoEsqDir(raiz);
              }
          }
      }
  } else if (valor > atual->info) {
      if ((res = inserir_ArvAVL(&(atual->dir), valor)) == 1) {
          if (fb_No(atual) >= 2) {
              if ((*raiz)->dir->info < valor) {
                  RotacaoEsquerda(raiz);
              } else {
                  RotacaoDirEsq(raiz);
              }
          }
      }
  } else {
      printf("Valor duplicado!!\n");
      return 0;
  }

  atual->alt = maior(alt_No(atual->esq), alt_No(atual->dir)) + 1;
  return res;
}

int pesquisar(ArvAVL *raiz, int valor) {
  if (*raiz == NULL) {
      return 0;
  }
  struct NO *atual = *raiz;
  while (atual != NULL) {
      if (valor == atual->info) {
          return 1;
      } else if (valor > atual->info) {
          atual = atual->dir;
      } else {
          atual = atual->esq;
      }
  }
  return 0;
}

struct NO* procuraMenor(struct NO* atual) {
  struct NO *no1 = atual;
  struct NO *no2 = atual->esq;
  while (no2 != NULL) {
      no1 = no2;
      no2 = no2->esq;
  }
  return no1;
}

int excluir(ArvAVL *raiz, int valor) {
  if (*raiz == NULL) {
      printf("Valor não existe.\n");
      return 0;
  }
  int res;

  if (valor < (*raiz)->info) {
      if ((res = excluir(&(*raiz)->esq, valor)) == 1) {
          if (fb_No(*raiz) >= 2) {
              if (alt_No((*raiz)->dir->esq) <= alt_No((*raiz)->dir->dir))
                  RotacaoEsquerda(raiz);
              else
                  RotacaoDirEsq(raiz);
          }
      }
  } else if ((*raiz)->info < valor) {
      if ((res = excluir(&(*raiz)->dir, valor)) == 1) {
          if (fb_No(*raiz) >= 2) {
              if (alt_No((*raiz)->esq->dir) <= alt_No((*raiz)->esq->esq))
                  RotacaoDireita(raiz);
              else
                  RotacaoEsqDir(raiz);
          }
      }
  } else {
      if (((*raiz)->esq == NULL || (*raiz)->dir == NULL)) {
          struct NO *velhoNo = (*raiz);
          if ((*raiz)->esq != NULL)
              *raiz = (*raiz)->esq;
          else
              *raiz = (*raiz)->dir;
          free(velhoNo);
      } else { // nó tem 2 filhos
          struct NO* temp = procuraMenor((*raiz)->dir);
          (*raiz)->info = temp->info;
          excluir(&(*raiz)->dir, (*raiz)->info);
          if (fb_No(*raiz) >= 2) {
              if (alt_No((*raiz)->esq->dir) <= alt_No((*raiz)->esq->esq))
                  RotacaoDireita(raiz);
              else
                  RotacaoEsqDir(raiz);
          }
      }
      return 1;
  }
  return res;
}


int main(void) {
  ArvAVL* raiz = cria_ArvAVL();
  int valor;
  int N = 0, i;
  printf("Entre a quantidade de elementos da arvore: "); scanf("%i", &N);
  int dados[N];
  for(i= 0; i<N; i++){
    printf("Insira o nó: \n"); scanf("%i", &valor);
    inserir_ArvAVL(raiz, valor);
    dados[i] = valor;
  }
  printf("Vetor: ");
  for(i=0; i<N; i++) {
      printf("| %i ", dados[i]);
  }
  printf("|\n");

  int num = 0;
  while(num != 4){
    printf("\n<------ MENU ------>\n");
    printf("0. Inserir.\n");
    printf("1. Ver altura.\n");
    printf("2. Fazer pesquisa.\n");
    printf("3. Excluir nó.\n");
    printf("4. Sair.\n");
    scanf("%i", &num);

    if(num == 0){
      printf("Digite o nó que queira inserir: "); scanf("%i", &valor);
      inserir_ArvAVL(raiz, valor);
      dados[N++] = valor;
      printf("Vetor: ");
      for(i=0; i<N; i++) {
          printf("| %i ", dados[i]);
      }
      printf("|\n");
    }
    else if(num == 1){
      int altura = alt_No(*raiz);
      printf("\nAltura: %d \n",altura);
    }
    else if(num == 2){
      printf("Insira o valor que deseja procurar: \n"); scanf("%d", &valor);
      if(pesquisar(raiz, valor))
        printf("Valor %d encontrado na arvore\n", valor);
      else
        printf("Valor %d não encontrado na arvore\n", valor);
    }
    else if(num == 3){
      printf("Inserir o nó que deseja excluir: "); scanf("%i", &valor);
      if (excluir(raiz, valor)) {
        int found = 0;
        for (i = 0; i < N; i++) {
            if (dados[i] == valor) {
                found = 1;
            }
            if (found && i < N - 1) {
                dados[i] = dados[i + 1];
            }
        }
        N--;
        printf("Vetor: ");
        for (i = 0; i < N; i++) {
            printf("| %i ", dados[i]);
        }
        printf("|\n");
      } else {
        printf("Valor não encontrado na árvore.\n");
      }
    }
  }
  printf("Encerrando...");
  return 0;
}