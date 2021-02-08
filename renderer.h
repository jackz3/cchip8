#ifndef RENDERER
#define RENDERER

#include<SDL2/SDL.h>

void createWindow();
Uint32 renderScreen(Uint32 interval, void *param);
void destroy();
void procIO ();

#endif