#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "chip8.h"

// #define NIBBLE1(b) (b >> 4)
// #define NIBBLE2(b) (b & 0xf)

static void printUnknown (uint_fast8_t byte1, uint_fast8_t byte2) {
  printf ("Unknown opcode: 0x%02X %02X", byte1, byte2);
  exit(1);
}

void initialize() {
  pc     = 0x200;
  opcode = 0;
  I      = 0;
  sp     = 0xfa0;
 
  // Clear display	
  // Clear stack
  // Clear registers V0-VF
  // Clear memory
 
  // Load fontset
  uint_fast8_t chip8_fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  for(int i = 0; i < 80; ++i)
    memory[i] = chip8_fontset[i];		
 
  // Reset timers
  halt = 0;
}

size_t loadGameFromFile (FILE *file) {
    //Get the file size    
    fseek(file, 0L, SEEK_END);    
    int fsize = ftell(file);    
    fseek(file, 0L, SEEK_SET);    
    //Read the file into memory at 0x200 and close it. 
    unsigned char *buffer = memory + 0x200;
    return fread(buffer, 1, fsize, file);    
}

size_t loadGame(char* fileName) {
  FILE *f= fopen(fileName, "rb");    
  if (f==NULL)    
  {    
      printf("error: Couldn't open %s\n", fileName);
      exit(1);
  }
  return loadGameFromFile(f);
}

void emulateCycle () {
  // Fetch opcode
  // opcode = memory[pc] << 8 | memory[pc + 1];
  uint_fast8_t byte1 = memory[pc++];
  uint_fast8_t byte2 = memory[pc++];
  opcode = NIBBLE1(byte1);
  switch(opcode) {
    case 0x0:
      switch (byte2)
      {
        case 0xe0:
          memset(gfx, 0, 64*32);
          break;
        case 0xee:
				  pc = (memory[sp] << 8) | memory[sp + 1];
          sp += 2;
          break;
        default:
          printUnknown(byte1, byte2);
      }
      break;
    case 0x1: // 1NNN	Flow	goto NNN;	Jumps to address NNN.
      {
        uint16_t target = (NIBBLE2(byte1) << 8) | byte2;
        if (target == (pc - 2)) {
          halt = 1;
          break;
        }
        pc = target;
      }
      break;
    case 0x2:
      sp -= 2;
      memory[sp] = pc >> 8; // (pc & 0xFF00) >> 8;
      memory[sp + 1] = pc & 0xFF;
	    pc = (NIBBLE2(byte1) << 8) | byte2;
      break;
    case 0x3:
      if (V[NIBBLE2(byte1)] == byte2)
        pc += 2;
      break;
    case 0x4:
      if (V[NIBBLE2(byte1)] != byte2)
        pc += 2;
      break;
    case 0x5:
      if (V[NIBBLE2(byte1)] == V[NIBBLE1(byte2)])
        pc += 2;
      break;
    case 0x6:
      V[NIBBLE2(byte1)] = byte2;
      break;
    case 0x7:
      V[NIBBLE2(byte1)] += byte2;
      break;
    case 0x8:
    {
      uint_fast8_t regX = NIBBLE2(byte1);
      uint_fast8_t regY = NIBBLE1(byte2);
      switch (NIBBLE2(byte2))
      {
      case 6:					//SHR
			{
        // (Optional, or configurable) Set VX to the value of VY
				uint8_t vf = V[regX] & 0x1;
				V[regX] = V[regX] >> 1;
				V[0xf] = vf;
			}
			  break; 
      case 0xe:
      {
        // uint8_t vf = (0x80 == (V[regX] & 0x80));
        uint8_t vf = (V[regX] >> 7);
				V[regX] <<= 1;
				V[0xf] = vf;
        break;
      }
      default:
        printUnknown(byte1, byte2);
        break;
      }
      break;
    }
    case 0x9:
      if (V[NIBBLE2(byte1)] != V[NIBBLE1(byte2)])
        pc += 2;
      break;
    case 0xA: // ANNN: Sets I to the address NNN
      I = (NIBBLE2(byte1) << 8) | byte2;
      break;
    case 0xc:
	    V[NIBBLE2(byte1)] = random() & byte2;
      break;
    case 0xd:
      {
        uint_fast8_t lines = NIBBLE2(byte2);
        uint_fast8_t x = V[NIBBLE2(byte1)];
        uint_fast8_t y = V[NIBBLE1(byte2)];	
        V[0xF] = 0;
        int i,j;
        for (i=0; i < lines; i++) {
          uint_fast8_t sprite = memory[I + i];
          for (j=x; j<(x+8) && j<64; j++) {
            uint_fast8_t srcbit = sprite & 0x80;
            if (srcbit) {
                uint8_t *destbyte_p = &gfx[(i + y) * (64) + j];
                uint8_t destbyte = *destbyte_p;
                
                if (1 & destbyte)
                  V[0xf] = 1;
                
                destbyte ^= 1;
                *destbyte_p = destbyte;
            }
            sprite <<= 1;
          }
        }
      }
      break;
    case 0xf:
    {
      uint_fast8_t reg = NIBBLE2(byte1);
      switch (byte2)
      {
        case 0x0a:
        {
          waitForKey = 1;
          // if (state->waiting_for_key == 0)
          // {
          //     //start the key_wait_cycle
          //     memcpy(&state->save_key_state, &state->key_state, 16);
          //     state->waiting_for_key = 1;
          //     //do not advance PC
          //     return;
          // }
          // else
          // {
          //     int i;
          //     //look for a change in key_state from 0 to 1
          //     for (i = 0; i < 16; i++)
          //     {
          //         if ((state->save_key_state[i] == 0) && (state->key_state[i] == 1))
          //         {
          //             state->waiting_for_key = 0;
          //             state->V[reg] = i;
          //             state->PC += 2;
          //             return;
          //         }
          //         //by copying the key state, this allows detection of a key that 
          //         // started pressed, then got released, then was pressed again
          //         state->save_key_state[i] = state->key_state[i];
          //     }
          //     //do not advance the PC
          //     return;
          // }
        break;
      }
      case 0x1e:
        I += V[reg];
        break;
      case 0x29:
        I = V[reg] * 5;
        break;
      case 0x55:
      {
				for (int i=0; i <= reg; i++)
					memory[I + i] = V[i];
				I += (reg + 1); //???
        break;
      }
      case 0x65:
      {
				for (int i=0; i <= reg; i++)
					V[i] = memory[I + i];
				I += (reg + 1);
        break;
      }
      default:
        printUnknown(byte1, byte2);
      }
      break;
    }
    default:
      printUnknown(byte1, byte2);
  }  
 
  // Update timers
  if(delay_timer > 0)
    --delay_timer;
 
  if(sound_timer > 0)
  {
    if(sound_timer == 1)
      printf("BEEP!\n");
    --sound_timer;
  }  
}