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
    POKEMON_PIKACHU,
    RANDOM_POKEMON
} Pokemon;

typedef struct {
    Texture2D texture;
    Rectangle rec;
} PokemonButton;

typedef struct {
    Rectangle rec;
    const char *text;
    int damage;
} AttackButton;

PokemonButton pokemonButtons[POKEMON_COUNT];
AttackButton attackButtons[NUM_ATTACK_BUTTONS];
Rectangle selectButton;

Pokemon randomPokemon = RANDOM_POKEMON;
bool gameOver = false;
Rectangle restartButton;

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

const char *pokemonNames[POKEMON_COUNT] = {
    "Charmander",
    "Squirtle",
    "Bulbasaur",
    "Pikachu",
};

Pokemon selectedPokemon = POKEMON_BULBASAUR;
bool pokemonChosen = false;
const char *winnerName = NULL;

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

        pokemonButtons[i] = (PokemonButton) {
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
    for (int i = 0; i < NUM_ATTACK_BUTTONS; i++) {
        attackButtons[i].rec = (Rectangle) {
            buttonX,
            buttonY,
            ATTACK_BUTTON_WIDTH,
            ATTACK_BUTTON_HEIGHT
        };
        buttonX += ATTACK_BUTTON_WIDTH + ATTACK_BUTTON_SPACING;
    }
    attackButtons[0].text = "Ataque 1 - 30";
    attackButtons[0].damage = 30;

    attackButtons[1].text = "Ataque 2 - 40";
    attackButtons[1].damage = 40;

    attackButtons[2].text = "Ataque 3 - 10";
    attackButtons[2].damage = 10;

    attackButtons[3].text = "Ataque 4 - 20";
    attackButtons[3].damage = 20;
}

void InitRestartButton() {
    const char *restartText = "Reiniciar";
    int textWidth = MeasureText(restartText, 20);
    int padding = 20;
    restartButton = (Rectangle) {
        (SCREEN_WIDTH - (textWidth + padding)) / 2,
        (SCREEN_HEIGHT - SELECT_BUTTON_HEIGHT) / 2 + 50,
        textWidth + padding,
        SELECT_BUTTON_HEIGHT
    };
}

void EnemyAttack() {
    int randomAttackIndex = GetRandomValue(0, NUM_ATTACK_BUTTONS - 1);
    pokemonLife[selectedPokemon] -= attackButtons[randomAttackIndex].damage;
    if (pokemonLife[selectedPokemon] <= 0) {
        pokemonLife[selectedPokemon] = 0;
        winnerName = pokemonNames[randomPokemon];
        gameOver = true;
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
        } else if (!gameOver) {
            for (int i = 0; i < NUM_ATTACK_BUTTONS; i++) {
                if (CheckCollisionPointRec(mousePoint, attackButtons[i].rec)) {
                    enemyPokemonLife -= attackButtons[i].damage;
                    if (enemyPokemonLife <= 0) {
                        enemyPokemonLife = 0;
                        winnerName = pokemonNames[selectedPokemon];
                        gameOver = true;
                    } else {
                        EnemyAttack();
                    }
                }
            }
        } else {
            if (CheckCollisionPointRec(mousePoint, restartButton)) {
                pokemonChosen = false;
                gameOver = false;
                winnerName = NULL;
                for (int i = 0; i < POKEMON_COUNT; i++) {
                    pokemonLife[i] = 100.0f;
                }
                enemyPokemonLife = 100.0f;
                randomPokemon = RANDOM_POKEMON;
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

Pokemon GetRandomPokemon() {
    if (randomPokemon == RANDOM_POKEMON) {
        int randomIndex = GetRandomValue(0, POKEMON_COUNT - 1);
        randomPokemon = (Pokemon)randomIndex;

        if (randomPokemon == selectedPokemon) {
            return GetRandomPokemon();
        }
    }

    return randomPokemon;
}

void DrawPokemonMenu() {
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

        DrawTexture(backPokemonTextures[selectedPokemon], posX, posY, WHITE);

        float enemyPosX = SCREEN_WIDTH - BUTTON_WIDTH - 150; 
        float enemyPosY = posY;

        DrawTexture(pokemonTextures[GetRandomPokemon()], enemyPosX, enemyPosY, WHITE);

        for (int i = 0; i < NUM_ATTACK_BUTTONS; i++) {
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

        float enemyLifeBarPosY = enemyPosY + BUTTON_HEIGHT + LIFE_BAR_HEIGHT;

        Rectangle enemyLifeBarRec = {
            enemyPosX,
            enemyLifeBarPosY,
            LIFE_BAR_WIDTH,
            LIFE_BAR_HEIGHT
        };

        DrawLifeBar(enemyPokemonLife, enemyLifeBarRec, RED);

        if (gameOver) {
            if (winnerName) {
                DrawTextWithShadow(TextFormat("O vencedor é: %s", winnerName), (SCREEN_WIDTH - MeasureText(TextFormat("O vencedor é: %s", winnerName), 40)) / 2, SCREEN_HEIGHT / 2 - 100, 40, GREEN, DARKGRAY, 2, 2);
            }

            DrawRectangleRec(restartButton, WHITE);
            DrawRectangleLinesEx(restartButton, 2, DARKGRAY);
            DrawText("Reiniciar", restartButton.x + (restartButton.width - MeasureText("Reiniciar", 20)) / 2, restartButton.y + 10, 20, BLACK);
        }
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
    InitRestartButton();

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
