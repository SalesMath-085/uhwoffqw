#include "hash_idx.h"
#include <stdlib.h>
#include <stdio.h>

static int hash_func(int chave) {
    return chave % HASH_CAP;
}

HashIdx* hash_idx_criar(void) {
    HashIdx* idx = (HashIdx*) malloc(sizeof(HashIdx));
    for (int i = 0; i < HASH_CAP; i++) idx->tabela[i] = NULL;
    idx->tamanho = 0;
    idx->colisoes = 0;
    return idx;
}

void hash_idx_destruir(HashIdx* idx) {
    if (!idx) return;
    for (int i = 0; i < HASH_CAP; i++) {
        NoHash* cur = idx->tabela[i];
        while (cur) {
            NoHash* prox = cur->prox;
            free(cur);
            cur = prox;
        }
    }
    free(idx);
}

int hash_idx_inserir(HashIdx* idx, int chave, long num_reg) {
    int pos = hash_func(chave);
    if (idx->tabela[pos] != NULL) idx->colisoes++;
    NoHash* novo = (NoHash*) malloc(sizeof(NoHash));
    novo->chave = chave;
    novo->num_reg = num_reg;
    novo->prox = idx->tabela[pos];
    idx->tabela[pos] = novo;
    idx->tamanho++;
    return 1;
}

long hash_idx_buscar(const HashIdx* idx, int chave) {
    int pos = hash_func(chave);
    NoHash* cur = idx->tabela[pos];
    while (cur) {
        if (cur->chave == chave) return cur->num_reg;
        cur = cur->prox;
    }
    return -1;
}

void hash_idx_imprimir(const HashIdx* idx) {
    for (int i = 0; i < HASH_CAP; i++) {
        NoHash* cur = idx->tabela[i];
        while (cur) {
            printf("  pos=%d <chave=%d, reg=%ld>\n", i, cur->chave, cur->num_reg);
            cur = cur->prox;
        }
    }
}

int hash_idx_colisoes(const HashIdx* idx) {
    return idx->colisoes;
}
