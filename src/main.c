#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Rect rect;
    SDL_Texture *texture;
} Button;

SDL_Texture* loadTexture(SDL_Renderer *renderer, const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        printf("Erro ao carregar a imagem: %s\n", IMG_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void drawButton(SDL_Renderer *renderer, Button *button) {
    SDL_RenderCopy(renderer, button->texture, NULL, &button->rect);
}

int main() {
    bool running = true;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erro ao inicializar o SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        printf("Erro ao inicializar a biblioteca TTF: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Criação da janela
    SDL_Window* window = SDL_CreateWindow("Minha Janela", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, SDL_WINDOW_SHOWN);
    if(!window) {
        fprintf(stderr, "Erro na criação da janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Criar o renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Erro na criação do renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Carregar a textura do botão
    SDL_Texture *buttonTexture = loadTexture(renderer, "../resources/pikachu-menu.png");
    if (!buttonTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Criar o botão
    Button button;
    button.texture = buttonTexture;

    // Obter as dimensões originais da textura do botão
    int buttonWidth, buttonHeight;
    SDL_QueryTexture(buttonTexture, NULL, NULL, &buttonWidth, &buttonHeight);

    // Definir as dimensões do botão
    button.rect.w = buttonWidth;
    button.rect.h = buttonHeight;
    button.rect.x = (640 - button.rect.w) / 2; // Posição x do botão
    button.rect.y = (480 - button.rect.h) / 2; // Posição y do botão

    // Carregar a fonte
    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/truetype/msttcorefonts/arial.ttf", 24);
    if (!font) {
        printf("Erro ao carregar a fonte: %s\n", TTF_GetError());
        SDL_DestroyTexture(button.texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Color textColor = {0, 0, 0}; // Cor preta
    
    // loop while para rodar a janela
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) { // Verificar se ocorreu um clique do mouse
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (mouseX >= button.rect.x && mouseX <= button.rect.x + button.rect.w && mouseY >= button.rect.y && mouseY <= button.rect.y + button.rect.h) { // Verificar se o clique foi dentro do botão
                    printf("Pikachu usou investida!\n");
                }
            }
        }

        // Definir cor de fundo para branco
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Limpar a tela com a cor de fundo
        SDL_RenderClear(renderer);

        // Renderizar os elementos na tela
        drawButton(renderer, &button);

        // Desenhar o texto
        SDL_Surface* surface = TTF_RenderText_Solid(font, "Escolha seu Pokemon:", textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect textRect = {50, 50, 0, 0}; // Posição do texto
        SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_DestroyTexture(texture);

        // Atualizar a janela
        SDL_RenderPresent(renderer);
    }

    // Limpar efeitos visuais
    SDL_DestroyTexture(button.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
