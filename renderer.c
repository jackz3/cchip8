#include<stdint.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include "renderer.h"
#include "chip8.h"

#define WIDTH 640
#define HEIGHT 320

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
                halt = 2;
                break; 
            default:
                break;
            }
        }
    }
}

Uint32 renderScreen (Uint32 interval, void *param) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int r = 0; r < 32; r++) {
        for (int c = 0; c < 64; c++) {
            // printf("%c", gfx[r * 64 + c] ? 'O' : ' ');
            if (gfx[r * 64 + c]) {
                SDL_Rect rect = {c * 10, r * 10, 10, 10};
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        // printf("\n");
    }
    SDL_RenderPresent(renderer);
    printf("render\n");
    return interval;
}

void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
