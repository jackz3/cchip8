#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include "chip8.h"
#include "debugger.h"
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
    // size_t len = loadGame("./roms/random_number_test.ch8");
    // size_t len = loadGame("./roms/Keypad Test [Hap, 2006].ch8");
    // size_t len = loadGame("./roms/delay_timer_test.ch8");
    // size_t len = loadGame("./roms/snake.ch8");
    // size_t len = loadGame("./roms/rockto.ch8");
    // size_t len = loadGame("./roms/chipwar.ch8");
    // size_t len = loadGame("./roms/slipperyslope.ch8");
    // printf("rom size:%lld\n", len);
    const int InterruptPeriod = OPS_PS / 10;
    Counter = InterruptPeriod;
    SDL_TimerID my_timer_id = SDL_AddTimer(1000 / 60, renderScreen, gfx);
    unsigned int lastTime = SDL_GetTicks(), currentTime;
    while (1) {
        procIO();
        if (Counter <= 0) {
            if (halt == 2) {
                break;
            }
            if (halt) {
                continue;
            }
            if (waitForKey == 1) {
                // printf("wait for key press\n");
                continue;
            }
            if (waitForKey == 2) {
                printf("wait 2\n");
                lastTime = SDL_GetTicks();
                // waitForKey = 0;
            } else {
                printf("wait 0\n");
                currentTime = SDL_GetTicks();
                int diff = 100 - (currentTime - lastTime);
                if (diff > 0) {
                    SDL_Delay(diff);
                }
                lastTime = currentTime + diff;
            }
            Counter = InterruptPeriod;
        }
        disasm(memory, pc);
        emulateCycle();
        Counter--;
    }
    destroy();
    return 0;
}
