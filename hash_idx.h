#ifndef HASH_IDX_H
#define HASH_IDX_H

#define HASH_CAP 9973

typedef struct NoHash {
    int chave;
    long num_reg;
    struct NoHash* prox;
} NoHash;

typedef struct {
    NoHash* tabela[HASH_CAP];
    int tamanho;
    int colisoes;
} HashIdx;

HashIdx* hash_idx_criar(void);
void hash_idx_destruir(HashIdx* idx);
int hash_idx_inserir(HashIdx* idx, int chave, long num_reg);
long hash_idx_buscar(const HashIdx* idx, int chave);
void hash_idx_imprimir(const HashIdx* idx);
int hash_idx_colisoes(const HashIdx* idx);

#endif
