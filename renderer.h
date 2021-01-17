#ifndef RENDERER
#define RENDERER

#include<SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

void createWindow();
Uint32 renderScreen (Uint32 interval, void *gfx);
void destroy();
void procIO ();

#endif