#include <raylib.h>
#include <stdio.h>

#define POKEMON_COUNT 4
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50
#define BUTTON_SPACING 20
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

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

#define FILENAME_SIZE 100

void InitPokemonButtons() {
    int totalButtonsWidth = BUTTON_WIDTH * POKEMON_COUNT + BUTTON_SPACING * (POKEMON_COUNT - 1);
    int buttonX = (SCREEN_WIDTH - totalButtonsWidth) / 2;
    int buttonY = (SCREEN_HEIGHT / 2) - (BUTTON_HEIGHT + BUTTON_SPACING) * 2;    

    for (int i = 0; i < POKEMON_COUNT; i++) {
        char filename[FILENAME_SIZE];
        snprintf(filename, sizeof(filename), "../resources/menu/pokemon_%d.png", i + 1);
        pokemonImages[i] = LoadImage(filename);
        ImageResize(&pokemonImages[i], 200, 200);
        pokemonTextures[i] = LoadTextureFromImage(pokemonImages[i]);

        pokemonButtons[i] = (PokemonButton){
            .texture = pokemonTextures[i],
            .rec = (Rectangle) {
                buttonX,
                buttonY,
                200,
                200
            }
        };
        buttonX += pokemonImages[i].width + BUTTON_SPACING;
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

void DrawTextWithShadow(const char *text, int posX, int posY, int fontSize, Color color, Color shadowColor, int offsetX, int offsetY) {
    // Desenhar a sombra
    DrawText(text, posX + offsetX, posY + offsetY, fontSize, shadowColor);
    
    // Desenhar o texto principal
    DrawText(text, posX, posY, fontSize, color);
}



void DrawPokemonMenu() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    float scaleX = (float)SCREEN_WIDTH / backgroundTexture.width;
    float scaleY = (float)SCREEN_HEIGHT / backgroundTexture.height;


    DrawTexturePro(backgroundTexture, (Rectangle){ 0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height },
               (Rectangle){ 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

    DrawTextWithShadow("Escolha seu Pokémon:", (SCREEN_WIDTH - MeasureText("Escolha seu Pokémon:", 40)) / 2, 250, 40, WHITE, DARKGRAY, 2, 2);

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
    LoadFont("../resources/Minecraft.ttf");
    backgroundImage = LoadImage("../resources/background.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    InitPokemonButtons();

    while (!WindowShouldClose()) {
        UpdatePokemonMenu();
        DrawPokemonMenu();
    }

    // Descarregar recursos
    UnloadFont(font);
    for (int i = 0; i < POKEMON_COUNT; i++) {
        UnloadImage(pokemonImages[i]);
        UnloadTexture(pokemonTextures[i]);
    }

    CloseWindow();
    return 0;
}
