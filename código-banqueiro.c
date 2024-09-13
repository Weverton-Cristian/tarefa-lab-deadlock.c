#include <stdio.h>
#include <stdbool.h>

// Número de processos e recursos
#define NUM_PROCESSOS 2
#define NUM_RECURSOS 2

// Matrizes de alocação, máxima necessidade e recursos disponíveis
int alocado[NUM_PROCESSOS][NUM_RECURSOS] = { {0, 1}, {1, 0} };
int max[NUM_PROCESSOS][NUM_RECURSOS] = { {1, 1}, {1, 1} };
int disponivel[NUM_RECURSOS] = {1, 1};

// Função para verificar se o sistema está em um estado seguro
bool estadoSeguro() {
    int trabalho[NUM_RECURSOS];
    bool finalizado[NUM_PROCESSOS] = {false};

    // Copia os recursos disponíveis para o vetor de trabalho
    for (int i = 0; i < NUM_RECURSOS; i++) {
        trabalho[i] = disponivel[i];
    }

    while (true) {
        bool encontrado = false;
        for (int i = 0; i < NUM_PROCESSOS; i++) {
            if (!finalizado[i]) {
                bool podeCompletar = true;
                for (int j = 0; j < NUM_RECURSOS; j++) {
                    if (max[i][j] - alocado[i][j] > trabalho[j]) {
                        podeCompletar = false;
                        break;
                    }
                }
                if (podeCompletar) {
                    for (int j = 0; j < NUM_RECURSOS; j++) {
                        trabalho[j] += alocado[i][j];
                    }
                    finalizado[i] = true;
                    encontrado = true;
                }
            }
        }
        if (!encontrado) {
            break;
        }
    }

    for (int i = 0; i < NUM_PROCESSOS; i++) {
        if (!finalizado[i]) {
            return false;  // Sistema está em estado inseguro
        }
    }
    return true;
}
