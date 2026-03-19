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
// Funções de interface com o usuário:
void exibirMapa(const struct Territorio *tabuleiro);
int faseDeAtaque(struct Territorio *tabuleiro);
// Funções de lógica principal do jogo:
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);
// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {

    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    struct Territorio *tabuleiro;
    tabuleiro = calloc(NUM_TERRITORIOS, sizeof(struct Territorio));
    if(tabuleiro == NULL) {
        printf("Erro ao alocar memória para o tabuleiro. Encerrando o jogo.\n");
        return 1;
    }
    int totalTerritorios = 0;

    // Cadastro inicial dos territórios
    printf("======================================\n");
    printf(" WAR ESTRUTURADO - CADASTRO INICIAL\n");
    printf("======================================\n");

    for(totalTerritorios; totalTerritorios<NUM_TERRITORIOS; totalTerritorios++) {
        printf("\n--- Cadastrando Território %d ---\n", totalTerritorios+1);
        printf("Nome do território: ");
        fgets(tabuleiro[totalTerritorios].nome, TAM_STRING, stdin);
        printf("Cor do Exercito: ");
        fgets(tabuleiro[totalTerritorios].cor, TAM_STRING, stdin);

        tabuleiro[totalTerritorios].nome[strcspn(tabuleiro[totalTerritorios].nome, "\n")] = '\0';
        tabuleiro[totalTerritorios].cor[strcspn(tabuleiro[totalTerritorios].cor, "\n")] = '\0';

        printf("Número de Tropas: ");
        scanf("%d", &tabuleiro[totalTerritorios].num_tropas);
        limparBufferEntrada();
    }

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
    } while (faseDeAtaque(tabuleiro));

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    free(tabuleiro);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

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

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}