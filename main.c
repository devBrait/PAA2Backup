/*
    Guilherme Teodoro de Oliveira RA: 10425362
    Luís Henrique Ribeiro Fernandes RA: 10420079
    Vinícius Brait Lorimier RA: 10420046
*/

#include <stdio.h>
#include <stdlib.h>

// Declarações das funções de backup.c
void printUsage();
void processBackup(FILE *input, FILE *output);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsage();
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Combinação não realizada. Arquivo de entrada não existe.\n");
        printUsage();
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (!output) {
        printf("Erro ao abrir o arquivo de saída.\n");
        fclose(input);
        return 1;
    }

    // Faz a chamada para ordenar e encontrar a melhor combinação possível
    processBackup(input, output);

    fclose(input);
    fclose(output);
    return 0;
}