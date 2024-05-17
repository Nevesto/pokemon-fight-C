#include "raylib.h"
#include <stdio.h>

#define POKEMON_COUNT 4
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_SPACING 20
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

Image pokemonImages[POKEMON_COUNT];
Texture2D pokemonTextures[POKEMON_COUNT];
typedef enum {
    POKEMON_BULBASAUR,
    POKEMON_CHARMANDER,
    POKEMON_SQUIRTLE,
    POKEMON_PIDGEY,
} Pokemon;

typedef struct {
    Texture2D texture;
    Rectangle rec;
} PokemonButton;

PokemonButton pokemonButtons[POKEMON_COUNT];

#define FILENAME_SIZE 100

void InitPokemonButtons() {
    int buttonX = (SCREEN_WIDTH - BUTTON_WIDTH * 2 - BUTTON_SPACING) / 2;
    int buttonY = (SCREEN_HEIGHT / 2) - (BUTTON_HEIGHT + BUTTON_SPACING) * 2;

    

    for (int i = 0; i < POKEMON_COUNT; i++) {
        char filename[FILENAME_SIZE];
        snprintf(filename, sizeof(filename), "../resources/menu/pokemon_%d.png", i + 1);
        pokemonImages[i] = LoadImage(filename);
        pokemonTextures[i] = LoadTextureFromImage(pokemonImages[i]);
        pokemonTextures[i] = LoadTextureFromImage(pokemonImages[i]);  // Use the original image without resizing

        pokemonButtons[i] = (PokemonButton){
            .texture = pokemonTextures[i],
            .rec = (Rectangle){
                buttonX,
                buttonY,
                BUTTON_WIDTH,
                BUTTON_HEIGHT
            }
        };
        buttonX += BUTTON_WIDTH + BUTTON_SPACING;
    }
}

Pokemon selectedPokemon = POKEMON_BULBASAUR;

void UpdatePokemonMenu() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();

        for (int i = 0; i < POKEMON_COUNT; i++) {
            if (CheckCollisionPointRec(mousePoint, pokemonButtons[i].rec)) {
                selectedPokemon = (Pokemon)i;
            }
        }
    }
}

void DrawPokemonMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Escolha seu Pokémon:", (SCREEN_WIDTH - MeasureText("Escolha seu Pokémon:", 20)) / 2, 50, 20, BLACK);

    for (int i = 0; i < POKEMON_COUNT; i++) {
        if (selectedPokemon == i) {
            DrawRectangleLines(pokemonButtons[i].rec.x, pokemonButtons[i].rec.y, pokemonButtons[i].rec.width, pokemonButtons[i].rec.height, RED);
        }
        DrawTexture(pokemonButtons[i].texture, pokemonButtons[i].rec.x, pokemonButtons[i].rec.y, WHITE);
    }
    
    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokemon Chooser");
    InitPokemonButtons();

    while (!WindowShouldClose())
    {
        UpdatePokemonMenu();
        DrawPokemonMenu();
    }

    CloseWindow();
    return 0;
}
