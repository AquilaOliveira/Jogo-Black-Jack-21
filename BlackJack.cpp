#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define NUM_CARTAS 52
#define MAX_CARTAS_MAO 10
#define MAX_JOGADORES 12
#define MAX_HISTORICO 100

int mostrar_menu() {
    int opcao;
    system("cls");
	printf("\n                    JOGO VINTE E UM ou BLACKJACK                  \n");
    printf("\n +---------+       +---------+        +---------+       +---------+");
    printf("\n | C       |       | O       |        | P       |       | E       |");
    printf("\n |         |       |         |        |         |       |         |");
    printf("\n |         |       |         |        |         |       |         |");
    printf("\n |         |       |         |        |         |       |         |");	
    printf("\n |       C |       |       O |        |       P |       |       E |");
    printf("\n +---------+       +---------+        +---------+       +---------+\n");
    printf("\n==================================================================\n\n");
    printf("     Escolha uma opcao:    \n");
    printf("1. Jogar Single Player     \n");
    printf("2. Jogar Multiplayer       \n");
    printf("3. Regras                  \n");
    printf("4. Creditos                \n");
    printf("5. Historico de Jogos      \n");
    printf("6. Sair                    \n");
    scanf("%d", &opcao);
    return opcao;
}

int regras(){
	system("cls");
	printf("\n                    JOGO VINTE E UM ou BLACKJACK                  \n");
	printf("\n1. OBJETIVO DO JOGO?");
	printf("\n---> Ter uma mao de cartas cuja soma de pontos seja mais proxima de 21, sem ultrapassa-lo.");
	printf("\n---> Se ultrapassar 21 pontos, o jogador ou dealer perde imediatamente ('estoura').\n");
	printf("\n2. PONTUACAO DAS CARTAS:");
	printf("\n---> Cartas numeradas (2 a 10) valem seu numero correspondente.");
	printf("\n---> Valete (J), Dama (Q), e Rei (K) valem 10 pontos cada.");
	printf("\n---> As pode valer 1 ou 11 pontos, dependendo do que for mais vantajoso para a mao.\n");
	
}



// Estrutura para armazenar informações de um jogo
typedef struct {
    char nome[MAX_JOGADORES][50];
    int pontos[MAX_JOGADORES];
    int num_jogadores;
} Historico;

// Estrutura para representar uma carta
typedef struct {
    char valor[10];
    char naipe[10];
    int pontos;
} Carta;

// Estrutura para armazenar informações de um jogador
typedef struct {
    char nome[50];
    Carta mao[MAX_CARTAS_MAO];
    int num_cartas;
    int pontos;
} Jogador;

void inicializar_baralho(Carta baralho[]) {
    const char *naipe[] = {"C", "O", "E", "P"};
    const char *valor[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    
    int index = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            strcpy(baralho[index].naipe, naipe[i]);
            strcpy(baralho[index].valor, valor[j]);
            if (j >= 10) {
                baralho[index].pontos = 10;
            } else if (j == 12) {
                baralho[index].pontos = 1;
            } else {
                baralho[index].pontos = j + 2;
            }
            index++;
        }
    }
}

void embaralhar_baralho(Carta baralho[]) {
    srand(time(NULL));  // Semente para números aleatórios
    for (int i = 0; i < NUM_CARTAS; i++) {
        int j = rand() % NUM_CARTAS;
        Carta temp = baralho[i];
        baralho[i] = baralho[j];
        baralho[j] = temp;
    }
}

// Função para calcular os pontos de uma mão
int somar_pontos(Carta mao[], int num_cartas) {
    int pontos = 0;
    int ases = 0;  // Contador de ases

    // Soma os pontos das cartas e conta os ases
    for (int i = 0; i < num_cartas; i++) {
        pontos += mao[i].pontos;  // Soma os pontos da carta ao total
        if (strcmp(mao[i].valor, "Ás") == 0) {
            ases++;
        }
    }

    // Ajusta os ases: tenta usá-los como 11 sem estourar 21
    while (pontos <= 11 && ases > 0) {
        pontos += 10;  // Converte o Ás de 1 para 11
        ases--;
    }

    return pontos;
}


// Função para desenhar a carta na tela com caracteres ASCII
void desenhar_carta(Carta carta) {
    printf("+-------+\n");
    printf("|%s      |\n", carta.valor);
    printf("|   %s   |\n", carta.naipe);
    printf("|      %s|\n", carta.valor);
    printf("+-------+\n");
}

void mostrar_mao(Carta mao[], int num_cartas) {
    for (int i = 0; i < num_cartas; i++) {
        desenhar_carta(mao[i]);
    }
}



int continuar_ou_sair() {
    int opcao;
    printf("\nEscolha uma opcao:\n");
    printf("1. Voltar ao menu\n");
    printf("2. Sair do jogo\n");
    scanf("%d", &opcao);
    return opcao;
}

void inicializar_historico(Historico* historico, int* total_jogos) {
    for (int i = 0; i < MAX_HISTORICO; i++) {
        historico[i].num_jogadores = 0;
    }
    *total_jogos = 0; // Contador de jogos salvos
}

