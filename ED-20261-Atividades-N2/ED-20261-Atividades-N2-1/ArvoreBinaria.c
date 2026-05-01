/*--------------------------------------------------*/
/* FATEC-Ipiranga                                   */
/* ADS - Estrutura de Dados                         */
/* Id da Atividade: N2-1                            */
/* Objetivo: Explorando Árvore Binária              */
/*                                                  */
/* Autor: André Gondim Carlucci                     */
/* Data: 01/05/2026                                 */
/*--------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minhabib.h"

No *criarNo(int valor) {
    No *novo = (No *)malloc(sizeof(No));
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

No *inserir(No *raiz, int valor) {
    if (raiz == NULL) return criarNo(valor);
    if (valor < raiz->valor)
        raiz->esq = inserir(raiz->esq, valor);
    else if (valor > raiz->valor)
        raiz->dir = inserir(raiz->dir, valor);
    return raiz;
}

/* Nós Internos: possuem pelo menos um filho */
void imprimir_nos_internos(No* raiz) {
    if (raiz == NULL) return;
    if (raiz->esq != NULL || raiz->dir != NULL)
        printf("  %d\n", raiz->valor);
    imprimir_nos_internos(raiz->esq);
    imprimir_nos_internos(raiz->dir);
}

/* Nós Externos (Folhas): sem nenhum filho */
void imprimir_folhas(No* raiz) {
    if (raiz == NULL) return;
    if (raiz->esq == NULL && raiz->dir == NULL)
        printf("  %d\n", raiz->valor);
    imprimir_folhas(raiz->esq);
    imprimir_folhas(raiz->dir);
}

/* Calcula a altura de um nó (distância até a folha mais distante) */
int calcular_altura(No* no) {
    if (no == NULL) return -1;
    int altEsq = calcular_altura(no->esq);
    int altDir = calcular_altura(no->dir);
    return 1 + (altEsq > altDir ? altEsq : altDir);
}

/* Calcula a profundidade de um nó (distância até a raiz) */
int calcular_profundidade(No* raiz, int valor, int profundidade_atual) {
    if (raiz == NULL) return -1;
    if (raiz->valor == valor) return profundidade_atual;
    int esq = calcular_profundidade(raiz->esq, valor, profundidade_atual + 1);
    if (esq != -1) return esq;
    return calcular_profundidade(raiz->dir, valor, profundidade_atual + 1);
}

/* ---- Funções auxiliares para imprimir_niveis ---- */

/* Conta nós num nível específico (nível 0 = raiz) */
static int contar_nivel(No *no, int nivelAtual, int nivelAlvo) {
    if (no == NULL) return 0;
    if (nivelAtual == nivelAlvo) return 1;
    return contar_nivel(no->esq, nivelAtual + 1, nivelAlvo) +
           contar_nivel(no->dir, nivelAtual + 1, nivelAlvo);
}

/* Imprime os valores de todos os nós de um nível específico */
static void imprimir_nos_do_nivel(No *no, int nivelAtual, int nivelAlvo) {
    if (no == NULL) return;
    if (nivelAtual == nivelAlvo) {
        printf("  %d\n", no->valor);
        return;
    }
    imprimir_nos_do_nivel(no->esq, nivelAtual + 1, nivelAlvo);
    imprimir_nos_do_nivel(no->dir, nivelAtual + 1, nivelAlvo);
}

/*
 * Exibe todos os níveis da árvore.
 * nivel_atual deve ser chamado com 0 (representa a raiz = Nível 0).
 * Exibe do Nível 0 até o Nível n (altura total da árvore).
 */
void imprimir_niveis(No* raiz, int nivel_atual) {
    /* nivel_atual é usado como ponto de partida;
       a função percorre do nível 0 até a altura máxima */
    int totalNiveis = calcular_altura(raiz) + 1; /* +1 pois altura retorna arestas */
    for (int nivel = nivel_atual; nivel < nivel_atual + totalNiveis; nivel++) {
        int nivelRelativo = nivel - nivel_atual; /* nível lógico (0, 1, 2...) */
        printf("  Nivel %d:", nivelRelativo);
        if (contar_nivel(raiz, 0, nivelRelativo) == 0) {
            printf(" (vazio)\n");
        } else {
            printf("\n");
            imprimir_nos_do_nivel(raiz, 0, nivelRelativo);
        }
    }
}

/* Retorna 1 se encontrou o valor e imprime os ancestrais no caminho de volta */
static int _ancestrais_rec(No *no, int valor) {
    if (no == NULL) return 0;
    if (no->valor == valor) return 1;
    if (_ancestrais_rec(no->esq, valor) || _ancestrais_rec(no->dir, valor)) {
        printf("  %d\n", no->valor);
        return 1;
    }
    return 0;
}

