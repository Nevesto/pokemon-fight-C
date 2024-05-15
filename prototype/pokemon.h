#ifndef POKEMON_H
#define POKEMON_H

struct Pokemon {
    char nome[50];
    int vida;
    char ataque1_nome[50];
    int ataque1;
    char ataque2_nome[50];
    int ataque2;
    char ataque3_nome[50];
    int ataque3;
    char ataque4_nome[50];
    int ataque4;
};

struct Pokemon criarPokemon(char *nome, int vida, char *ataque1_nome, int ataque1, char *ataque2_nome, int ataque2, char *ataque3_nome, int ataque3, char *ataque4_nome, int ataque4);

#endif