// Função para salvar um jogo no histórico
void salvar_historico(Historico* historico, int* total_jogos, char nomes[][50], int* pontos, int num_jogadores) {
    if (*total_jogos < MAX_HISTORICO) {
        Historico* jogo_atual = &historico[*total_jogos];
        for (int i = 0; i < num_jogadores; i++) {
            strcpy(jogo_atual->nome[i], nomes[i]);
            jogo_atual->pontos[i] = pontos[i];
        }
        jogo_atual->num_jogadores = num_jogadores;
        (*total_jogos)++;
    } else {
        printf("\nO historico esta cheio! Nao e possivel salvar mais jogos.\n");
    }
}

// Função principal de jogar o jogo de Blackjack (Single Player)
void jogar_single_player(Historico* historico, int* total_jogos) {
    char resposta;
    Carta baralho[NUM_CARTAS];
    Carta mao_jogador[MAX_CARTAS_MAO], mao_dealer[MAX_CARTAS_MAO];
    int num_cartas_jogador = 0, num_cartas_dealer = 0;
    int pontos_jogador, pontos_dealer;
    int posicao_baralho = 0;  // Controla a posição no baralho
    char acao;

    system("cls");
    printf("\nEsta preparado? Posso embaralhar as cartas? (S para sim/ N para nao)\n");
    scanf(" %c", &resposta);

    if (resposta == 'S' || resposta == 's') {
        inicializar_baralho(baralho);
        embaralhar_baralho(baralho);

        // Distribuir 2 cartas para cada jogador
        mao_jogador[num_cartas_jogador++] = baralho[posicao_baralho++];
        mao_jogador[num_cartas_jogador++] = baralho[posicao_baralho++];
        mao_dealer[num_cartas_dealer++] = baralho[posicao_baralho++];
        mao_dealer[num_cartas_dealer++] = baralho[posicao_baralho++];

        printf("\nSuas cartas:\n");
        mostrar_mao(mao_jogador, num_cartas_jogador);
        pontos_jogador = somar_pontos(mao_jogador, num_cartas_jogador);
        printf("Pontos do jogador: %d\n", pontos_jogador);

        // Exibe a carta visível do dealer
        printf("\nCarta visivel do dealer:\n");
        desenhar_carta(mao_dealer[0]);

        // Turno do jogador
        while (1) {
            printf("\nDeseja (P)pedir mais carta ou (S)parar?\n");
            scanf(" %c", &acao);

            if (acao == 'P' || acao == 'p') {
                mao_jogador[num_cartas_jogador++] = baralho[posicao_baralho++];
                printf("\nSuas novas cartas:\n");
                mostrar_mao(mao_jogador, num_cartas_jogador);

                pontos_jogador = somar_pontos(mao_jogador, num_cartas_jogador);
                printf("Pontos do jogador: %d\n", pontos_jogador);

                if (pontos_jogador > 21) {
                    printf("\nVoce estourou! Dealer ganha.\n");
                    return;
                }
            } else if (acao == 'S' || acao == 's') {
                break;
            } else {
                printf("\nOpcao invalida. Por favor, escolha (P)pedir ou (S)parar.\n");
            }
        }

        // Turno do dealer (jogo básico; dealer compra até atingir 17 ou mais)
        while (somar_pontos(mao_dealer, num_cartas_dealer) < 17) {
            mao_dealer[num_cartas_dealer++] = baralho[posicao_baralho++];
        }

        // Calcula pontos finais
        pontos_jogador = somar_pontos(mao_jogador, num_cartas_jogador);
        pontos_dealer = somar_pontos(mao_dealer, num_cartas_dealer);

        // Exibe o resultado
        printf("\nPontos do jogador: %d\n", pontos_jogador);
        printf("Pontos do dealer: %d\n", pontos_dealer);

        if (pontos_jogador > 21 || (pontos_dealer <= 21 && pontos_dealer > pontos_jogador)) {
            printf("\nDealer ganha!\n");
        } else if (pontos_dealer > 21 || pontos_jogador > pontos_dealer) {
            printf("\nJogador ganha!\n");
        } else {
            printf("\nEmpate!\n");
        }

        // Salvar no histórico
        char nomes[2][50] = {"Jogador", "Dealer"};
        int pontos[2] = {pontos_jogador, pontos_dealer};
        salvar_historico(historico, total_jogos, nomes, pontos, 2);
    } else {
        printf("\nJogo cancelado.\n");
    }
}

