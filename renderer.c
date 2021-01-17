#include<stdint.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include "renderer.h"

#define WIDTH 800
#define HEIGHT 600

void createWindow () {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("sdl init error: %s\n", SDL_GetError());
    }
    SDL_Surface *surface;
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
    }

    // Uint32 render_flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flag);
    // if (!renderer) {
    //     printf("sdl renderer create error: %s\n", SDL_GetError());
    // }
}

void procIO () {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            // running = 0;
        }
        if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym) {
            case SDLK_w:
            case SDLK_UP:
                break;
            case SDLK_a:
            case SDLK_LEFT:
                break;
            case SDLK_s:
            case SDLK_DOWN:
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                break;
            case SDLK_ESCAPE:
                break; 
            default:
                break;
            }
        }
    }
    SDL_RenderClear(renderer);
    SDL_Rect rect = {50, 50, 200, 200};
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(1000/60);
}

void renderScreen (const uint8_t *gfx) {
    for (int r = 0; r < 32; r++) {
        for (int c = 0; c < 64; c++) {
            printf("%c", gfx[r * 64 + c] ? 'O' : ' ');
        }
        printf("\n");
    }
}

void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
