#include "registro.h"
#include "arquivo.h"
#include "abp_idx.h"
#include "hash_idx.h"
#include "avl_preco.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h> // <-- ADICIONE ESTA LINHA AQUI NO TOPO
#endif

#define N 10000
#define N_CONSULTAS 30
#define MAX_RANGE 20000

static double obter_tempo(void) {
#ifdef _WIN32
    LARGE_INTEGER frequencia;
    LARGE_INTEGER tempo;
    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&tempo);
    return (double)tempo.QuadPart / (double)frequencia.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#endif
}

static void separador(void) {
    printf("\n==================================================\n");
}

static void teste_pequeno_caminhamento(void) {
    separador();
    printf("  TESTE COM POUCOS ELEMENTOS (CAMINHAMENTO)\n");
    separador();

    arquivo_criar("teste_pequeno.bin", 15);

    ABPIdx* abp_pequeno = abp_idx_criar();
    HashIdx* hash_pequeno = hash_idx_criar();

    FILE* f = fopen("teste_pequeno.bin", "rb");
    Registro r;
    long num = 0;
    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        abp_idx_inserir(abp_pequeno, r.codigo, num);
        hash_idx_inserir(hash_pequeno, r.codigo, num);
        num++;
    }
    fclose(f);

    printf("\n[Caminhamento ABP - Pares <chave, reg>]\n");
    abp_idx_imprimir(abp_pequeno);

    printf("\n[Caminhamento Tabela Hash - Pares <chave, reg>]\n");
    hash_idx_imprimir(hash_pequeno);

    abp_idx_destruir(abp_pequeno);
    hash_idx_destruir(hash_pequeno);
}

static void construir_indices(ABPIdx** abp, HashIdx** hash, AVLPreco** avl, long total) {
    *abp  = abp_idx_criar();
    *hash = hash_idx_criar();
    *avl  = avl_preco_criar();

    FILE* f = fopen(NOME_ARQUIVO, "rb");
    Registro r;
    long num = 0;
    while (fread(&r, sizeof(Registro), 1, f) == 1) {
        abp_idx_inserir(*abp,  r.codigo, num);
        hash_idx_inserir(*hash, r.codigo, num);
        avl_preco_inserir(*avl, r.preco,  num);
        num++;
    }
    fclose(f);
    (void)total;
}

static void busca_por_chave(ABPIdx* abp, HashIdx* hash) {
    separador();
    printf("  BUSCA PELO ATRIBUTO CHAVE \n");
    separador();

    int chaves[N_CONSULTAS];
    srand(9999);
    for (int i = 0; i < 15; i++) chaves[i] = (rand() % N) + 1;
    for (int i = 15; i < N_CONSULTAS; i++) chaves[i] = N + 1000 + rand() % 5000;

    double t_abp[N_CONSULTAS], t_hash[N_CONSULTAS], t_seq[N_CONSULTAS];

    for (int i = 0; i < N_CONSULTAS; i++) {
        double t0 = obter_tempo();
        long r = abp_idx_buscar(abp, chaves[i]);
        t_abp[i] = (obter_tempo() - t0) * 1e6;
        (void)r;

        t0 = obter_tempo();
        r = hash_idx_buscar(hash, chaves[i]);
        t_hash[i] = (obter_tempo() - t0) * 1e6;
        (void)r;

        t0 = obter_tempo();
        Registro reg;
        arquivo_busca_sequencial_chave(NOME_ARQUIVO, chaves[i], &reg);
        t_seq[i] = (obter_tempo() - t0) * 1e6;
    }

    printf("\n%-5s | %-8s | %-14s | %-14s | %-14s\n",
           "Busca", "Codigo", "ABP (us)", "Hash (us)", "Seq (us)");
    printf("---------------------------------------------------------------\n");

    double soma_abp = 0, soma_hash = 0, soma_seq = 0;
    for (int i = 0; i < N_CONSULTAS; i++) {
        soma_abp  += t_abp[i];
        soma_hash += t_hash[i];
        soma_seq  += t_seq[i];
        printf("%-5d | %-8d | %-14.3f | %-14.3f | %-14.3f\n",
               i+1, chaves[i], t_abp[i], t_hash[i], t_seq[i]);
    }

    printf("---------------------------------------------------------------\n");
    printf("MEDIA        |          | %-14.3f | %-14.3f | %-14.3f\n",
           soma_abp/N_CONSULTAS, soma_hash/N_CONSULTAS, soma_seq/N_CONSULTAS);

    printf("\nColisoes na tabela hash: %d\n", hash_idx_colisoes(hash));
}

