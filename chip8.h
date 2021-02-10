#ifndef CHIP8
#define CHIP8
#include<stdint.h>

#define NIBBLE1(b) (b >> 4)
#define NIBBLE2(b) (b & 0xf)
#define OPS_PS 600

extern uint8_t memory[4096];
extern uint8_t V[16];
extern uint16_t I;
extern uint16_t pc;
// 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
// 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
// 0x200-0xFFF - Program ROM and work RAM
extern uint8_t gfx[64 * 32];
extern uint8_t delay_timer;
extern uint8_t sound_timer;

extern uint16_t stack[16];
extern uint16_t sp;
// the Chip 8 has a HEX based keypad (0x0-0xF)
extern uint8_t keypad[16];

extern uint_fast8_t halt;
extern uint8_t waitForKey;

extern int Counter;

void initialize();
size_t loadGame(const char* fileName);
void emulateCycle(uint8_t cycles);

#endif