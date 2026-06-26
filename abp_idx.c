#include "abp_idx.h"
#include <stdlib.h>
#include <stdio.h>

ABPIdx* abp_idx_criar(void) {
    ABPIdx* idx = (ABPIdx*) malloc(sizeof(ABPIdx));
    idx->raiz = NULL;
    idx->tamanho = 0;
    return idx;
}

static void destruir_rec(NoABPIdx* no) {
    if (!no) return;
    destruir_rec(no->esq);
    destruir_rec(no->dir);
    free(no);
}

void abp_idx_destruir(ABPIdx* idx) {
    if (!idx) return;
    destruir_rec(idx->raiz);
    free(idx);
}

static NoABPIdx* inserir_rec(NoABPIdx* no, int chave, long num_reg, int* ok) {
    if (!no) {
        NoABPIdx* novo = (NoABPIdx*) malloc(sizeof(NoABPIdx));
        novo->chave = chave;
        novo->num_reg = num_reg;
        novo->esq = novo->dir = NULL;
        *ok = 1;
        return novo;
    }
    if (chave < no->chave) no->esq = inserir_rec(no->esq, chave, num_reg, ok);
    else if (chave > no->chave) no->dir = inserir_rec(no->dir, chave, num_reg, ok);
    else *ok = 0;
    return no;
}

int abp_idx_inserir(ABPIdx* idx, int chave, long num_reg) {
    int ok = 0;
    idx->raiz = inserir_rec(idx->raiz, chave, num_reg, &ok);
    if (ok) idx->tamanho++;
    return ok;
}

long abp_idx_buscar(const ABPIdx* idx, int chave) {
    NoABPIdx* cur = idx->raiz;
    while (cur) {
        if (chave == cur->chave) return cur->num_reg;
        if (chave < cur->chave) cur = cur->esq;
        else cur = cur->dir;
    }
    return -1;
}

static void imprimir_rec(NoABPIdx* no) {
    if (!no) return;
    imprimir_rec(no->esq);
    printf("  <chave=%d, reg=%ld>\n", no->chave, no->num_reg);
    imprimir_rec(no->dir);
}

void abp_idx_imprimir(const ABPIdx* idx) {
    imprimir_rec(idx->raiz);
}
