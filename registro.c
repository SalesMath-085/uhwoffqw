#include "registro.h"
#include <stdio.h>

void registro_imprimir(const Registro* r) {
    printf("  Codigo: %d | Nome: %-38s | Cat: %-18s | Fab: %-22s | Preco: %8.2f | Estoque: %d\n",
           r->codigo, r->nome, r->categoria, r->fabricante, r->preco, r->estoque);
}
