#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define MAX_TENTATIVAS 6
#define NUM_PALAVRAS 10

// Estrutura para armazenar palavras e dicas
typedef struct {
    char palavra[20];
    char dica[100];
} PalavraDica;

void exibirPalavra(char palavra[], int acertos[]);
int verificarLetra(char letra, char palavra[], int acertos[]);
int jogarNovamente();

int main() {
    PalavraDica listaPalavras[NUM_PALAVRAS] = {
        {"PROGRAMACAO", "Arte de escrever códigos."},
        {"ELEFANTE", "O maior animal terrestre."},
        {"BICICLETA", "Meio de transporte com duas rodas."},
        {"ABACAXI", "Fruta tropical com casca espinhosa."},
        {"ASTRONOMIA", "Estudo dos astros e do universo."},
        {"GUITARRA", "Instrumento musical de cordas."},
        {"VULCAO", "Estrutura geológica que expele lava."},
        {"ESMERALDA", "Uma pedra preciosa de cor verde."},
        {"DINOSSAURO", "Réptil pré-histórico gigante."},
        {"PARALELEPIPEDO", "Um sólido geométrico de seis faces."}
    };

    int jogar = 1; // Variável para controlar se o jogador quer continuar

    while (jogar) {
        srand(time(NULL));
        int indice = rand() % NUM_PALAVRAS; // Escolhe uma palavra aleatória
        char palavraSecreta[20];
        strcpy(palavraSecreta, listaPalavras[indice].palavra);

        int tamanho = strlen(palavraSecreta);
        int acertos[tamanho];
        int tentativas = 0;
        char letra;
        int venceu = 0;

        for (int i = 0; i < tamanho; i++) {
            acertos[i] = 0; // Inicializa o vetor de acertos
        }

        printf("\nBem-vindo ao Jogo da Forca!\n");
        printf("Dica: %s\n", listaPalavras[indice].dica);

        while (tentativas < MAX_TENTATIVAS) {
            exibirPalavra(palavraSecreta, acertos);

            printf("\nDigite uma letra: ");
            scanf(" %c", &letra);
            letra = toupper(letra);

            if (!isalpha(letra)) {
                printf("Por favor, insira uma letra válida.\n");
                continue;
            }

            if (!verificarLetra(letra, palavraSecreta, acertos)) {
                tentativas++;
                printf("\nLetra errada! Você tem %d tentativas restantes.\n", MAX_TENTATIVAS - tentativas);
            }

            venceu = 1;
            for (int i = 0; i < tamanho; i++) {
                if (acertos[i] == 0) {
                    venceu = 0;
                    break;
                }
            }

            if (venceu) {
                printf("\nParabéns! Você acertou a palavra: %s\n", palavraSecreta);
                break;
            }
        }

        if (!venceu) {
            printf("\nFim de jogo! A palavra era: %s\n", palavraSecreta);
        }

        // Pergunta se o jogador quer jogar novamente
        jogar = jogarNovamente();
    }

    printf("\nObrigado por jogar! Até a próxima!\n");
    return 0;
}

void exibirPalavra(char palavra[], int acertos[]) {
    printf("\nPalavra: ");
    for (int i = 0; i < strlen(palavra); i++) {
        if (acertos[i]) {
            printf("%c ", palavra[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

int verificarLetra(char letra, char palavra[], int acertos[]) {
    int encontrou = 0;
    for (int i = 0; i < strlen(palavra); i++) {
        if (palavra[i] == letra) {
            acertos[i] = 1;
            encontrou = 1;
        }
    }
    return encontrou;
}

int jogarNovamente() {
    char resposta;
    printf("\nDeseja jogar novamente? (S/N): ");
    scanf(" %c", &resposta);
    resposta = toupper(resposta);

    while (resposta != 'S' && resposta != 'N') {
        printf("Por favor, insira S para Sim ou N para Não: ");
        scanf(" %c", &resposta);
        resposta = toupper(resposta);
    }

    return (resposta == 'S') ? 1 : 0;
}