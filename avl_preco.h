#ifndef AVL_PRECO_H
#define AVL_PRECO_H

typedef struct NoAVLPreco {
    float chave;
    long num_reg;
    int altura;
    struct NoAVLPreco* esq;
    struct NoAVLPreco* dir;
} NoAVLPreco;

typedef struct {
    NoAVLPreco* raiz;
    int tamanho;
} AVLPreco;

AVLPreco* avl_preco_criar(void);
void avl_preco_destruir(AVLPreco* idx);
int avl_preco_inserir(AVLPreco* idx, float chave, long num_reg);
int avl_preco_busca_range(const AVLPreco* idx, char op, float valor,
                          long* resultados, int max_res);

#endif
