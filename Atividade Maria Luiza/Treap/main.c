#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para representar um nó Treap
typedef struct TreapNo {
  int data;
  int prioridade;
  struct TreapNo *left, *right;
} TreapNo;

// Função para criar um novo nó Treap
TreapNo* novoTreapNo(int data) {
  TreapNo* no = (TreapNo*)malloc(sizeof(TreapNo));
  no->data = data;
  no->prioridade = rand() % 100;
  no->left = no->right = NULL;
  return no;
}

// Função para rotacionar para a esquerda
void RE(TreapNo** raiz) {
  TreapNo* R = (*raiz)->right;
  TreapNo* X = R->left;

  // Gira
  R->left = *raiz;
  (*raiz)->right = X;

  // Define uma nova raiz
  *raiz = R;
}

// Função para rotacionar para a direita
void RD(TreapNo** raiz) {
  TreapNo* L = (*raiz)->left;
  TreapNo* Y = L->right;

  // Gira
  L->right = *raiz;
  (*raiz)->left = Y;

  // Define uma nova raiz
  *raiz = L;
}

// Função recursiva para inserir um nó com prioridade na treap
void insertNo(TreapNo** raiz, int data) {
  // Caso base
  if (*raiz == NULL) {
      *raiz = novoTreapNo(data);
      return;
  }

  // Inserção na subárvore esquerda ou direita, conforme valor
  if (data < (*raiz)->data) {
    insertNo(&(*raiz)->left, data);

      // Rotação à direita se a propriedade de heap for violada
      if ((*raiz)->left != NULL && (*raiz)->left->prioridade > (*raiz)->prioridade) {
          RD(raiz);
      }
  } else {
      insertNo(&(*raiz)->right, data);

      // Rotação à esquerda se a propriedade de heap for violada
      if ((*raiz)->right != NULL && (*raiz)->right->prioridade > (*raiz)->prioridade) {
          RE(raiz);
      }
  }
}

// Função recursiva para buscar uma chave na treap
int pesquisarNo(TreapNo* raiz, int key) {
  // Se a chave não estiver presente
  if (raiz == NULL) {
      return 0;
  }

  // Se a chave for encontrada
  if (raiz->data == key) {
      return 1;
  }

  // Busca na subárvore esquerda ou direita
  if (key < raiz->data) {
      return pesquisarNo(raiz->left, key);
  }
  return pesquisarNo(raiz->right, key);
}

// Função recursiva para deletar uma chave de uma treap
void deleteNo(TreapNo** raiz, int key) {
  // Caso base: a chave não foi encontrada
  if (*raiz == NULL) {
      return;
  }

  // Busca na subárvore esquerda ou direita
  if (key < (*raiz)->data) {
    deleteNo(&(*raiz)->left, key);
  } else if (key > (*raiz)->data) {
    deleteNo(&(*raiz)->right, key);
  } else {
    // Caso 1: Nó folha
    if ((*raiz)->left == NULL && (*raiz)->right == NULL) {
        free(*raiz);
        *raiz = NULL;
    }
    // Caso 2: Nó com dois filhos
    else if ((*raiz)->left && (*raiz)->right) {
        if ((*raiz)->left->prioridade < (*raiz)->right->prioridade) {
            RE(raiz);
              deleteNo(&(*raiz)->left, key);
        } else {
            RD(raiz);
              deleteNo(&(*raiz)->right, key);
        }
    }
    // Caso 3: Nó com um filho
    else {
        TreapNo* filho = ((*raiz)->left) ? (*raiz)->left : (*raiz)->right;
        TreapNo* atual = *raiz;
        *raiz = filho;
        free(atual);
    }
  }
}

// Função para imprimir a treap
void printTreap(TreapNo *raiz, int espaco, int altura) {
  if (raiz == NULL) {
      return;
  }
  espaco += altura;

  // Imprime a subárvore direita primeiro
  printTreap(raiz->right, espaco, altura);
  printf("\n");

  // Imprime o nó atual após preencher com espaços
  for (int i = altura; i < espaco; i++) {
    printf(" ");
  }
  printf("%d(%d)\n", raiz->data, raiz->prioridade);

  // Imprime a subárvore esquerda
  printTreap(raiz->left, espaco, altura);
}

int main() {
    // Chaves de treap
    int keys[] = { 5, 2, 1, 4, 9, 8, 10 };
    int n = sizeof(keys) / sizeof(keys[0]);

    // Inicializa a treap
    TreapNo* raiz = NULL;
    srand(time(NULL));

    // Insere as chaves
    for (int i = 0; i < n; i++) {
      insertNo(&raiz, keys[i]);
    }

    printf("Treap construida:\n\n");
    printTreap(raiz, 0, 10);

    printf("\nDeletando o nó 1:\n\n");
    deleteNo(&raiz, 1);
    printTreap(raiz, 0, 10);

    printf("\nDeletando o nó 5:\n\n");
    deleteNo(&raiz, 5);
    printTreap(raiz, 0, 10);

    printf("\nDeletando o nó 9:\n\n");
    deleteNo(&raiz, 9);
    printTreap(raiz, 0, 10);

    return 0;
}
