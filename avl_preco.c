#include "avl_preco.h"
#include <stdlib.h>

static int max2(int a, int b) { return a > b ? a : b; }

static int altura_no(NoAVLPreco* no) {
    return no ? no->altura : -1;
}

static int fator(NoAVLPreco* no) {
    return no ? altura_no(no->esq) - altura_no(no->dir) : 0;
}

static void atualizar_altura(NoAVLPreco* no) {
    no->altura = 1 + max2(altura_no(no->esq), altura_no(no->dir));
}

static NoAVLPreco* rot_dir(NoAVLPreco* y) {
    NoAVLPreco* x = y->esq;
    y->esq = x->dir;
    x->dir = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

static NoAVLPreco* rot_esq(NoAVLPreco* x) {
    NoAVLPreco* y = x->dir;
    x->dir = y->esq;
    y->esq = x;
    atualizar_altura(x);
    atualizar_altura(y);
    return y;
}

static NoAVLPreco* balancear(NoAVLPreco* no) {
    atualizar_altura(no);
    int fb = fator(no);
    if (fb > 1) {
        if (fator(no->esq) < 0) no->esq = rot_esq(no->esq);
        return rot_dir(no);
    }
    if (fb < -1) {
        if (fator(no->dir) > 0) no->dir = rot_dir(no->dir);
        return rot_esq(no);
    }
    return no;
}

AVLPreco* avl_preco_criar(void) {
    AVLPreco* idx = (AVLPreco*) malloc(sizeof(AVLPreco));
    idx->raiz = NULL;
    idx->tamanho = 0;
    return idx;
}

static void destruir_rec(NoAVLPreco* no) {
    if (!no) return;
    destruir_rec(no->esq);
    destruir_rec(no->dir);
    free(no);
}

void avl_preco_destruir(AVLPreco* idx) {
    if (!idx) return;
    destruir_rec(idx->raiz);
    free(idx);
}

static NoAVLPreco* inserir_rec(NoAVLPreco* no, float chave, long num_reg) {
    if (!no) {
        NoAVLPreco* novo = (NoAVLPreco*) malloc(sizeof(NoAVLPreco));
        novo->chave = chave;
        novo->num_reg = num_reg;
        novo->altura = 0;
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (chave < no->chave) no->esq = inserir_rec(no->esq, chave, num_reg);
    else no->dir = inserir_rec(no->dir, chave, num_reg);
    return balancear(no);
}

int avl_preco_inserir(AVLPreco* idx, float chave, long num_reg) {
    idx->raiz = inserir_rec(idx->raiz, chave, num_reg);
    idx->tamanho++;
    return 1;
}

static int range_rec(NoAVLPreco* no, char op, float valor, long* res, int max_res, int count) {
    if (!no || count >= max_res) return count;
    
    int match = 0;
    if (op == '>' && no->chave > valor) match = 1;
    else if (op == '<' && no->chave < valor) match = 1;
    else if (op == 'G' && no->chave >= valor) match = 1;
    else if (op == 'L' && no->chave <= valor) match = 1;

    
    if (op == '>' || op == 'G') {
        if ((op == '>' && no->chave <= valor) || (op == 'G' && no->chave < valor)) {
            
            count = range_rec(no->dir, op, valor, res, max_res, count);
        } else {
            
            count = range_rec(no->esq, op, valor, res, max_res, count);
            if (match && count < max_res) res[count++] = no->num_reg;
            count = range_rec(no->dir, op, valor, res, max_res, count);
        }
    }
   
    else { 
        if ((op == '<' && no->chave >= valor) || (op == 'L' && no->chave > valor)) {
            
            count = range_rec(no->esq, op, valor, res, max_res, count);
        } else {
           
            count = range_rec(no->esq, op, valor, res, max_res, count);
            if (match && count < max_res) res[count++] = no->num_reg;
            count = range_rec(no->dir, op, valor, res, max_res, count);
        }
    }
    return count;
}

int avl_preco_busca_range(const AVLPreco* idx, char op, float valor, long* resultados, int max_res) {
    return range_rec(idx->raiz, op, valor, resultados, max_res, 0);
}