/* Imprime os ancestrais do nó com o valor dado (do pai até a raiz) */
void imprimir_ancestrais(No* raiz, int valor) {
    _ancestrais_rec(raiz, valor);
}

/* Localiza um nó pelo valor */
static No *buscar_no(No *no, int valor) {
    if (no == NULL) return NULL;
    if (no->valor == valor) return no;
    No *resultado = buscar_no(no->esq, valor);
    if (resultado != NULL) return resultado;
    return buscar_no(no->dir, valor);
}

/* Imprime todos os descendentes de um nó (todos os nós da subárvore, exceto ele mesmo) */
void imprimir_descendentes(No* no) {
    if (no == NULL) return;
    /* Imprime filhos e todos os seus descendentes */
    if (no->esq != NULL) {
        printf("  %d\n", no->esq->valor);
        imprimir_descendentes(no->esq);
    }
    if (no->dir != NULL) {
        printf("  %d\n", no->dir->valor);
        imprimir_descendentes(no->dir);
    }
}

static void exibir_sub_arvore(No *no, char *prefixo, int ehFilhoDir) {
    if (no == NULL) return;

    printf("%s", prefixo);
    if (ehFilhoDir == -1) {
        printf("%d\n", no->valor);
    } else if (ehFilhoDir) {
        printf("└── %d\n", no->valor);
    } else {
        printf("├── %d\n", no->valor);
    }

    char novoPrefixo[256];
    if (ehFilhoDir == -1) {
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s", prefixo);
    } else if (ehFilhoDir) {
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s    ", prefixo);
    } else {
        snprintf(novoPrefixo, sizeof(novoPrefixo), "%s│   ", prefixo);
    }

    if (no->esq != NULL || no->dir != NULL) {
        if (no->esq != NULL)
            exibir_sub_arvore(no->esq, novoPrefixo, 0);
        if (no->dir != NULL)
            exibir_sub_arvore(no->dir, novoPrefixo, 1);
    }
}

void analisar_arvore(No* raiz, int valorBusca) {

    printf("==================================================\n");
    printf(" DIAGNÓSTICO GERAL DA ÁRVORE\n");
    printf("==================================================\n");

    /* Raiz */
    printf("\nRAIZ:\n");
    if (raiz != NULL)
        printf("  %d\n", raiz->valor);
    else
        printf("  (árvore vazia)\n");

    /* Nós Internos */
    printf("\nNÓS INTERNOS (possuem ao menos um filho):\n");
    imprimir_nos_internos(raiz);

    /* Nós Externos / Folhas */
    printf("\nNÓS EXTERNOS / FOLHAS (sem filhos):\n");
    imprimir_folhas(raiz);

    /* Níveis (Nível 0 = raiz) */
    printf("\nNÍVEIS DA ÁRVORE:\n");
    imprimir_niveis(raiz, 0);

    printf("\n==================================================\n");
    printf(" DIAGNÓSTICO ESPECÍFICO — Nó buscado: %d\n", valorBusca);
    printf("==================================================\n");

    No *alvo = buscar_no(raiz, valorBusca);
    if (alvo == NULL) {
        printf("  Nó %d não encontrado na árvore.\n", valorBusca);
        return;
    }

    int grau = 0;
    if (alvo->esq != NULL) grau++;
    if (alvo->dir != NULL) grau++;
    printf("\nGRAU DO NÓ %d:\n  %d filho(s)\n", valorBusca, grau);

    printf("\nANCESTRAIS DE %d (do pai até a raiz):\n", valorBusca);
    if (raiz->valor == valorBusca) {
        printf("  (é a raiz — sem ancestrais)\n");
    } else {
        imprimir_ancestrais(raiz, valorBusca);
    }

    printf("\nDESCENDENTES DE %d:\n", valorBusca);
    if (alvo->esq == NULL && alvo->dir == NULL) {
        printf("  (folha — sem descendentes)\n");
    } else {
        imprimir_descendentes(alvo);
    }

    int alt = calcular_altura(alvo);
    printf("\nALTURA DO NÓ %d:\n  %d\n", valorBusca, alt);
    int prof = calcular_profundidade(raiz, valorBusca, 0);
    printf("\nPROFUNDIDADE DO NÓ %d:\n  %d\n", valorBusca, prof);
    printf("\n==================================================\n");
    printf(" SUB-ÁRVORE COM RAIZ EM %d\n", valorBusca);
    printf("==================================================\n\n");
    exibir_sub_arvore(alvo, "", -1);
    printf("\n");
}
