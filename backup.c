/*
    Guilherme Teodoro de Oliveira RA: 10425362
    Luís Henrique Ribeiro Fernandes RA: 10420079
    Vinícius Brait Lorimier RA: 10420046
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 50

// Variáveis globais
int bestSizeA, bestSizeB, bestUsedA, bestUsedB;
int bestPendriveA[MAX_FILES], bestPendriveB[MAX_FILES];
int foundSolution = 0;

void printUsage() {
    printf("Uso correto: ./backup <ARQUIVO_ENTRADA> <ARQUIVO_SAIDA>\n");
}

// Função para avaliar a qualidade de uma distribuição
int evaluateDistribution(int sizeA, int usedA, int sizeB, int usedB) {
    // Se um pendrive tem só um arquivo e o outro tem vários, priorizamos essa distribuição
    if ((sizeA == 1 && sizeB > 1) || (sizeB == 1 && sizeA > 1)) {
        return 1000;  // Alta pontuação para distribuições com um arquivo grande isolado
    }
    
    // Priorizamos distribuições com equilíbrio de espaço usado
    int difference = abs(usedA - usedB);
    return 500 - difference;  // Quanto menor a diferença, melhor a pontuação
}

void saveBestSolution(int *pendriveA, int sizeA, int usedA, int *pendriveB, int sizeB, int usedB) {
    // Se ainda não temos solução, salvamos esta
    if (!foundSolution) {
        bestSizeA = sizeA;
        bestSizeB = sizeB;
        bestUsedA = usedA;
        bestUsedB = usedB;
        memcpy(bestPendriveA, pendriveA, sizeA * sizeof(int));
        memcpy(bestPendriveB, pendriveB, sizeB * sizeof(int));
        foundSolution = true;
        return;
    }
    
    // Avaliar a qualidade da solução atual e da melhor solução anterior
    int currentScore = evaluateDistribution(sizeA, usedA, sizeB, usedB);
    int lastScore = evaluateDistribution(bestSizeA, bestUsedA, bestSizeB, bestUsedB);
    
    // Se a solução atual é melhor, salvamos
    if (currentScore > lastScore) {
        bestSizeA = sizeA;
        bestSizeB = sizeB;
        bestUsedA = usedA;
        bestUsedB = usedB;
        memcpy(bestPendriveA, pendriveA, sizeA * sizeof(int));
        memcpy(bestPendriveB, pendriveB, sizeB * sizeof(int));
    }
}

void backtracking(int *files, int t, int index, int usedA, int usedB, int capacity, int *pendriveA, int sizeA, int *pendriveB, int sizeB, int totalSize) {
    // Se todos os arquivos foram considerados
    if (index == t) {
        // Verifica se todos os arquivos foram alocados
        if (usedA <= capacity && usedB <= capacity && usedA + usedB == totalSize) {
            saveBestSolution(pendriveA, sizeA, usedA, pendriveB, sizeB, usedB);
        }
        return;
    }

    // Primeiro tentamos colocar arquivos grandes no pendrive B
    if (index == 0 && files[index] > capacity / 2) {
        pendriveB[sizeB] = files[index];
        backtracking(files, t, index + 1, usedA, usedB + files[index], capacity, pendriveA, sizeA, pendriveB, sizeB + 1, totalSize);
        return;
    }

    // Para outros casos, tentamos primeiro o pendrive A (para arquivos menores)
    if (usedA + files[index] <= capacity) {
        pendriveA[sizeA] = files[index];
        backtracking(files, t, index + 1, usedA + files[index], usedB, capacity, pendriveA, sizeA + 1, pendriveB, sizeB, totalSize);
    }

    // Depois tentamos o pendrive B
    if (usedB + files[index] <= capacity) {
        pendriveB[sizeB] = files[index];
        backtracking(files, t, index + 1, usedA, usedB + files[index], capacity, pendriveA, sizeA, pendriveB, sizeB + 1, totalSize);
    }
}

void processBackup(FILE *input, FILE *output) {
    int N;
    fscanf(input, "%d", &N);

    for (int test = 0; test < N; test++) {
        int L, T;
        fscanf(input, "%d %d", &L, &T);

        int files[MAX_FILES];
        int totalSize = 0;
        for (int i = 0; i < T; i++) {
            fscanf(input, "%d", &files[i]);
            totalSize += files[i];
        }

        int pendriveA[MAX_FILES], pendriveB[MAX_FILES];
        bestSizeA = bestSizeB = bestUsedA = bestUsedB = 0;
        foundSolution = false;
        int capacity = L / 2;

        fprintf(output, "%d GB\n", L);
        printf("%d GB\n", L);

        // Verificação rápida se é possível encaixar todos os arquivos
        if (totalSize > L || files[0] > capacity) {
            fprintf(output, "Impossível gravar todos os arquivos nos pendrives.\n");
            printf("Impossivel gravar todos os arquivos nos pendrives.\n");
        } 
        else {
            backtracking(files, T, 0, 0, 0, capacity, pendriveA, 0, pendriveB, 0, totalSize);

            if (foundSolution) {
                fprintf(output, "Pendrive A (%d GB)\n", capacity);
                printf("Pendrive A (%d GB)\n", capacity);
                
                for (int i = 0; i < bestSizeA; i++) {
                    fprintf(output, "%d GB\n", bestPendriveA[i]); 
                    printf("%d GB\n", bestPendriveA[i]);
                }
                fprintf(output, "\nPendrive B (%d GB)\n", capacity);
                printf("\nPendrive B (%d GB)\n", capacity);

                for (int i = 0; i < bestSizeB; i++) {
                    fprintf(output, "%d GB\n", bestPendriveB[i]);
                    printf("%d GB\n", bestPendriveB[i]);
                }
            } else {
                fprintf(output, "Impossível gravar todos os arquivos nos pendrives.\n");
                printf("Impossivel gravar todos os arquivos nos pendrives.\n");
            }
        }
        
        if (test < N - 1) {
            fprintf(output, "\n");
            printf("\n");
        }
    }
}