void jogar_multiplayer(Historico* historico, int* total_jogos) {
    Carta baralho[NUM_CARTAS];
    Jogador jogadores[MAX_JOGADORES];
    int num_jogadores;
    int posicao_baralho = 0; // Controla a posição atual do baralho
    char acao;

    system("cls");
    printf("\nQuantos jogadores vao participar (2 a 12)? ");
    scanf("%d", &num_jogadores);

    if (num_jogadores < 2 || num_jogadores > MAX_JOGADORES) {
        printf("Numero invalido de jogadores.\n");
        return;
    }

    // Coleta os nomes dos jogadores
    char nomes[MAX_JOGADORES][50];
    int pontos[MAX_JOGADORES];
    for (int i = 0; i < num_jogadores; i++) {
        printf("Digite o nome do jogador %d: ", i + 1);
        scanf(" %49[^\n]", jogadores[i].nome);
        jogadores[i].num_cartas = 0;
        jogadores[i].pontos = 0;
        strcpy(nomes[i], jogadores[i].nome);
    }

    // Inicializa e embaralha o baralho
    inicializar_baralho(baralho);
    embaralhar_baralho(baralho);

    // Distribui 2 cartas para cada jogador
    for (int i = 0; i < num_jogadores; i++) {
        jogadores[i].mao[jogadores[i].num_cartas++] = baralho[posicao_baralho++];
        jogadores[i].mao[jogadores[i].num_cartas++] = baralho[posicao_baralho++];
        jogadores[i].pontos = somar_pontos(jogadores[i].mao, jogadores[i].num_cartas);
        pontos[i] = jogadores[i].pontos;  // Salva os pontos para o histórico
    }

    // Cada jogador joga seu turno
    for (int i = 0; i < num_jogadores; i++) {
        printf("\nTurno de %s:\n", jogadores[i].nome);
        mostrar_mao(jogadores[i].mao, jogadores[i].num_cartas);
        printf("Pontos: %d\n", jogadores[i].pontos);

        while (1) {
            printf("Deseja (P)pedir mais carta ou (S)parar?\n");
            scanf(" %c", &acao);

            if (acao == 'P' || acao == 'p') {
                // Dá uma nova carta ao jogador
                jogadores[i].mao[jogadores[i].num_cartas++] = baralho[posicao_baralho++];
                jogadores[i].pontos = somar_pontos(jogadores[i].mao, jogadores[i].num_cartas);

                // Mostra as cartas e os pontos atualizados
                mostrar_mao(jogadores[i].mao, jogadores[i].num_cartas);
                printf("Pontos: %d\n", jogadores[i].pontos);

                if (jogadores[i].pontos > 21) {
                    printf("%s estourou! Voce perdeu o jogo.\n", jogadores[i].nome);
                    break;
                }
            } else if (acao == 'S' || acao == 's') {
                break;
            } else {
                printf("Opcao invalida! Escolha (P) ou (S).\n");
            }
        }
        pontos[i] = jogadores[i].pontos;  // Atualiza os pontos para o histórico
    }

    // Determina o vencedor
    int maior_pontos = 0;
    int vencedor = -1;
    for (int i = 0; i < num_jogadores; i++) {
        if (jogadores[i].pontos <= 21 && jogadores[i].pontos > maior_pontos) {
            maior_pontos = jogadores[i].pontos;
            vencedor = i;
        }
    }

    // Exibe o resultado
    if (vencedor == -1) {
        printf("\nTodos os jogadores estouraram! Nao ha vencedor.\n");
    } else {
        printf("\nO vencedor e %s com %d pontos!\n", jogadores[vencedor].nome, maior_pontos);
    }

    // Salva o jogo no histórico
    salvar_historico(historico, total_jogos, nomes, pontos, num_jogadores);
}





void exibir_historico(Historico* historico) {
    system("cls");
    printf("===== HISTORICO DE JOGOS =====\n");
    for (int i = 0; i < MAX_HISTORICO; i++) {
        if (historico[i].num_jogadores == 0) {
            break;
        }
        printf("\nJogo %d:\n", i + 1);
        for (int j = 0; j < historico[i].num_jogadores; j++) {
            printf("Jogador: %s - Pontos: %d\n", historico[i].nome[j], historico[i].pontos[j]);
        }
    }
}

int creditos(){
	system("cls");
    printf("\nDesenvolvido por Alunos do Instituto Federal de Sao Paulo");
    printf("\n------------IFSP - Campus Braganca Paulista--------------\n");
    printf("\n-->Aquila Matheus de Oliveira");
    printf("\n-->Bianca de Aguiar Celestino");
	printf("\n-->Douglas Gustavo da Silva");
	printf("\n-->Eduarda Caroline de Almeida Monezzi\n");
    printf("\n      Disciplina BRAPROG - Introducao a Programacao      ");
	printf("\n         Professor Orientador: Sergio Francisco          \n");
}



int main() {
    int opcao;
    int opcao_continuar;
    Historico historico[MAX_HISTORICO];
    int total_jogos;

    inicializar_historico(historico, &total_jogos);

    while (1) {
        opcao = mostrar_menu();

        switch(opcao) {
            case 1:
                jogar_single_player(historico, &total_jogos);
                break;
            case 2:
                jogar_multiplayer(historico, &total_jogos);
                break;
            case 3:
                regras();
                break;
            case 4:
                creditos();
                break;
            case 5:
                exibir_historico(historico);
                break;
            case 6:
                printf("\nSaindo... Ate logo!\n");
                return 0;
            default:
                system("cls");
                printf("\nOpcao invalida! Por favor, escolha uma opcao valida.\n");
                continue;
        }

        // Opção de continuar ou sair
        opcao_continuar = continuar_ou_sair();
        if (opcao_continuar == 2) {
            break;  // Sai do jogo
        }
    }

    return 0;
}
