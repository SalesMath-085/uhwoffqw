#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "registro.h"

#define NOME_ARQUIVO "produtos.bin"

int arquivo_criar(const char* nome, int n);
long arquivo_total_registros(const char* nome);
int arquivo_ler_registro(const char* nome, long num_reg, Registro* dest);
int arquivo_busca_sequencial_chave(const char* nome, int codigo, Registro* dest);
int arquivo_busca_sequencial_preco(const char* nome, char op, float valor, int imprimir);

#endif
