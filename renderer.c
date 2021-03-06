#include<stdint.h>
#include<stdio.h>
#include<SDL2/SDL.h>
#include "renderer.h"
#include "chip8.h"

#define WIDTH 640
#define HEIGHT 320

static SDL_Window *window;
static SDL_Renderer *renderer;

void createWindow () {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("sdl init error: %s\n", SDL_GetError());
    }
    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
    }
}

void procIO (int* running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        if (event.type == SDL_QUIT){
            // halt = 2;
            *running = 0;
        }
        if (event.type == SDL_KEYDOWN){
            switch (event.key.keysym.sym) {
            case SDLK_0:
                keypad[0] = 1;
                break;
            case SDLK_1:
                keypad[1] = 1;
                break;
            case SDLK_2:
                keypad[2] = 1;
                break;
            case SDLK_3:
                keypad[3] = 1;
                break;
            case SDLK_4:
                keypad[4] = 1;
                break;
            case SDLK_5:
            case SDLK_UP:
                keypad[5] = 1;
                break;
            case SDLK_6:
                keypad[6] = 1;
                break;
            case SDLK_7:
            case SDLK_LEFT:
                keypad[7] = 1;
                break;
            case SDLK_8:
            case SDLK_DOWN:
                keypad[8] = 1;
                break;
            case SDLK_9:
            case SDLK_RIGHT:
                keypad[9] = 1;
                break;
            case SDLK_a:
                keypad[0xa] = 1;
                break;
            case SDLK_b:
                keypad[0xb] = 1;
                break;
            case SDLK_c:
                keypad[0xc] = 1;
                break;
            case SDLK_d:
                keypad[0xd] = 1;
                break;
            case SDLK_e:
                keypad[0xe] = 1;
                break;
            case SDLK_f:
                keypad[0xf] = 1;
                break;
            case SDLK_ESCAPE:
                // halt = 2;
                *running = 0;
                break; 
            default:
                return;
            }
            // if (waitForKey == 1) {
            //     waitForKey = 2;
            // }
        }
        if (event.type == SDL_KEYUP) {
            switch (event.key.keysym.sym) {
            case SDLK_0:
                keypad[0] = 0;
                break;
            case SDLK_1:
                keypad[1] = 0;
                break;
            case SDLK_2:
                keypad[2] = 0;
                break;
            case SDLK_3:
                keypad[3] = 0;
                break;
            case SDLK_4:
                keypad[4] = 0;
                break;
            case SDLK_5:
            case SDLK_UP:
                keypad[5] = 0;
                break;
            case SDLK_6:
                keypad[6] = 0;
                break;
            case SDLK_7:
            case SDLK_LEFT:
                keypad[7] = 0;
                break;
            case SDLK_8:
            case SDLK_DOWN:
                keypad[8] = 0;
                break;
            case SDLK_9:
            case SDLK_RIGHT:
                keypad[9] = 0;
                break;
            case SDLK_a:
                keypad[0xa] = 0;
                break;
            case SDLK_b:
                keypad[0xb] = 0;
                break;
            case SDLK_c:
                keypad[0xc] = 0;
                break;
            case SDLK_d:
                keypad[0xd] = 0;
                break;
            case SDLK_e:
                keypad[0xe] = 0;
                break;
            case SDLK_f:
                keypad[0xf] = 0;
                break;
            }
        }
    }
}

// Uint32 renderScreen (Uint32 interval, void *param) {
void renderScreen () {
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
    // printf("render\n");
    if (delay_timer > 0) {
        delay_timer--;
    }
    if(sound_timer > 0) {
        if(sound_timer == 1)
            printf("BEEP!\n");
        --sound_timer;
    }
}

void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
