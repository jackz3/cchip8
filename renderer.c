#include<stdint.h>
#include<stdio.h>
#include<SDL2/SDL.h>


void createWindow () {

}

void renderScreen (const uint8_t *gfx) {
    for (int r = 0; r < 32; r++) {
        for (int c = 0; c < 64; c++) {
            printf("%c", gfx[r * 64 + c] ? 'O' : ' ');
        }
        printf("\n");
    }
}