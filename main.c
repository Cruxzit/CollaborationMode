#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PRATELEIRAS 8
#define MAX_POSICOES 8
#define MAX_EXEMPLARES 10
#define MAX_NOME 50
#define MAX_MARCA 30
#define ARQUIVO_TXT "maquina_vendas.txt"  // Nome do arquivo para armazenar os dados

typedef struct {
    int dia, mes, ano;
} DataValidade;

typedef struct {
    char nome[MAX_NOME];
    char tipo; // Comida, Agua, Sumo, Refrigerante, Leite
    char marca[MAX_MARCA];
    float preco;
    DataValidade validade;
} Produto;

typedef struct {
    Produto produtos[MAX_EXEMPLARES];
    int quantidade; // Número atual de exemplares
} Posicao;

typedef struct {
    Posicao prateleiras[MAX_PRATELEIRAS][MAX_POSICOES];
} MaquinaVendas;

// Função para inicializar a máquina
void iniciarMaquina(MaquinaVendas *maquina) {
    for (int i = 0; i < MAX_PRATELEIRAS; i++) {
        for (int j = 0; j < MAX_POSICOES; j++) {
            maquina->prateleiras[i][j].quantidade = 0;
        }
    }
}

// Função para adicionar um produto
void adicionarProduto(MaquinaVendas *maquina, int prateleira, int posicao, Produto produto, int quantidade) {
    if (prateleira >= MAX_PRATELEIRAS || posicao >= MAX_POSICOES) {
        printf("Posição inválida!\n");
        return;
    }

    if (maquina->prateleiras[prateleira][posicao].quantidade + quantidade > MAX_EXEMPLARES) {
        printf("Espaço insuficiente nesta posição!\n");
        return;
    }

    Posicao *slot = &maquina->prateleiras[prateleira][posicao];
    for (int i = 0; i < quantidade; i++) {
        slot->produtos[slot->quantidade++] = produto;
    }
    printf("%d unidades de '%s' adicionadas à prateleira %d, posição %d.\n", quantidade, produto.nome, prateleira, posicao);
}

// Função para exibir o stock
void exibirStock(MaquinaVendas *maquina) {
    for (int i = 0; i < MAX_PRATELEIRAS; i++) {
        for (int j = 0; j < MAX_POSICOES; j++) {
            Posicao *slot = &maquina->prateleiras[i][j];
            if (slot->quantidade > 0) {
                Produto p = slot->produtos[0];
                printf("Prateleira %d, Posição %d: %d unidades de %s (%s) - Tipo: %c, Preço: %.2f\n",
                       i, j, slot->quantidade, p.nome, p.marca, p.tipo, p.preco);
            }
        }
    }
}

// Função para registar uma venda
float venderProduto(MaquinaVendas *maquina, int prateleira, int posicao) {
    if (prateleira >= MAX_PRATELEIRAS || posicao >= MAX_POSICOES) {
        printf("Posição inválida!\n");
        return 0;
    }

    Posicao *slot = &maquina->prateleiras[prateleira][posicao];
    if (slot->quantidade == 0) {
        printf("O Produto nesta posição está esgotado!\n");
        return 0;
    }

    Produto vendido = slot->produtos[--slot->quantidade];
    printf("Produto '%s' vendido por %.2f!\n", vendido.nome, vendido.preco);
    return vendido.preco;
}

// Função para verificar a validade dos produtos
void verificarValidade(MaquinaVendas *maquina) {
    time_t t = time(NULL);
    struct tm hoje = *localtime(&t);

    for (int i = 0; i < MAX_PRATELEIRAS; i++) {
        for (int j = 0; j < MAX_POSICOES; j++) {
            Posicao *slot = &maquina->prateleiras[i][j];
            for (int k = 0; k < slot->quantidade; k++) {
                Produto *p = &slot->produtos[k];
                if (p->validade.ano < hoje.tm_year + 1900 ||
                    (p->validade.ano == hoje.tm_year + 1900 && p->validade.mes < hoje.tm_mon + 1) ||
                    (p->validade.ano == hoje.tm_year + 1900 && p->validade.mes == hoje.tm_mon + 1 && p->validade.dia < hoje.tm_mday)) {
                    printf("Produto %s na prateleira %d, posição %d está vencido!\n", p->nome, i, j);
                }
            }
        }
    }
}

