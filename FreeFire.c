#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_INICIAL 5
#define TAM_NOME 30
#define TAM_TIPO 20

// ======================
// Estruturas e Enumerações
// ======================

typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
    int prioridade; // 1 a 5 (quanto maior, mais importante)
} Item;

typedef enum {
    ORDENAR_POR_NOME = 1,
    ORDENAR_POR_TIPO,
    ORDENAR_POR_PRIORIDADE
} CriterioOrdenacao;

// ======================
// Funções utilitárias
// ======================

void limparTela() {
    for (int i = 0; i < 20; i++)
        printf("\n");
}

void exibirMenu(bool ordenadaPorNome) {
    printf("=== CODIGO DA ILHA: MOCHILA ===\n");
    printf("[1] Adicionar item\n");
    printf("[2] Remover item\n");
    printf("[3] Listar itens\n");
    printf("[4] Ordenar itens\n");
    printf("[5] Buscar item (busca binária por nome)\n");
    printf("[0] Sair\n");
    printf("\n(Status: %s)\n", ordenadaPorNome ? "Ordenada por nome ✅" : "Desordenada ❌");
    printf("Escolha uma opção: ");
}

// ======================
// Inserção e Remoção
// ======================

void inserirItem(Item **mochila, int *numItens, int *capacidade, bool *ordenadaPorNome) {
    if (*numItens >= *capacidade) {
        *capacidade *= 2;
        *mochila = realloc(*mochila, (*capacidade) * sizeof(Item));
        if (*mochila == NULL) {
            printf("Erro de memória!\n");
            exit(1);
        }
        printf("\n[+] Mochila expandida para %d espaços!\n", *capacidade);
    }

    Item novo;
    printf("Digite o nome do item: ");
    scanf(" %29[^\n]", novo.nome);
    printf("Digite o tipo do item: ");
    scanf(" %19[^\n]", novo.tipo);
    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Digite a prioridade (1 a 5): ");
    scanf("%d", &novo.prioridade);

    (*mochila)[*numItens] = novo;
    (*numItens)++;
    *ordenadaPorNome = false;

    printf("\nItem adicionado com sucesso!\n");
}

void removerItem(Item *mochila, int *numItens) {
    if (*numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    char alvo[TAM_NOME];
    printf("Digite o nome do item a remover: ");
    scanf(" %29[^\n]", alvo);

    for (int i = 0; i < *numItens; i++) {
        if (strcmp(mochila[i].nome, alvo) == 0) {
            for (int j = i; j < *numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*numItens)--;
            printf("Item removido com sucesso!\n");
            return;
        }
    }

    printf("Item não encontrado!\n");
}

// ======================
// Listagem
// ======================

void listarItens(Item *mochila, int numItens) {
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    printf("\n=== ITENS NA MOCHILA ===\n");
    printf("%-20s %-15s %-10s %-10s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < numItens; i++) {
        printf("%-20s %-15s %-10d %-10d\n",
               mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

// ======================
// Ordenação (Insertion Sort)
// ======================

int insertionSort(Item *mochila, int numItens, CriterioOrdenacao criterio) {
    int comparacoes = 0;

    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;

        while (j >= 0) {
            bool troca = false;
            comparacoes++;

            switch (criterio) {
                case ORDENAR_POR_NOME:
                    troca = strcmp(chave.nome, mochila[j].nome) < 0;
                    break;
                case ORDENAR_POR_TIPO:
                    troca = strcmp(chave.tipo, mochila[j].tipo) < 0;
                    break;
                case ORDENAR_POR_PRIORIDADE:
                    troca = chave.prioridade > mochila[j].prioridade;
                    break;
            }

            if (troca) {
                mochila[j + 1] = mochila[j];
                j--;
            } else break;
        }

        mochila[j + 1] = chave;
    }

    return comparacoes;
}

void menuDeOrdenacao(Item *mochila, int numItens, bool *ordenadaPorNome) {
    int opcao;
    printf("\n=== CRITÉRIOS DE ORDENAÇÃO ===\n");
    printf("[1] Nome (A-Z)\n");
    printf("[2] Tipo (A-Z)\n");
    printf("[3] Prioridade (alta -> baixa)\n");
    printf("Escolha: ");
    scanf("%d", &opcao);

    if (opcao < 1 || opcao > 3) {
        printf("Opção inválida!\n");
        return;
    }

    int comparacoes = insertionSort(mochila, numItens, opcao);
    printf("\nItens ordenados! Comparações realizadas: %d\n", comparacoes);

    *ordenadaPorNome = (opcao == ORDENAR_POR_NOME);
}

// ======================
// Busca Binária
// ======================

void buscaBinariaPorNome(Item *mochila, int numItens, bool ordenadaPorNome) {
    if (!ordenadaPorNome) {
        printf("A mochila precisa estar ordenada por nome!\n");
        return;
    }

    char alvo[TAM_NOME];
    printf("Digite o nome do item a buscar: ");
    scanf(" %29[^\n]", alvo);

    int inicio = 0, fim = numItens - 1, meio;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        int cmp = strcmp(alvo, mochila[meio].nome);
        if (cmp == 0) {
            printf("\nItem encontrado:\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d | Prioridade: %d\n",
                   mochila[meio].nome, mochila[meio].tipo,
                   mochila[meio].quantidade, mochila[meio].prioridade);
            return;
        } else if (cmp < 0)
            fim = meio - 1;
        else
            inicio = meio + 1;
    }

    printf("Item não encontrado.\n");
}

// ======================
// Função Principal
// ======================

int main() {
    Item *mochila = malloc(TAM_INICIAL * sizeof(Item));
    if (!mochila) {
        printf("Erro de memória!\n");
        return 1;
    }

    int numItens = 0, capacidade = TAM_INICIAL;
    bool ordenadaPorNome = false;
    int opcao;

    do {
        exibirMenu(ordenadaPorNome);
        scanf("%d", &opcao);
        limparTela();

        switch (opcao) {
            case 1: inserirItem(&mochila, &numItens, &capacidade, &ordenadaPorNome); break;
            case 2: removerItem(mochila, &numItens); break;
            case 3: listarItens(mochila, numItens); break;
            case 4: menuDeOrdenacao(mochila, numItens, &ordenadaPorNome); break;
            case 5: buscaBinariaPorNome(mochila, numItens, ordenadaPorNome); break;
            case 0: printf("Saindo... até a próxima fuga!\n"); break;
            default: printf("Opção inválida!\n");
        }

        printf("\n");
    } while (opcao != 0);

    free(mochila);
    return 0;
}
