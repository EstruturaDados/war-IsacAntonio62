#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Definição das Constantes ---
// Usando #define para que o pré-processador substitua os valores.
// Aumentamos os tamanhos para evitar os warnings.
#define TAM_NOME 50
#define TAM_COR 20

// --- Definição das Structs ---
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

typedef struct {
    char* descricao;
    char tipoMissao[TAM_NOME];
    char alvo[TAM_COR];
    int quantidade;
} Missao;

typedef struct {
    char cor[TAM_COR];
    Missao* missao; 
} Jogador;

// --- Protótipos das Funções ---
void inicializarJogo(Territorio** mapa, Jogador** jogadores, int numTerritorios, int numJogadores);
void configurarMapaInicial(Territorio* mapa, int numTerritorios);
void atribuirMissoes(Jogador* jogadores, int numJogadores, Missao missoes[], int totalMissoes);
void exibirMenu(const Jogador* jogador, const Territorio* mapa, int numTerritorios);
void realizarAcao(int opcao, Jogador* jogador, Territorio* mapa, int numTerritorios);
void atacar(Territorio* atacante, Territorio* defensor);
int verificarMissao(const Jogador* jogador, const Territorio* mapa, int numTerritorios);
void liberarRecursos(Territorio* mapa, Jogador* jogadores, int numJogadores);
int contarTerritoriosPorCor(const Territorio* mapa, int numTerritorios, const char* cor);

// --- Vetor de Missões Estratégicas ---
Missao missoes[] = {
    {"Conquistar 18 territorios.", "CONQUISTAR_QUANTIDADE", "Nenhum", 18},
    {"Conquistar 24 territorios.", "CONQUISTAR_QUANTIDADE", "Nenhum", 24},
    {"Eliminar todas as tropas da cor Vermelha.", "ELIMINAR_COR", "Vermelha", 0},
    {"Eliminar todas as tropas da cor Azul.", "ELIMINAR_COR", "Azul", 0},
    {"Eliminar todas as tropas da cor Verde.", "ELIMINAR_COR", "Verde", 0}
};
const int TOTAL_MISSOES = 5;

// ===================================== Funções do Jogo =====================================

/**
 * @brief Inicializa a memória para os territórios e jogadores.
 * @param mapa Ponteiro para o ponteiro do mapa de territórios.
 * @param jogadores Ponteiro para o ponteiro do vetor de jogadores.
 * @param numTerritorios Número total de territórios no mapa.
 * @param numJogadores Número total de jogadores.
 */
void inicializarJogo(Territorio** mapa, Jogador** jogadores, int numTerritorios, int numJogadores) {
    *mapa = (Territorio*)calloc(numTerritorios, sizeof(Territorio));
    if (*mapa == NULL) {
        printf("Erro ao alocar memoria para os territorios.\n");
        exit(1);
    }

    *jogadores = (Jogador*)calloc(numJogadores, sizeof(Jogador));
    if (*jogadores == NULL) {
        printf("Erro ao alocar memoria para os jogadores.\n");
        free(*mapa);
        exit(1);
    }

    for (int i = 0; i < numJogadores; i++) {
        (*jogadores)[i].missao = (Missao*)malloc(sizeof(Missao));
        if ((*jogadores)[i].missao == NULL) {
            printf("Erro ao alocar memoria para a missao do jogador %d.\n", i + 1);
            liberarRecursos(*mapa, *jogadores, i);
            exit(1);
        }
    }
}

/**
 * @brief Configura o mapa inicial com nomes, cores e tropas.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param numTerritorios Número de territórios a serem configurados.
 */
void configurarMapaInicial(Territorio* mapa, int numTerritorios) {
    if (numTerritorios < 5) {
        printf("Erro: Numero insuficiente de territorios para o mapa padrao.\n");
        exit(1);
    }
    
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "EUA");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 5;

    strcpy(mapa[2].nome, "Africa");
    strcpy(mapa[2].cor, "Vermelha");
    mapa[2].tropas = 5;

    strcpy(mapa[3].nome, "Paris");
    strcpy(mapa[3].cor, "Verde");
    mapa[3].tropas = 5;

    strcpy(mapa[4].nome, "Alemanha");
    strcpy(mapa[4].cor, "Azul");
    mapa[4].tropas = 5;
    
    for (int i = 5; i < numTerritorios; i++) {
        sprintf(mapa[i].nome, "Territorio %d", i + 1);
        strcpy(mapa[i].cor, "Neutro");
        mapa[i].tropas = 2;
    }
}

