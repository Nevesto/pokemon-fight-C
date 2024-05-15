#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

// Função para carregar uma imagem
SDL_Texture* loadTexture(SDL_Renderer *renderer, const char *filePath) {
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface) {
        std::cerr << "Erro ao carregar imagem: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

int main() {
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    // Inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Cria janela
    window = SDL_CreateWindow("Exemplo de SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Cria renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Carrega imagem
    SDL_Texture *image = loadTexture(renderer, "imagem.png");
    if (!image) {
        return -1;
    }

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Limpa a tela
        SDL_RenderClear(renderer);

        // Renderiza a imagem
        SDL_RenderCopy(renderer, image, NULL, NULL);

        // Atualiza a tela
        SDL_RenderPresent(renderer);
    }

    // Libera recursos
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
