#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* categorias[] = {"Eletronico", "Alimento", "Vestuario", "Movel", "Ferramenta"};
static const char* fabricantes[] = {"AlphaCorp", "BetaInd", "GammaTec", "DeltaFab", "EpsilonMfg",
                                     "ZetaCo", "EtaLtda", "ThetaSA", "IotaGroup", "KappaBR"};

int arquivo_criar(const char* nome, int n) {
    FILE* f = fopen(nome, "wb");
    if (!f) return 0;
    srand(1234);

    int* codigos = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) codigos[i] = i + 1;

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = codigos[i];
        codigos[i] = codigos[j];
        codigos[j] = temp;
    }

    for (int i = 0; i < n; i++) {
        Registro r;
        r.codigo = codigos[i]; // Usa o código embaralhado!
        snprintf(r.nome, TAM_NOME, "Produto_%06d", r.codigo);
        strncpy(r.categoria, categorias[i % 5], TAM_CATEGORIA - 1);
        r.categoria[TAM_CATEGORIA - 1] = '\0';
        strncpy(r.fabricante, fabricantes[i % 10], TAM_FABRICANTE - 1);
        r.fabricante[TAM_FABRICANTE - 1] = '\0';
        r.preco = 5.0f + (float)(rand() % 99600) / 100.0f;
        r.estoque = rand() % 1000 + 1;
        fwrite(&r, sizeof(Registro), 1, f);
    }
    
    free(codigos);
    fclose(f);
    return 1;
}

long arquivo_total_registros(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long total = ftell(f) / sizeof(Registro);
    fclose(f);
    return total;
}

int arquivo_ler_registro(const char* nome, long num_reg, Registro* dest) {
    FILE* f = fopen(nome, "rb");
    if (!f) return 0;
    fseek(f, num_reg * (long)sizeof(Registro), SEEK_SET);
    int ok = (fread(dest, sizeof(Registro), 1, f) == 1);
    fclose(f);
    return ok;
}

int arquivo_busca_sequencial_chave(const char* nome, int codigo, Registro* dest) {
    FILE* f = fopen(nome, "rb");
    if (!f) return -1;
    Registro r;
    long num = 0;
    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        if (r.codigo == codigo) {
            if (dest) *dest = r;
            fclose(f);
            return (int)num;
        }
        num++;
    }
    fclose(f);
    return -1;
}

int arquivo_busca_sequencial_preco(const char* nome, char op, float valor, int imprimir) {
    FILE* f = fopen(nome, "rb");
    if (!f) return 0;
    Registro r;
    int count = 0;
    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        int match = 0;
        if (op == '>' && r.preco > valor) match = 1;
        else if (op == '<' && r.preco < valor) match = 1;
        else if (op == 'G' && r.preco >= valor) match = 1;
        else if (op == 'L' && r.preco <= valor) match = 1;
        if (match) {
            if (imprimir) registro_imprimir(&r);
            count++;
        }
    }
    fclose(f);
    return count;
}
