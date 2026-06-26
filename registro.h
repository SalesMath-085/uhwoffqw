#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>

#define TAM_NOME 40
#define TAM_CATEGORIA 20
#define TAM_FABRICANTE 24

typedef struct {
    int codigo;
    char nome[TAM_NOME];
    char categoria[TAM_CATEGORIA];
    char fabricante[TAM_FABRICANTE];
    float preco;
    int estoque;
} Registro;

void registro_imprimir(const Registro* r);

#endif
