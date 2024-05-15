#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pokemon.h"

// Função para exibir os Pokémon disponíveis e permitir ao jogador escolher um
struct Pokemon escolherPokemon() {
    int escolha;
    printf("\nEscolha um Pokémon:\n");
    printf("1. Pikachu\n");
    printf("2. Squirtle\n");
    printf("3. Charmander\n");
    printf("4. Bulbasaur\n");
    printf("Escolha: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            return criarPokemon("Pikachu", 120, "Investida", 25, "Choque do Trovão", 35, "Rabo de Ferro", 30, "Ataque Rápido", 40);
        case 2:
            return criarPokemon("Squirtle", 100, "Jato d'Água", 30, "Tackle", 15, "Bolha", 50, "Recuo", 10);
        case 3:
            return criarPokemon("Charmander", 80, "Brasa", 30, "Garra de Dragão", 25, "Investida Flamejante", 40, "Chama de Fogo", 35);
        case 4:
            return criarPokemon("Bulbasaur", 150, "Chicote de Vinha", 20, "Semente Sanguessuga", 15, "Raio Solar", 30, "Folha Navalha", 25);
        default:
            printf("Opção inválida! Escolhendo Pikachu por padrão.\n");
            return criarPokemon("Pikachu", 120, "Investida", 25, "Choque do Trovão", 35, "Rabo de Ferro", 30, "Ataque Rápido", 40);
    }
}

// Função para realizar uma rodada de batalha entre dois Pokémon
void batalha(struct Pokemon *p1, struct Pokemon *p2) {
    printf("\nA batalha começa entre %s e %s!\n", p1->nome, p2->nome);
    
    while (p1->vida > 0 && p2->vida > 0) {
        // Exibir opções de ataques para o Pokémon 1
        printf("\n%s, escolha seu ataque:\n", p1->nome);
        printf("1. %s\n", p1->ataque1_nome);
        printf("2. %s\n", p1->ataque2_nome);
        printf("3. %s\n", p1->ataque3_nome);
        printf("4. %s\n", p1->ataque4_nome);
        printf("Escolha o número do ataque: ");
        
        int escolha1;
        scanf("%d", &escolha1);
        
        // Exibir opções de ataques para o Pokémon 2
        printf("\n%s, escolha seu ataque:\n", p2->nome);
        printf("1. %s\n", p2->ataque1_nome);
        printf("2. %s\n", p2->ataque2_nome);
        printf("3. %s\n", p2->ataque3_nome);
        printf("4. %s\n", p2->ataque4_nome);
        printf("Escolha o número do ataque: ");
        
        int escolha2;
        scanf("%d", &escolha2);
        
        // Realizar ataques
        int dano1, dano2;
        switch (escolha1) {
            case 1:
                dano1 = rand() % p1->ataque1 + 1;
                break;
            case 2:
                dano1 = rand() % p1->ataque2 + 1;
                break;
            case 3:
                dano1 = rand() % p1->ataque3 + 1;
                break;
            case 4:
                dano1 = rand() % p1->ataque4 + 1;
                break;
            default:
                printf("Opção inválida! Utilizando o ataque padrão.\n");
                dano1 = rand() % p1->ataque1 + 1;
                break;
        }
        
        switch (escolha2) {
            case 1:
                dano2 = rand() % p2->ataque1 + 1;
                break;
            case 2:
                dano2 = rand() % p2->ataque2 + 1;
                break;
            case 3:
                dano2 = rand() % p2->ataque3 + 1;
                break;
            case 4:
                dano2 = rand() % p2->ataque4 + 1;
                break;
            default:
                printf("Opção inválida! Utilizando o ataque padrão.\n");
                dano2 = rand() % p2->ataque1 + 1;
                break;
        }
        
        // Aplicar danos
        p2->vida -= dano1;
        p1->vida -= dano2;
        
        // Exibir resultados da rodada
        printf("%s ataca %s com %s, causando %d de dano. %s tem %d de vida restante.\n", p1->nome, p2->nome, p1->ataque1_nome, dano1, p2->nome, p2->vida);
        printf("%s ataca %s com %s, causando %d de dano. %s tem %d de vida restante.\n", p2->nome, p1->nome, p2->ataque1_nome, dano2, p1->nome, p1->vida);
        
        // Verificar se algum Pokémon desmaiou
        if (p1->vida <= 0) {
            printf("%s desmaiou! %s vence a batalha!\n", p1->nome, p2->nome);
            break;
        }
        
        if (p2->vida <= 0) {
            printf("%s desmaiou! %s vence a batalha!\n", p2->nome, p1->nome);
            break;
        }
    }
}

int main() {
    // Inicializar a semente para a função rand()
    srand(time(NULL));
    
    // Escolher Pokémon para os jogadores
    struct Pokemon jogador1 = escolherPokemon();
    struct Pokemon jogador2 = escolherPokemon();
    
    // Batalha entre os Pokémon escolhidos
    batalha(&jogador1, &jogador2);
    
    return 0;
}
