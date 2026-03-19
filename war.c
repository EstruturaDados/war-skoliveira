// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 5
#define TAM_STRING 100

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int num_tropas;
};

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
struct Territorio *alocarMapa();
void liberarMemoria(struct Territorio *tabuleiro);

// Funções de interface com o usuário:
void inicializarTerritorios(struct Territorio *tabuleiro);
void exibirMapa(const struct Territorio *tabuleiro);
int sortearMissao(struct Territorio *tabuleiro);
void exibirMissao(struct Territorio *tabuleiro, int missaoID);
void exibirMenuPrincipal();
int faseDeAtaque(struct Territorio *tabuleiro);

// Funções de lógica principal do jogo:
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);
int verificarVitoria(struct Territorio *tabuleiro);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    struct Territorio *tabuleiro = alocarMapa();
    if(tabuleiro == NULL) {
        printf("Erro ao alocar memória para o tabuleiro. Encerrando o jogo.\n");
        return 1;
    }

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(tabuleiro);
    exibirMapa(tabuleiro);

    // - Define a cor do jogador e sorteia sua missão secreta.
    int missaoID = sortearMissao(tabuleiro);
    exibirMissao(tabuleiro, missaoID);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
    int opcao;
    do {
        exibirMapa(tabuleiro);
        exibirMissao(tabuleiro, missaoID);
        exibirMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada(); // Limpa o buffer para evitar problemas com entradas subsequentes
        switch(opcao) {
            case 1:
                if(faseDeAtaque(tabuleiro) == 0) {
                    opcao = 0; // Sinaliza para sair se a fase de ataque retornar 0
                }
                break;
            case 2:
                if(verificarVitoria(tabuleiro)) {
                    printf("Parabéns! Você cumpriu sua missão e venceu o jogo!\n");
                    opcao = 0; // Sinaliza para sair após vitória
                } else {
                    printf("Missão ainda não cumprida. Continue jogando!\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("Opção inválida. Por favor, tente novamente.\n");
        }
        printf("\nPressione Enter para continuar...\n");
        getchar(); // Pausa para o jogador ler os resultados
    } while(opcao != 0);

    return 0;
}

// --- Implementação das Funções ---
// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
struct Territorio *alocarMapa() {
    struct Territorio *tabuleiro;
    tabuleiro = calloc(NUM_TERRITORIOS, sizeof(struct Territorio));
    
    if (tabuleiro == NULL) {
        printf("Erro ao alocar memória para o tabuleiro. Encerrando o jogo.\n");
        return NULL; // ← retorna NULL, não 1
    }
    
    return tabuleiro; // ← retorna o ponteiro alocado
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(struct Territorio *tabuleiro) {
    strcpy(tabuleiro[0].nome, "América");
    strcpy(tabuleiro[0].cor, "Verde");
    tabuleiro[0].num_tropas = 5;

    strcpy(tabuleiro[1].nome, "Europa");
    strcpy(tabuleiro[1].cor, "Azul");
    tabuleiro[1].num_tropas = 3;

    strcpy(tabuleiro[2].nome, "Asia");
    strcpy(tabuleiro[2].cor, "Vermelho");
    tabuleiro[2].num_tropas = 2;

    strcpy(tabuleiro[3].nome, "África");
    strcpy(tabuleiro[3].cor, "Amarelo");
    tabuleiro[3].num_tropas = 4;

    strcpy(tabuleiro[4].nome, "Oceania");
    strcpy(tabuleiro[4].cor, "Branco");
    tabuleiro[4].num_tropas = 1;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(struct Territorio *tabuleiro) {
    free(tabuleiro);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("--- MENU DE ACOES ---\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("0 - Sair\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const struct Territorio *tabuleiro) {
    printf("\n=======================================");
    printf("\n    MAPA DO MUNDO - ESTADO ATUAL");
    printf("\n=======================================\n");

    for(int i=0; i<NUM_TERRITORIOS; i++) {
        printf("%d. %s ", i+1, tabuleiro[i].nome);
        printf("(Exército: %s, ", tabuleiro[i].cor);
        printf("Tropas: %d)\n", tabuleiro[i].num_tropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(struct Territorio *tabuleiro, int missaoID) {
    int seuID = rand() % NUM_TERRITORIOS; // ID do jogador (exército) para personalizar a missão

    printf("\n--- SUA MISSAO (Exercito %s) ---\n", tabuleiro[seuID].cor); // Exibe a cor do exército do jogador
    switch(missaoID) {
        case 0:
            printf("Conquistar a %s.\n", tabuleiro[0].nome);
            break;
        case 1:
            printf("Destruir o exército %s.\n", tabuleiro[1].cor);
            break;
        case 2:
            printf("Conquistar 3 territórios.\n");
            break;
        case 3:
            printf("Destruir o exército %s.\n", tabuleiro[2].cor);
            break;
        case 4:
            printf("Conquistar a %s e a %s.\n", tabuleiro[3].nome, tabuleiro[4].nome);
            break;
        default:
            printf("Desconhecida.\n");
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
int faseDeAtaque(struct Territorio *tabuleiro) {
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha o território atacante (1-%d, 0 para sair): ", NUM_TERRITORIOS);
    
    int opcao;
    scanf("%d", &opcao);
    limparBufferEntrada();
    if (opcao == 0) {
        printf("Encerrando o jogo. Obrigado por jogar!\n");
        return 0; // sinaliza para sair
    }

    int atacanteIndex = opcao - 1;
    printf("Escolha o território defensor (1-%d, 0 para sair): ", NUM_TERRITORIOS);
    scanf("%d", &opcao);
    limparBufferEntrada();
    if (opcao == 0) {
        printf("Encerrando o jogo. Obrigado por jogar!\n");
        return 0; // sinaliza para sair
    }

    int defensorIndex = opcao - 1;
    if (atacanteIndex < 0 || atacanteIndex >= NUM_TERRITORIOS ||
        defensorIndex < 0 || defensorIndex >= NUM_TERRITORIOS) {
        printf("Índices inválidos. Por favor, tente novamente.\n");
        return 1; // sinaliza para continuar (equivalente ao continue)
    }

    simularAtaque(&tabuleiro[atacanteIndex], &tabuleiro[defensorIndex]);
    return 1; // sinaliza para continuar
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor) {
    // Validações iniciais
    if(atacante->num_tropas < 2) {
        printf("O território %s não tem tropas suficientes para atacar.\n", atacante->nome);
        return;
    }
    if(strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Não é possível atacar um território do mesmo exército.\n");
        return;
    }

    // Simulação de dados (1 a 6)
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    // Comparação dos dados
    if(dadoAtacante > dadoDefensor) {
        defensor->num_tropas--;
        printf("VITÓRIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        if(defensor->num_tropas == 0) {
            printf("%s foi conquistado por %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor);
            defensor->num_tropas = 1; // Move uma tropa para o território conquistado
            atacante->num_tropas--; // Remove a tropa que se moveu
        }
    } else {
        atacante->num_tropas--;
        printf("VITÓRIA DO DEFENSOR! O atacante perdeu 1 tropa.\n");
    }

    printf("\nPressione Enter para continuar para o próximo turno...\n");
    getchar(); // Pausa para o jogador ler o resultado
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao(struct Territorio *tabuleiro) {
    return rand() % NUM_TERRITORIOS; // Retorna um ID de missão aleatório
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(struct Territorio *tabuleiro) {
    return 0; // Retorna 0 por padrão, indicando que a missão ainda não foi cumprida
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}