// Função para exibir o menu
void exibirMenu() {
    printf("\n=== Menu da Máquina de Vendas ===\n");
    printf("1. Adicionar Produto\n");
    printf("2. Exibir Stock\n");
    printf("3. Vender Produto\n");
    printf("4. Verificar Validade\n");
    printf("5. Guardar no TXT\n");
    printf("6. Carregar do TXT\n");
    printf("7. Sair\n");
    printf("Selecione uma opção: ");
}

// Função para salvar a máquina de vendas num arquivo texto
void guardarMaquinaTXT(MaquinaVendas *maquina) {
    FILE *arquivo = fopen(ARQUIVO_TXT, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para salvar!\n");
        return;
    }

    for (int i = 0; i < MAX_PRATELEIRAS; i++) {
        for (int j = 0; j < MAX_POSICOES; j++) {
            Posicao *slot = &maquina->prateleiras[i][j];
            if (slot->quantidade > 0) {
                fprintf(arquivo, "Prateleira %d, Posição %d\n", i, j);
                for (int k = 0; k < slot->quantidade; k++) {
                    Produto *p = &slot->produtos[k];
                    fprintf(arquivo, "%s,%c,%s,%.2f,%02d/%02d/%04d\n",
                            p->nome, p->tipo, p->marca, p->preco,
                            p->validade.dia, p->validade.mes, p->validade.ano);
                }
            }
        }
    }

    fclose(arquivo);
    printf("Dados guardados com sucesso no arquivo '%s'.\n", ARQUIVO_TXT);
}

// Função para carregar o stock da máquina de vendas a partir de um arquivo de texto
void carregarMaquinaTXT(MaquinaVendas *maquina) {
    FILE *arquivo = fopen(ARQUIVO_TXT, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura!\n");
        return;
    }

    iniciarMaquina(maquina);
    int prateleira, posicao;
    char linha[200];

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (sscanf(linha, "Prateleira %d, Posição %d", &prateleira, &posicao) == 2) {
            while (fgets(linha, sizeof(linha), arquivo)) {
                if (strstr(linha, "Prateleira")) {
                    fseek(arquivo, -strlen(linha), SEEK_CUR);
                    break;
                }

                Produto produto;
                int dia, mes, ano;
                if (sscanf(linha, "%49[^,],%c,%29[^,],%f,%d/%d/%d",
                           produto.nome, &produto.tipo, produto.marca,
                           &produto.preco, &dia, &mes, &ano) == 7) {
                    produto.validade.dia = dia;
                    produto.validade.mes = mes;
                    produto.validade.ano = ano;
                    adicionarProduto(maquina, prateleira, posicao, produto, 1);
                }
            }
        }
    }

    fclose(arquivo);
    printf("Dados do arquivo carregados com sucesso '%s'.\n", ARQUIVO_TXT);
}

int main() {
    MaquinaVendas maquina;
    iniciarMaquina(&maquina);
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Produto produto;
                int prateleira, posicao, quantidade;
printf("Introduza o nome do produto: ");
scanf(" %49[^\n]", produto.nome);
printf("Introduza o tipo do produto (C, A, S, R, L): ");
scanf(" %c", &produto.tipo);
printf("Introduza a marca do produto: ");
scanf(" %29[^\n]", produto.marca);
                printf("Introduza o preço do produto: ");
                scanf("%f", &produto.preco);
                printf("Introduza a data de validade (dia mes ano): ");
                scanf("%d %d %d", &produto.validade.dia, &produto.validade.mes, &produto.validade.ano);
                printf("Introduza a prateleira (0-7): ");
                scanf("%d", &prateleira);
                printf("Introduza a posição (0-7): ");
                scanf("%d", &posicao);
                printf("Introduza a quantidade: ");
                scanf("%d", &quantidade);

                adicionarProduto(&maquina, prateleira, posicao, produto, quantidade);
                break;
            }
            case 2:
                exibirStock(&maquina);
                break;
            case 3: {
                int prateleira, posicao;
                printf("Introduza a prateleira (0-7): ");
                scanf("%d", &prateleira);
                printf("Introduza a posição (0-7): ");
                scanf("%d", &posicao);
                venderProduto(&maquina, prateleira, posicao);
                break;
            }
            case 4:
                verificarValidade(&maquina);
                break;
            case 5:
                guardarMaquinaTXT(&maquina);
                break;
            case 6:
                carregarMaquinaTXT(&maquina);
                break;
            case 7:
                printf("A sair do programa.\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 7);

    return 0;
}
