#include <stdio.h>
#include <string.h>

// --- Definição da Struct ---
// Define a estrutura 'Territorio' para agrupar dados relacionados a um território,
// como nome, cor do exército e número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    // --- Declaração do Vetor de Structs ---
    // Cria um array de 5 elementos do tipo 'Territorio' para armazenar os dados de cada um.
    Territorio territorios[5];

    // --- Entrada de Dados ---
    // Usa um loop 'for' para iterar 5 vezes, solicitando e lendo os dados de cada território.
    printf("--- Cadastro de Territorios ---\n");
    for (int i = 0; i < 5; i++) {
        printf("\nCadastro do Territorio %d:\n", i + 1);

        printf("Nome do territorio: ");
        scanf(" %29[^\n]", territorios[i].nome);

        printf("Cor do exercito: ");
        scanf(" %9[^\n]", territorios[i].cor);

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // --- Exibição de Dados ---
    // Após o cadastro, outro loop 'for' percorre o vetor para exibir as informacoes de forma clara e formatada.
    printf("\n--- Territorios Cadastrados ---\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    return 0;
}