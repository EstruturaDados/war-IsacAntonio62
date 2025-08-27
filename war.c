#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definição da Struct ---
// Define a estrutura 'Territorio' para agrupar dados relacionados a um território,
// como nome, cor do exército e número de tropas.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para exibir as informações de todos os territórios.
void mostrarTerritorios(Territorio* mapa, int numTerritorios) {
    printf("\n--- Estado Atual dos Territorios ---\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("  Nome: %s\n", (mapa + i)->nome);
        printf("  Cor: %s\n", (mapa + i)->cor);
        printf("  Tropas: %d\n\n", (mapa + i)->tropas);
    }
    printf("------------------------------------\n");
}

// Função para simular um ataque entre dois territórios.
// Recebe ponteiros para os territórios atacante e defensor.
void atacar(Territorio* atacante, Territorio* defensor) {
    // Rola um "dado" de 1 a 6 para cada lado.
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- Simulacao de Batalha ---\n");
    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
    printf("Dado do atacante (%s): %d\n", atacante->cor, dadoAtacante);
    printf("Dado do defensor (%s): %d\n", defensor->cor, dadoDefensor);

    // Compara os resultados dos dados para determinar o vencedor.
    if (dadoAtacante > dadoDefensor) {
        printf("O atacante (%s) venceu a batalha!\n", atacante->nome);
        
        // Se o atacante vencer, o defensor muda de cor.
        strcpy(defensor->cor, atacante->cor);
        
        // Transfere metade das tropas do atacante para o defensor.
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        
        printf("Territorio %s agora e controlado por %s.\n", defensor->nome, defensor->cor);
        printf("Tropas de %s: %d\n", defensor->nome, defensor->tropas);
        printf("Tropas de %s: %d\n", atacante->nome, atacante->tropas);

    } else {
        printf("O defensor (%s) venceu a batalha!\n", defensor->nome);
        
        // Se o atacante perder, ele perde uma tropa.
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("O atacante (%s) perdeu uma tropa. Tropas restantes: %d\n", atacante->nome, atacante->tropas);
        } else {
            printf("O atacante (%s) nao tem mais tropas para perder.\n", atacante->nome);
        }
    }
    printf("----------------------------\n");
}

int main() {
    // Inicializa o gerador de números aleatórios com a hora atual.
    srand(time(NULL));

    // --- Declaração do Vetor de Structs ---
    // Cria um array de 5 elementos do tipo 'Territorio' para armazenar os dados de cada um.
    const int numTerritorios = 5;
    Territorio territorios[numTerritorios];

    // --- Entrada de Dados ---
    printf("--- Cadastro de Territorios ---\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("\nCadastro do Territorio %d:\n", i + 1);

        printf("Nome do territorio: ");
        scanf(" %29[^\n]", territorios[i].nome);
        // Limpa o buffer de entrada.
        while (getchar() != '\n');

        printf("Cor do exercito: ");
        scanf(" %9[^\n]", territorios[i].cor);
        // Limpa o buffer de entrada.
        while (getchar() != '\n');

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
        // Limpa o buffer de entrada.
        while (getchar() != '\n');
    }
    
    int opcao;
    do {
        mostrarTerritorios(territorios, numTerritorios);
        printf("\nO que voce gostaria de fazer?\n");
        printf("1. Simular um ataque\n");
        printf("2. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        // Limpa o buffer de entrada para evitar erros.
        while (getchar() != '\n'); 

        if (opcao == 1) {
            int indiceAtacante, indiceDefensor;
            
            printf("\n--- Selecao de Territorios ---\n");
            printf("Escolha o numero do territorio atacante (1 a %d): ", numTerritorios);
            scanf("%d", &indiceAtacante);
            
            printf("Escolha o numero do territorio defensor (1 a %d): ", numTerritorios);
            scanf("%d", &indiceDefensor);
            
            printf("------------------------------\n");
            
            // Limpa o buffer de entrada.
            while (getchar() != '\n'); 

            // Valida a escolha do usuário.
            if (indiceAtacante < 1 || indiceAtacante > numTerritorios || 
                indiceDefensor < 1 || indiceDefensor > numTerritorios || 
                indiceAtacante == indiceDefensor) 
            {
                printf("Opcao invalida. Por favor, escolha territorios validos e diferentes.\n");
                continue;
            }

            // Converte os índices baseados em 1 para índices de array baseados em 0.
            Territorio* atacante = &territorios[indiceAtacante - 1];
            Territorio* defensor = &territorios[indiceDefensor - 1];

            // Valida se o jogador não está atacando seu próprio território.
            if (strcmp(atacante->cor, defensor->cor) == 0) {
                printf("Nao e possivel atacar um territorio da sua propria cor.\n");
            } else {
                // Chama a função de ataque.
                atacar(atacante, defensor);
            }
        } else if (opcao != 2) {
            printf("Opcao invalida. Por favor, tente novamente.\n");
        }
    } while (opcao != 2);

    return 0;
}
