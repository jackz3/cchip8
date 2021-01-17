#ifndef RENDERER
#define RENDERER

#include<SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

void createWindow();
void renderScreen ();
void destroy();
void procIO ();

#endif