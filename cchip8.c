#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include "chip8.h"
#include "renderer.h"
#include<SDL2/SDL.h>


int main (int argc, char**argv) {
    if (argc < 2) {
		printf("usage: %s [game]", argv[0]);
        exit(EXIT_FAILURE);
    }
    createWindow();
    initialize();

    // char file[50] = "./roms/";
    // strcat(file, argv[1]);
    // strcat(file, ".ch8");
    loadGame(argv[1]);
    // size_t len = loadGame("./roms/IBM Logo.ch8");
    // size_t len = loadGame("Fishie.ch8");
    // size_t len = loadGame("Chip8 Picture.ch8");
    // size_t len = loadGame("./roms/Maze.ch8");
    // size_t len = loadGame("./roms/test_opcode.ch8");
    // size_t len = loadGame("./roms/Keypad Test [Hap, 2006].ch8");
    // size_t len = loadGame("./roms/random_number_test.ch8");
    // size_t len = loadGame("./roms/delay_timer_test.ch8");
    // size_t len = loadGame("./roms/snake.ch8");
    // size_t len = loadGame("./roms/slipperyslope.ch8");
    // size_t len = loadGame("./roms/chipwar.ch8");
    // size_t len = loadGame("./roms/rockto.ch8"); //schip
    // printf("rom size:%lld\n", len);
    const uint8_t cycles = OPS_PS / 60;
    int running = 1;
    // SDL_TimerID my_timer_id = SDL_AddTimer(1000 / 60, renderScreen, gfx);
    Uint32 lastTime = SDL_GetTicks(), currentTime;
    while (running) {
        procIO(&running);
        if (halt) {
            continue;
        }
        emulateCycle(cycles);
        // Counter--;
        currentTime = SDL_GetTicks();
        int delta = 17 - (currentTime - lastTime);
        if (delta> 0) {
            SDL_Delay(delta);
        }
        renderScreen();
        // lastTime = SDL_GetTicks();
        lastTime = currentTime + delta;
    }
    destroy();
    return 0;
}
