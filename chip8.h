#ifndef CHIP8
#define CHIP8
#include<stdint.h>

#define NIBBLE1(b) (b >> 4)
#define NIBBLE2(b) (b & 0xf)
#define OPS_PS 500

static uint_fast8_t opcode;
uint8_t memory[4096];
uint8_t V[16];
uint16_t I;
uint16_t pc;
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
uint8_t gfx[64 * 32];
uint8_t delay_timer;
uint8_t sound_timer;

uint16_t stack[16];
uint16_t sp;
// the Chip 8 has a HEX based keypad (0x0-0xF)
uint8_t keypad[16];

uint_fast8_t halt;
uint8_t waitForKey;

int Counter;

void initialize();
size_t loadGame(char* fileName);
void emulateCycle();

#endif