#include <stdio.h>
#include <raylib.h>

#define POKEMON_COUNT 4
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 200
#define BUTTON_SPACING 20
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960
#define SELECT_BUTTON_WIDTH 200
#define SELECT_BUTTON_HEIGHT 50

Image pokemonImages[POKEMON_COUNT];
Texture2D pokemonTextures[POKEMON_COUNT];
Font font;
Image backgroundImage;
Texture2D backgroundTexture;

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

Rectangle selectButton;

#define FILENAME_SIZE 100

void InitPokemonButtons() {
    int totalButtonsWidth = (BUTTON_WIDTH * POKEMON_COUNT) + (BUTTON_SPACING * (POKEMON_COUNT - 1));
    int buttonX = (SCREEN_WIDTH - totalButtonsWidth) / 2;
    int buttonY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2 + 50;

    for (int i = 0; i < POKEMON_COUNT; i++) {
        char filename[FILENAME_SIZE];
        snprintf(filename, sizeof(filename), "../resources/menu/pokemon_%d.png", i + 1);
        pokemonImages[i] = LoadImage(filename);
        ImageResize(&pokemonImages[i], BUTTON_WIDTH, BUTTON_HEIGHT);
        pokemonTextures[i] = LoadTextureFromImage(pokemonImages[i]);

        pokemonButtons[i] = (PokemonButton){
            .texture = pokemonTextures[i],
            .rec = (Rectangle) {
                buttonX,
                buttonY,
                BUTTON_WIDTH,
                BUTTON_HEIGHT
            }
        };

        buttonX += BUTTON_WIDTH + BUTTON_SPACING;
    }

    // Initialize the "Select Pokemon" button
    selectButton = (Rectangle){
        (SCREEN_WIDTH - SELECT_BUTTON_WIDTH) / 2,
        buttonY + BUTTON_HEIGHT + 20,  // Position it below the Pokemon buttons
        SELECT_BUTTON_WIDTH,
        SELECT_BUTTON_HEIGHT
    };
}

Pokemon selectedPokemon = POKEMON_BULBASAUR;
bool pokemonChosen = false;

void UpdatePokemonMenu() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();

        if (!pokemonChosen) {
            for (int i = 0; i < POKEMON_COUNT; i++) {
                if (CheckCollisionPointRec(mousePoint, pokemonButtons[i].rec)) {
                    selectedPokemon = (Pokemon)i;
                }
            }

            // Check if the "Select Pokemon" button is clicked
            if (CheckCollisionPointRec(mousePoint, selectButton)) {
                pokemonChosen = true;
            }
        }
    }
}

void DrawTextWithShadow(const char *text, int posX, int posY, int fontSize, Color color, Color shadowColor, int offsetX, int offsetY) {
    DrawText(text, posX + offsetX, posY + offsetY, fontSize, shadowColor);
    DrawText(text, posX, posY, fontSize, color);
}

void DrawPokemonSelectionMenu() {
    // Draw the selection text
    DrawTextWithShadow("Escolha seu Pokémon:", (SCREEN_WIDTH - MeasureText("Escolha seu Pokémon:", 40)) / 2, (SCREEN_HEIGHT / 2) - BUTTON_HEIGHT - 20, 40, WHITE, DARKGRAY, 2, 2);

    // Draw the Pokemon buttons
    for (int i = 0; i < POKEMON_COUNT; i++) {
        if (selectedPokemon == i) {
            DrawRectangleLinesEx(
                (Rectangle){
                    pokemonButtons[i].rec.x - 5,
                    pokemonButtons[i].rec.y - 5,
                    pokemonButtons[i].rec.width + 10,
                    pokemonButtons[i].rec.height + 10
                },
                5,
                WHITE
            );
        }
        DrawTexture(pokemonButtons[i].texture, pokemonButtons[i].rec.x, pokemonButtons[i].rec.y, WHITE);
    }

    // Draw the "Select Pokemon" button
    DrawRectangleRec(selectButton, LIGHTGRAY);
    DrawRectangleLinesEx(selectButton, 2, DARKGRAY);
    DrawText("Selecionar Pokémon", selectButton.x + 10, selectButton.y + 10, 20, BLACK);
}

void DrawChosenPokemon() {
    // Draw only the selected Pokemon
    DrawTexture(pokemonTextures[selectedPokemon], (SCREEN_WIDTH - BUTTON_WIDTH) / 2, (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2, WHITE);
}

void DrawPokemonMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexturePro(backgroundTexture, (Rectangle){ 0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height }, 
    (Rectangle){ 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

    if (!pokemonChosen) {
        DrawPokemonSelectionMenu();
    } else {
        DrawChosenPokemon();
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokemon Chooser");
    font = LoadFont("../resources/Minecraft.ttf");
    backgroundImage = LoadImage("../resources/background.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    InitPokemonButtons();

    while (!WindowShouldClose()) {
        UpdatePokemonMenu();
        DrawPokemonMenu();
    }

    UnloadFont(font);
    for (int i = 0; i < POKEMON_COUNT; i++) {
        UnloadImage(pokemonImages[i]);
        UnloadTexture(pokemonTextures[i]);
    }

    CloseWindow();
    return 0;
}
