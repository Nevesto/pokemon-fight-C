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

Image backPokemonImages[POKEMON_COUNT];
Texture2D backPokemonTextures[POKEMON_COUNT];

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

const char *menuPokemonFilenames[POKEMON_COUNT] = {
    "../resources/menu/pokemon_1.png",
    "../resources/menu/pokemon_2.png",
    "../resources/menu/pokemon_3.png",
    "../resources/menu/pokemon_4.png"
};

const char *backPokemonFilenames[POKEMON_COUNT] = {
    "../resources/fight/back/pokemon_1_back.png",
    "../resources/fight/back/pokemon_2_back.png",
    "../resources/fight/back/pokemon_3_back.png",
    "../resources/fight/back/pokemon_4_back.png"
};

Pokemon selectedPokemon = POKEMON_BULBASAUR;
bool pokemonChosen = false;

void InitPokemonButtons() {
    int totalButtonsWidth = (BUTTON_WIDTH * POKEMON_COUNT) + (BUTTON_SPACING * (POKEMON_COUNT - 1));
    int buttonX = (SCREEN_WIDTH - totalButtonsWidth) / 2;
    int buttonY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2 + 50;

    for (int i = 0; i < POKEMON_COUNT; i++) {
        pokemonImages[i] = LoadImage(menuPokemonFilenames[i]);
        ImageResize(&pokemonImages[i], BUTTON_WIDTH, BUTTON_HEIGHT);
        pokemonTextures[i] = LoadTextureFromImage(pokemonImages[i]);

        backPokemonImages[i] = LoadImage(backPokemonFilenames[i]);
        ImageResize(&backPokemonImages[i], BUTTON_WIDTH, BUTTON_HEIGHT);
        backPokemonTextures[i] = LoadTextureFromImage(backPokemonImages[i]);

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

    const char *selectText = "Selecionar Pokémon";
    int textWidth = MeasureText(selectText, 20);
    int padding = 20; // Add some padding around the text

    selectButton = (Rectangle){
        (SCREEN_WIDTH - (textWidth + padding)) / 2,
        buttonY + BUTTON_HEIGHT + 20,
        textWidth + padding,
        SELECT_BUTTON_HEIGHT
    };
}

void UpdatePokemonMenu() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();

        if (!pokemonChosen) {
            for (int i = 0; i < POKEMON_COUNT; i++) {
                if (CheckCollisionPointRec(mousePoint, pokemonButtons[i].rec)) {
                    selectedPokemon = (Pokemon)i;
                }
            }

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
    int textWidth = MeasureText("Escolha seu Pokémon:", 40);
    int posX = (SCREEN_WIDTH - textWidth) / 2; 
    DrawTextWithShadow("Escolha seu Pokémon:", posX, (SCREEN_HEIGHT / 2) - BUTTON_HEIGHT - 20, 40, WHITE, DARKGRAY, 2, 2);

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

    DrawRectangleRec(selectButton, WHITE);
    DrawRectangleLinesEx(selectButton, 2, DARKGRAY);
    DrawText("Selecionar Pokémon", selectButton.x + (selectButton.width - MeasureText("Selecionar Pokémon", 20)) / 2, selectButton.y + 10, 20, BLACK);
}

void DrawChosenPokemon() {
    float posX = (SCREEN_WIDTH - BUTTON_WIDTH) / 4;
    float posY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2;
    DrawTexture(backPokemonTextures[selectedPokemon], posX, posY, WHITE);
}

Pokemon randomPokemon = POKEMON_BULBASAUR;

Pokemon GetRandomPokemon() {
    if (randomPokemon == POKEMON_BULBASAUR) {
        int randomIndex = GetRandomValue(0, POKEMON_COUNT - 1);
        randomPokemon = (Pokemon)randomIndex;
    }
    return randomPokemon;
}

void DrawPokemonMenu() {
    static bool renderedRandomPokemon = false;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexturePro(backgroundTexture, 
                   (Rectangle){ 0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height }, 
                   (Rectangle){ 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }, 
                   (Vector2){ 0.0f, 0.0f }, 
                   0.0f, 
                   WHITE);

    if (!pokemonChosen) {
        DrawPokemonSelectionMenu();
    } else {
        DrawChosenPokemon();
        
        DrawTexture(pokemonTextures[GetRandomPokemon()], SCREEN_WIDTH - BUTTON_WIDTH, 10, WHITE);
        
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokemon Fight");
    font = LoadFont("../resources/Minecraft.ttf");
    backgroundImage = LoadImage("../resources/background/background-1.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    InitPokemonButtons();

    while (!WindowShouldClose()) {
        UpdatePokemonMenu();
        DrawPokemonMenu();
    }

    UnloadFont(font);
    for (int i = 0; i < POKEMON_COUNT; i++) {
        UnloadTexture(pokemonTextures[i]);
        UnloadTexture(backPokemonTextures[i]);
        UnloadImage(pokemonImages[i]);
        UnloadImage(backPokemonImages[i]);
    }
    UnloadTexture(backgroundTexture);

    CloseWindow();
    return 0;
}