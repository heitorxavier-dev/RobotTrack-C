#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "caminho.h"

// Esse cara Verifica se um caractere é um comando válido tlgd
static int ComandoValido(char c) {
    return (c == 'N' || c == 'S' || c == 'L' || c == 'O');
}

// Esse cara Cria um novo nó de Posicao para as coordenadas
static Posicao* CriarPosicao(int x, int y) {
    Posicao* p = (Posicao*)malloc(sizeof(Posicao));
    if (p == NULL) return NULL;
    p->X = x;
    p->Y = y;
    p->Proximo = NULL;
    return p;
}

// Libera uma lista de Posicao para evitar vazamento de memória
static void LiberarPosicoes(Posicao* p) {
    while (p != NULL) {
        Posicao* prox = p->Proximo;
        free(p);
        p = prox;
    }
}

// Libera uma lista de Comando para evitar tambem vazamento de memória
static void LiberarComandos(Comando* c) {
    while (c != NULL) {
        Comando* prox = c->Proximo;
        free(c);
        c = prox;
    }
}

Caminho* InicializarCaminho(const char* Sequencia, int xInicial, int yInicial) {
    if (Sequencia == NULL) return NULL;

    // Valida todos os comandos antes de alocar qualquer coisa para que assim a gente evite alocar memória desnecessariamente se a sequência for inválida
    for (int i = 0; Sequencia[i] != '\0'; i++) {
        if (!ComandoValido(Sequencia[i])) return NULL;
    }

    Caminho* C = (Caminho*)malloc(sizeof(Caminho));
    if (C == NULL) return NULL;

    C->Inicio    = NULL;
    C->Fim       = NULL;
    C->Historico = NULL;
    C->Instrucoes = NULL;
    C->N         = 0;

    // Monta a lista de Instrucoes a partir da sequência de comandos fornecida
    Comando* ultimoCmd = NULL;
    for (int i = 0; Sequencia[i] != '\0'; i++) {
        Comando* cmd = (Comando*)malloc(sizeof(Comando));
        if (cmd == NULL) {
            LiberarComandos(C->Instrucoes);
            free(C);
            return NULL;
        }
        cmd->Direcao = Sequencia[i];
        cmd->Proximo = NULL;

        if (C->Instrucoes == NULL) {
            C->Instrucoes = cmd;
            ultimoCmd = cmd;
        } else {
            ultimoCmd->Proximo = cmd;
            ultimoCmd = cmd;
        }
        C->N++;
    }

    // Calcula posições e monta Historico, Inicio e Fim do caminho com base nas instruções
    int x = xInicial, y = yInicial;

    // Posição inicial
    Posicao* posInicio = CriarPosicao(x, y);
    if (posInicio == NULL) {
        LiberarComandos(C->Instrucoes);
        free(C);
        return NULL;
    }
    C->Inicio    = posInicio;
    C->Historico = posInicio;
    Posicao* ultimaPos = posInicio;

    // Percorre instruções
    Comando* cmd = C->Instrucoes;
    while (cmd != NULL) {
        if (cmd->Direcao == 'N') y++;
        else if (cmd->Direcao == 'S') y--;
        else if (cmd->Direcao == 'L') x++;
        else if (cmd->Direcao == 'O') x--;

        Posicao* novaPosicao = CriarPosicao(x, y);
        if (novaPosicao == NULL) {
            LiberarPosicoes(C->Historico);
            LiberarComandos(C->Instrucoes);
            free(C);
            return NULL;
        }
        ultimaPos->Proximo = novaPosicao;
        ultimaPos = novaPosicao;

        cmd = cmd->Proximo;
    }

    C->Fim = ultimaPos;

    return C;
}

void DestruirCaminho(Caminho* C) {
    if (C == NULL) return;
    LiberarPosicoes(C->Historico);
    LiberarComandos(C->Instrucoes);
    C->Inicio    = NULL;
    C->Fim       = NULL;
    C->Historico = NULL;
    C->Instrucoes = NULL;
    free(C);
}

Posicao* DeterminarFim(Caminho* C) {
    if (C == NULL) return NULL;
    return C->Fim;
}

Posicao* HistoricoPosicoes(Caminho* C) {
    if (C == NULL) return NULL;
    return C->Historico;
}

int CalcularDistanciaTotal(Caminho* C) {
    if (C == NULL) return -1;
    return C->N;
}

double CalcularDistanciaGeometrica(Caminho* C) {
    if (C == NULL || C->Inicio == NULL || C->Fim == NULL) return -1.0;
    double dx = (double)(C->Fim->X - C->Inicio->X);
    double dy = (double)(C->Fim->Y - C->Inicio->Y);
    return sqrt(dx * dx + dy * dy);
}

int CalcularDistanciaManhattan(Caminho* C) {
    if (C == NULL || C->Inicio == NULL || C->Fim == NULL) return -1;
    int dx = abs(C->Fim->X - C->Inicio->X);
    int dy = abs(C->Fim->Y - C->Inicio->Y);
    return dx + dy;
}

int ContarInstrucoes(Caminho* C) {
    if (C == NULL) return -1;
    return C->N;
}