#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "chip8.h"
#include "debugger.h"
#include "renderer.h"

unsigned char *readFile (FILE *file) {
    //Get the file size    
    fseek(file, 0L, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);
    //Read the file into memory at 0x200 and close it.
    unsigned char *buffer=malloc(fsize);
    fread(buffer, fsize, 1, file);
    return buffer;
}

void disassemble (const char* fileName) {
    // argv[1]
    FILE *f= fopen(fileName, "rb");    
    if (f==NULL)
    {
        printf("error: Couldn't open %s\n", fileName);
        exit(1);
    }

    //Get the file size
    fseek(f, 0L, SEEK_END);    
    int fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);

    //CHIP-8 convention puts programs in memory at 0x200    
    unsigned char *buffer=malloc(fsize+0x200);    
    fread(buffer+0x200, fsize, 1, f);

    unsigned char *buffer2 = readFile(f);

    fclose(f);

    int pc = 0x200;
    int i = 0;
}

int main (int argc, char**argv) {
    createWindow();
    initialize();
    // size_t len = loadGame("IBM Logo.ch8");
    // size_t len = loadGame("Fishie.ch8");
    // size_t len = loadGame("Chip8 Picture.ch8");
    // size_t len = loadGame("Maze.ch8");
    size_t len = loadGame("Keypad Test [Hap, 2006].ch8");
    // size_t len = loadGame("./roms/random_number_test.ch8");
    // size_t len = loadGame("test_opcode.ch8");
    printf("rom size:%ld\n", len);
    int i = 0;
    while (1) {
        disasm(memory, pc);
        emulateCycle();
        if (waitForKey) {
            printf("wait for key press\n");
            break;
        }
        if (halt) {
            renderScreen(gfx);
            break;
        }
        if (i % 6 == 5) {
            renderScreen(gfx);
        }
        i++;
        procIO();
    }
    destroy();
    return 0;
}
