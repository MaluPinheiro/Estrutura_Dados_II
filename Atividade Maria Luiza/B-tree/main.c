#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 4 // Grau máximo da árvore-B

struct NoBTree {
    int num_chaves; // Número de chaves atualmente no nó
    int chaves[M-1]; // Array de chaves
    struct NoBTree *filhos[M]; // Array de ponteiros para os filhos
    bool eh_folha; // Verdadeiro se o nó é uma folha
};

// Função para criar um novo nó
struct NoBTree *criarNo(bool eh_folha) {
    struct NoBTree *novoNo = (struct NoBTree *)malloc(sizeof(struct NoBTree));
    if (novoNo == NULL) {
        perror("Falha na alocação de memória");
        exit(EXIT_FAILURE);
    }
    novoNo->num_chaves = 0;
    novoNo->eh_folha = eh_folha;
    for (int i = 0; i < M; i++) {
        novoNo->filhos[i] = NULL;
    }
    return novoNo;
}

// Função para dividir um nó filho cheio
void dividirFilho(struct NoBTree *pai, int indice) {
    struct NoBTree *filho = pai->filhos[indice];
    struct NoBTree *novoNo = criarNo(filho->eh_folha);

    novoNo->num_chaves = M / 2 - 1;

    // Move chaves e filhos para o novo nó
    for (int i = 0; i < M / 2 - 1; i++) {
        novoNo->chaves[i] = filho->chaves[i + M / 2];
    }

    if (!filho->eh_folha) {
        for (int i = 0; i < M / 2; i++) {
            novoNo->filhos[i] = filho->filhos[i + M / 2];
        }
    }

    filho->num_chaves = M / 2 - 1;

    // Desloca os filhos do pai para dar espaço para o novo nó
    for (int i = pai->num_chaves; i > indice; i--) {
        pai->filhos[i + 1] = pai->filhos[i];
    }

    pai->filhos[indice + 1] = novoNo;

    // Desloca as chaves do pai para inserir a chave do meio do filho
    for (int i = pai->num_chaves - 1; i >= indice; i--) {
        pai->chaves[i + 1] = pai->chaves[i];
    }

    pai->chaves[indice] = filho->chaves[M / 2 - 1];
    pai->num_chaves++;
}

// Função para inserir uma chave em um nó não cheio
void inserirNaoCheio(struct NoBTree *no, int chave) {
    int i = no->num_chaves - 1;

    if (no->eh_folha) {
        // Insere a chave em ordem
        while (i >= 0 && no->chaves[i] > chave) {
            no->chaves[i + 1] = no->chaves[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->num_chaves++;
    } else {
        // Encontra o filho onde a chave deve ser inserida
        while (i >= 0 && no->chaves[i] > chave) {
            i--;
        }
        i++;

        if (no->filhos[i]->num_chaves == M - 1) {
            // Divide o filho se estiver cheio
            dividirFilho(no, i);

            // Determina qual dos dois filhos será o novo
            if (no->chaves[i] < chave) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

// Função para inserir uma chave na árvore-B
void inserir(struct NoBTree **raiz, int chave) {
    struct NoBTree *no = *raiz;

    if (no == NULL) {
        // Cria um novo nó raiz
        *raiz = criarNo(true);
        (*raiz)->chaves[0] = chave;
        (*raiz)->num_chaves = 1;
    } else {
        if (no->num_chaves == M - 1) {
            // Divide a raiz se estiver cheia
            struct NoBTree *nova_raiz = criarNo(false);
            nova_raiz->filhos[0] = no;
            dividirFilho(nova_raiz, 0);
            *raiz = nova_raiz;
        }
        inserirNaoCheio(*raiz, chave);
    }
}

// Função para percorrer e imprimir a árvore-B em ordem
void percorrer(struct NoBTree *raiz) {
    if (raiz != NULL) {
        int i;
        for (i = 0; i < raiz->num_chaves; i++) {
            percorrer(raiz->filhos[i]);
            printf("%d ", raiz->chaves[i]);
        }
        percorrer(raiz->filhos[i]);
    }
}

// Função principal para testar a implementação da árvore-B
int main() {
    struct NoBTree *raiz = NULL;

    inserir(&raiz, 10);
    inserir(&raiz, 20);
    inserir(&raiz, 5);
    inserir(&raiz, 6);
    inserir(&raiz, 12);
    inserir(&raiz, 30);

    printf("Percorrendo a árvore-B em ordem: ");
    percorrer(raiz);
    printf("\n");

    return 0;
}