static void busca_por_preco(AVLPreco* avl) {
    separador();
    printf("  BUSCA POR PRECO \n");
    separador();

    float valores[N_CONSULTAS];
    srand(7777);
    for (int i = 0; i < N_CONSULTAS; i++) {
       valores[i] = 50.0f + (float)(rand() % 20000) / 100.0f;
    }

    long* resultados = (long*) malloc(sizeof(long) * MAX_RANGE);

    double t_avl[N_CONSULTAS], t_seq[N_CONSULTAS];
    int cnt_avl[N_CONSULTAS], cnt_seq[N_CONSULTAS];

    for (int i = 0; i < N_CONSULTAS; i++) {
        double t0 = obter_tempo();
        cnt_avl[i] = avl_preco_busca_range(avl, '>', valores[i], resultados, MAX_RANGE);
        t_avl[i] = (obter_tempo() - t0) * 1e6;

        t0 = obter_tempo();
        cnt_seq[i] = arquivo_busca_sequencial_preco(NOME_ARQUIVO, '>', valores[i], 0);
        t_seq[i] = (obter_tempo() - t0) * 1e6;
    }

    printf("\n%-5s | %-10s | %-14s | %-7s | %-14s | %-7s\n",
           "Busca", "Preco>", "AVL (us)", "Achou", "Seq (us)", "Achou");
    printf("--------------------------------------------------------------------------\n");

    double soma_avl = 0, soma_seq = 0;
    for (int i = 0; i < N_CONSULTAS; i++) {
        soma_avl += t_avl[i];
        soma_seq += t_seq[i];
        printf("%-5d | %-10.2f | %-14.3f | %-7d | %-14.3f | %-7d\n",
               i+1, valores[i], t_avl[i], cnt_avl[i], t_seq[i], cnt_seq[i]);
    }
    printf("--------------------------------------------------------------------------\n");
    printf("MEDIA        |            | %-14.3f |         | %-14.3f |\n",
           soma_avl/N_CONSULTAS, soma_seq/N_CONSULTAS);

    // --- A PARTE CORRIGIDA ENTRA AQUI ---
    separador();
    printf("  DEMONSTRACAO DA LISTA DE REGISTROS (Preco > %.2f)\n", valores[0]);
    separador();
    
    // REFAZ a primeira busca para garantir que o array 'resultados' tenha os dados certos
    int cnt_demo = avl_preco_busca_range(avl, '>', valores[0], resultados, MAX_RANGE);
    
    if (cnt_demo > 0) {
        printf("Exibindo os primeiros registos encontrados pela AVL:\n");
        int limite_exibicao = (cnt_demo < 5) ? cnt_demo : 5; 
        
        for(int j = 0; j < limite_exibicao; j++) { 
             Registro reg;
             if (arquivo_ler_registro(NOME_ARQUIVO, resultados[j], &reg)) {
                 registro_imprimir(&reg);
             }
        }
        if (cnt_demo > 5) {
            printf("... (Omitindo os restantes. Total de %d registos encontrados nesta consulta)\n", cnt_demo);
        }
    } else {
        printf("Nenhum registo encontrado para Preco > %.2f nesta consulta.\n", valores[0]);
    }
    
    free(resultados);
}

static void tamanho_registro(void) {
    separador();
    printf("  INFORMACOES DO ARQUIVO E REGISTRO\n");
    separador();
    printf("\nTamanho do struct Registro: %zu bytes\n", sizeof(Registro));
    long total = arquivo_total_registros(NOME_ARQUIVO);
    printf("Total de registros no arquivo: %ld\n", total);
    printf("Tamanho total do arquivo: %ld bytes (~%.2f MB)\n",
           total * (long)sizeof(Registro),
           total * (double)sizeof(Registro) / 1024.0 / 1024.0);
}

int main(void) {
    printf("  TRABALHO 3 - AED2 - INDEXACAO DE ARQUIVO\n");
    printf("  Cadastro de Produtos\n");

    teste_pequeno_caminhamento();

    printf("\n-> Criando arquivo com %d registros para medicao de tempo...\n", N);
    if (!arquivo_criar(NOME_ARQUIVO, N)) {
        fprintf(stderr, "Erro ao criar arquivo.\n");
        return 1;
    }
    printf("   Arquivo '%s' criado.\n", NOME_ARQUIVO);

    tamanho_registro();

    long total = arquivo_total_registros(NOME_ARQUIVO);

    printf("\n Construindo indices...\n");
    ABPIdx* abp  = NULL;
    HashIdx* hash = NULL;
    AVLPreco* avl  = NULL;
    construir_indices(&abp, &hash, &avl, total);
    printf("   Indices construidos. ABP: %d nos | Hash: %d entradas | AVL: %d nos\n",
           abp->tamanho, hash->tamanho, avl->tamanho);

    busca_por_chave(abp, hash);
    busca_por_preco(avl);

    separador();
    printf("  EXEMPLO: leitura direta de registro via indice ABP\n");
    separador();
    int exemplo = 42;
    long nr = abp_idx_buscar(abp, exemplo);
    if (nr >= 0) {
        Registro r;
        arquivo_ler_registro(NOME_ARQUIVO, nr, &r);
        printf("\nRegistro com codigo %d (num_reg=%ld):\n", exemplo, nr);
        registro_imprimir(&r);
    }

    abp_idx_destruir(abp);
    hash_idx_destruir(hash);
    avl_preco_destruir(avl);

    return 0;
}
