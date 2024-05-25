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

#define ATTACK_BUTTON_WIDTH 200
#define ATTACK_BUTTON_HEIGHT 50
#define ATTACK_BUTTON_SPACING 20
#define NUM_ATTACK_BUTTONS 4

Image pokemonImages[POKEMON_COUNT];
Texture2D pokemonTextures[POKEMON_COUNT];
Font font;
Image backgroundImage;
Texture2D backgroundTexture;

Image backPokemonImages[POKEMON_COUNT];
Texture2D backPokemonTextures[POKEMON_COUNT];

float pokemonLife[POKEMON_COUNT] = {100.0f, 100.0f, 100.0f, 100.0f};
float enemyPokemonLife = 100.0f;

const int LIFE_BAR_WIDTH = 200;
const int LIFE_BAR_HEIGHT = 20;
const int LIFE_BAR_OFFSET_Y = 50;
const int ENEMY_LIFE_BAR_HEIGHT = 20;

typedef enum {
    POKEMON_BULBASAUR,
    POKEMON_CHARMANDER,
    POKEMON_SQUIRTLE,
    POKEMON_PIDGEY,
    RANDOM_POKEMON
} Pokemon;

typedef struct {
    Texture2D texture;
    Rectangle rec;
} PokemonButton;

typedef struct {
    Rectangle rec;
    const char *text;
} AttackButton;

PokemonButton pokemonButtons[POKEMON_COUNT];

AttackButton attackButtons[NUM_ATTACK_BUTTONS];

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
    int padding = 20;

    selectButton = (Rectangle){
        (SCREEN_WIDTH - (textWidth + padding)) / 2,
        buttonY + BUTTON_HEIGHT + 20,
        textWidth + padding,
        SELECT_BUTTON_HEIGHT
    };
}

    void initAttackButtons() {
        int totalButtonsWidth = (ATTACK_BUTTON_WIDTH * NUM_ATTACK_BUTTONS) + (ATTACK_BUTTON_SPACING * (NUM_ATTACK_BUTTONS - 1));
        int buttonX = (SCREEN_WIDTH - totalButtonsWidth) / 2;
        int buttonY = SCREEN_HEIGHT - ATTACK_BUTTON_HEIGHT - 20;

        for(int i = 0; i < NUM_ATTACK_BUTTONS; i++) {
            attackButtons[i].rec = (Rectangle) {
                buttonX,
                buttonY,
                ATTACK_BUTTON_WIDTH,
                ATTACK_BUTTON_HEIGHT
            };

            attackButtons[0].text = TextFormat("Investida");
            attackButtons[1].text = TextFormat("CHoque do trovão");
            attackButtons[2].text = TextFormat("Ataque rápido");
            attackButtons[3].text = TextFormat("Rabo de ferro");
            buttonX += ATTACK_BUTTON_WIDTH + ATTACK_BUTTON_SPACING;
        }
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

void DrawLifeBar(float life, Rectangle rec, Color color) {
    float lifePercentage = life / 100.0f;
    int lifeBarWidth = (int)(lifePercentage * LIFE_BAR_WIDTH);

    DrawRectangle(rec.x, rec.y, lifeBarWidth, LIFE_BAR_HEIGHT, color);
    DrawRectangleLines(rec.x, rec.y, LIFE_BAR_WIDTH, LIFE_BAR_HEIGHT, BLACK);
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

Pokemon randomPokemon = RANDOM_POKEMON;

Pokemon GetRandomPokemon() {
    if (randomPokemon == RANDOM_POKEMON) {
        int randomIndex = GetRandomValue(0, POKEMON_COUNT - 1);
        randomPokemon = (Pokemon)randomIndex;

        if(randomPokemon == selectedPokemon) {
            return GetRandomPokemon();
            randomPokemon == (Pokemon)randomIndex;
        }
    }
    return randomPokemon;
}

void DrawPokemonMenu() {
    static bool renderedRandomPokemon = false;

    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTexturePro(
                    backgroundTexture, 
                   (Rectangle){ 0.0f, 0.0f, (float)backgroundTexture.width, (float)backgroundTexture.height }, 
                   (Rectangle){ 0.0f, 0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }, 
                   (Vector2){ 0.0f, 0.0f }, 
                   0.0f, 
                   WHITE
                   );

    if (!pokemonChosen) {
        DrawPokemonSelectionMenu();
    } else {
        float posX = (SCREEN_WIDTH - BUTTON_WIDTH) / 4;
        float posY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2;

        DrawChosenPokemon();
        
        DrawTexture(pokemonTextures[GetRandomPokemon()], SCREEN_WIDTH - BUTTON_WIDTH, posY, WHITE); // Desenhe o Pokémon inimigo
                
        for(int i = 0; i < NUM_ATTACK_BUTTONS; i++) {
            DrawRectangleRec(attackButtons[i].rec, WHITE);
            DrawRectangleLinesEx(attackButtons[i].rec, 2, DARKGRAY);
            DrawText(attackButtons[i].text, attackButtons[i].rec.x + (attackButtons[i].rec.width - MeasureText(attackButtons[i].text, 20)) / 2, attackButtons[i].rec.y + 10, 20, BLACK);
        }


        Rectangle lifeBarRec = {
            posX,
            posY + BUTTON_HEIGHT + LIFE_BAR_HEIGHT,
            LIFE_BAR_WIDTH,
            LIFE_BAR_HEIGHT
        };
        DrawLifeBar(pokemonLife[selectedPokemon], lifeBarRec, GREEN);

        float enemy_posY = (SCREEN_HEIGHT - BUTTON_HEIGHT) / 2 - ENEMY_LIFE_BAR_HEIGHT - LIFE_BAR_OFFSET_Y;

        Rectangle enemyLifeBarRec = {
                SCREEN_WIDTH - BUTTON_WIDTH - LIFE_BAR_WIDTH,
                enemy_posY + BUTTON_HEIGHT + ENEMY_LIFE_BAR_HEIGHT,
                LIFE_BAR_WIDTH,
                LIFE_BAR_HEIGHT

        };
        DrawLifeBar(enemyPokemonLife, enemyLifeBarRec, RED);
    }

    EndDrawing();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pokemon Fight");
    font = LoadFont("../resources/Minecraft.ttf");
    backgroundImage = LoadImage("../resources/background/background-1.png");
    backgroundTexture = LoadTextureFromImage(backgroundImage);
    InitPokemonButtons();
    initAttackButtons();

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