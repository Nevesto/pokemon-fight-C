#include "pokemon.h"
#include <string.h>

struct Pokemon criarPokemon(char *nome, int vida, char *ataque1_nome, int ataque1, char *ataque2_nome, int ataque2, char *ataque3_nome, int ataque3, char *ataque4_nome, int ataque4) {
    struct Pokemon novoPokemon;
    strcpy(novoPokemon.nome, nome);
    novoPokemon.vida = vida;
    strcpy(novoPokemon.ataque1_nome, ataque1_nome);
    novoPokemon.ataque1 = ataque1;
    strcpy(novoPokemon.ataque2_nome, ataque2_nome);
    novoPokemon.ataque2 = ataque2;
    strcpy(novoPokemon.ataque3_nome, ataque3_nome);
    novoPokemon.ataque3 = ataque3;
    strcpy(novoPokemon.ataque4_nome, ataque4_nome);
    novoPokemon.ataque4 = ataque4;
    return novoPokemon;
}