/**
 * @brief Atribui missões aleatoriamente a cada jogador.
 * @param jogadores Vetor de jogadores.
 * @param numJogadores Número de jogadores.
 * @param missoes Vetor de missões disponíveis.
 * @param totalMissoes Número total de missões.
 */
void atribuirMissoes(Jogador* jogadores, int numJogadores, Missao missoes[], int totalMissoes) {
    printf("\n--- ATRIBUINDO MISSOES SECRETAS ---\n");
    for (int i = 0; i < numJogadores; i++) {
        int indice = rand() % totalMissoes;
        *jogadores[i].missao = missoes[indice];
        printf("Jogador com a cor %s recebeu sua missao.\n", jogadores[i].cor);
    }
}

/**
 * @brief Exibe o menu principal e o estado atual do jogo.
 * @param jogador Jogador atual (para exibir a missão).
 * @param mapa Mapa de territórios.
 * @param numTerritorios Número de territórios.
 */
void exibirMenu(const Jogador* jogador, const Territorio* mapa, int numTerritorios) {
    printf("\n======== MAPA DO MUNDO ========\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("%d. %s (Exercito: %s, Tropas: %d)\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("===============================\n");
    
    printf("\n--- SUA MISSAO (Exercito %s) ---\n", jogador->cor);
    printf("%s\n", jogador->missao->descricao);
    printf("----------------------------------\n");

    printf("\n--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
    printf("Escolha sua acao: ");
}

/**
 * @brief Executa a ação escolhida pelo jogador.
 * @param opcao Opção de menu escolhida (1, 2 ou 0).
 * @param jogador Jogador atual.
 * @param mapa Mapa de territórios.
 * @param numTerritorios Número de territórios.
 */
void realizarAcao(int opcao, Jogador* jogador, Territorio* mapa, int numTerritorios) {
    if (opcao == 1) {
        int indiceAtacante, indiceDefensor;
        
        printf("\n--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d): ", numTerritorios);
        scanf("%d", &indiceAtacante);
        
        printf("Escolha o territorio defensor (1 a %d): ", numTerritorios);
        scanf("%d", &indiceDefensor);
        
        while (getchar() != '\n'); // Limpa o buffer de entrada

        if (indiceAtacante < 1 || indiceAtacante > numTerritorios || 
            indiceDefensor < 1 || indiceDefensor > numTerritorios || 
            indiceAtacante == indiceDefensor) 
        {
            printf("Opcao invalida. Por favor, escolha territorios validos e diferentes.\n");
            return;
        }

        Territorio* atacante = &mapa[indiceAtacante - 1];
        Territorio* defensor = &mapa[indiceDefensor - 1];

        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Nao e possivel atacar um territorio da sua propria cor.\n");
        } else {
            atacar(atacante, defensor);
        }
    } else if (opcao == 2) {
        if (verificarMissao(jogador, mapa, numTerritorios)) {
            printf("\n==================================\n");
            printf("PARABENS! Voce venceu o jogo!\n");
            printf("Sua missao era: %s\n", jogador->missao->descricao);
            printf("==================================\n");
            // A lógica de saída é tratada no main
        } else {
            printf("\nMissao nao cumprida ainda. Continue jogando!\n");
        }
    } else if (opcao != 0) {
        printf("Opcao invalida. Por favor, tente novamente.\n");
    }
}

/**
 * @brief Simula uma batalha entre dois territórios.
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    // ... (lógica de batalha, inalterada)
    printf("\n--- SIMULACAO DE BATALHA ---\n");
    printf("%s (%s, %d tropas) ataca %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);
    printf("Dado do atacante (%s): %d\n", atacante->cor, dadoAtacante);
    printf("Dado do defensor (%s): %d\n", defensor->cor, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("O atacante (%s) venceu a batalha!\n", atacante->nome);
        
        strcpy(defensor->cor, atacante->cor);
        
        int tropasTransferidas = atacante->tropas / 2;
        defensor->tropas = tropasTransferidas;
        atacante->tropas -= tropasTransferidas;
        
        printf("Territorio %s agora e controlado por %s.\n", defensor->nome, defensor->cor);
        printf("Tropas de %s: %d\n", defensor->nome, defensor->tropas);
        printf("Tropas de %s: %d\n", atacante->nome, atacante->tropas);
    } else {
        printf("O defensor (%s) venceu a batalha!\n", defensor->nome);
        
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("O atacante (%s) perdeu uma tropa. Tropas restantes: %d\n", atacante->nome, atacante->tropas);
        } else {
            printf("O atacante (%s) nao tem mais tropas para perder.\n", atacante->nome);
        }
    }
    printf("----------------------------\n");
}

/**
 * @brief Verifica se a missão de um jogador foi cumprida.
 * @param jogador O jogador cuja missão será verificada.
 * @param mapa Mapa de territórios.
 * @param numTerritorios Número de territórios.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const Jogador* jogador, const Territorio* mapa, int numTerritorios) {
    if (strcmp(jogador->missao->tipoMissao, "CONQUISTAR_QUANTIDADE") == 0) {
        int territoriosDoJogador = contarTerritoriosPorCor(mapa, numTerritorios, jogador->cor);
        return (territoriosDoJogador >= jogador->missao->quantidade);
    }

    if (strcmp(jogador->missao->tipoMissao, "ELIMINAR_COR") == 0) {
        if (strcmp(jogador->missao->alvo, jogador->cor) == 0) return 0; // Não pode ser a própria cor

        int oponenteRestante = contarTerritoriosPorCor(mapa, numTerritorios, jogador->missao->alvo);
        return (oponenteRestante == 0);
    }
    return 0;
}

/**
 * @brief Conta o número de territórios de uma cor específica.
 * @param mapa Mapa de territórios.
 * @param numTerritorios Número de territórios.
 * @param cor A cor a ser contada.
 * @return O número de territórios com a cor especificada.
 */
int contarTerritoriosPorCor(const Territorio* mapa, int numTerritorios, const char* cor) {
    int count = 0;
    for (int i = 0; i < numTerritorios; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Libera toda a memória alocada dinamicamente.
 * @param mapa Ponteiro para o mapa.
 * @param jogadores Ponteiro para o vetor de jogadores.
 * @param numJogadores Número de jogadores.
 */
void liberarRecursos(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    free(mapa);
    for (int i = 0; i < numJogadores; i++) {
        free(jogadores[i].missao); 
    }
    free(jogadores);
    printf("\nMemoria alocada liberada com sucesso!\n");
}

// ===================================== Função Principal =====================================

int main() {
    srand(time(NULL));
    const int NUM_TERRITORIOS = 5;
    const int NUM_JOGADORES = 2;
    
    Territorio* territorios = NULL;
    Jogador* jogadores = NULL;

    inicializarJogo(&territorios, &jogadores, NUM_TERRITORIOS, NUM_JOGADORES);
    configurarMapaInicial(territorios, NUM_TERRITORIOS);
    
    // Atribuição de cores para os jogadores, correspondente ao mapa inicial.
    strcpy(jogadores[0].cor, "Azul"); 
    strcpy(jogadores[1].cor, "Verde");

    atribuirMissoes(jogadores, NUM_JOGADORES, missoes, TOTAL_MISSOES);
    
    int opcao = -1;
    
    do {
        exibirMenu(&jogadores[0], territorios, NUM_TERRITORIOS);
        scanf("%d", &opcao);
        while (getchar() != '\n');

        realizarAcao(opcao, &jogadores[0], territorios, NUM_TERRITORIOS);
        
        // Verificação final após cada ação
        if (verificarMissao(&jogadores[0], territorios, NUM_TERRITORIOS)) {
            printf("\n==================================\n");
            printf("PARABENS! Voce venceu o jogo!\n");
            printf("Sua missao era: %s\n", jogadores[0].missao->descricao);
            printf("==================================\n");
            opcao = 0; // Encerra o jogo
        }
        
    } while (opcao != 0);

    liberarRecursos(territorios, jogadores, NUM_JOGADORES);

    return 0;
}