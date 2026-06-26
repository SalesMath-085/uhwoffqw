#ifndef ABP_IDX_H
#define ABP_IDX_H

typedef struct NoABPIdx {
    int chave;
    long num_reg;
    struct NoABPIdx* esq;
    struct NoABPIdx* dir;
} NoABPIdx;

typedef struct {
    NoABPIdx* raiz;
    int tamanho;
} ABPIdx;

ABPIdx* abp_idx_criar(void);
void abp_idx_destruir(ABPIdx* idx);
int abp_idx_inserir(ABPIdx* idx, int chave, long num_reg);
long abp_idx_buscar(const ABPIdx* idx, int chave);
void abp_idx_imprimir(const ABPIdx* idx);

#endif
