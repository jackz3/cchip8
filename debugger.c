#include<stdio.h>
#include<stdlib.h>
#include "chip8.h"
#include "debugger.h"

void printMem () {
  printf("PC=%02x$", pc);
  for (int i = 0; i < 2; i += 2) {
    printf("%02x%02x ", memory[pc + i], memory[pc + i]);
  }
  printf("I=%02x$", I);
  for (int i = 0; i < 2; i += 1) {
    printf("%02x%02x ", memory[I + i], memory[I + i + 1]);
  }
  printReg();
  printf("\n");
}

void printReg () {
  for (int i = 0; i < 16; i++) {
    if (V[i]) {
      printf("V%01X=%02x ", i, V[i]);
    }
  }
  printf("SP=$%04x", sp);
}

void printUnknown (uint_fast8_t byte1, uint_fast8_t byte2) {
  printf ("Unknown opcode: 0x%02X %02X", byte1, byte2);
  exit(1);
}

void disasm(const uint8_t *mem, int pc) {
  printMem();
  uint_fast8_t byte1 = memory[pc];
  uint_fast8_t byte2 = memory[pc + 1];
  uint_fast8_t opcode = NIBBLE1(byte1);
  switch(opcode) {
    case 0x0:
      switch (byte2)
      {
        case 0xe0:
          printf("%-10s", "CLS");
          break;
        case 0xee:
          printf("%-10s", "RET ???");
          break;
        default:
          printUnknown(byte1, byte2);
      }
      break;
    case 0x1: // 1NNN	Flow	goto NNN;	Jumps to address NNN.
      {
        uint16_t target = (NIBBLE2(byte1) << 8) | byte2;
        printf("%-10s $%02x%02x ", "Goto", NIBBLE2(byte1), byte2);
        if (target == pc) {
          printf("Infinite loop detected. halt");
        }
      }
      break;
    case 0x2:
      printf("%-10s $%04x", "Call", ((NIBBLE2(byte1) << 8) | byte2));
      break;
    case 0x3:
      printf("If V%01X==#%02x Skip", NIBBLE2(byte1), byte2);
      break;
    case 0x4:
      printf("If V%01X!=#%02x Skip", NIBBLE2(byte1), byte2);
      break;
    case 0x5:
      printf("If V%01X==V%01X Skip", NIBBLE2(byte1), NIBBLE1(byte2));
      break;
    case 0x6:
      printf("V%01X = #%02x", NIBBLE2(byte1), byte2);
      break;
    case 0x7:
      printf("V%01X += #%02x", NIBBLE2(byte1), byte2);
      break;
    case 0x8:
    {
      uint_fast8_t regX = NIBBLE2(byte1);
      uint_fast8_t regY = NIBBLE1(byte2);
      switch (NIBBLE2(byte2)) {
      case 6:
        printf("V%01X >>= 1", regX);
			  break; 
      case 0xe:
        printf("V%01X <<= 1", regX);
        break;
      default:
        printUnknown(byte1, byte2);
        break;
      }
      break;
    }
    case 0x9:
      printf("If V%01X!=V%01X Skip", NIBBLE2(byte1), NIBBLE1(byte2));
      break;
    case 0xA: // ANNN: Sets I to the address NNN
      printf("I = $%02x%02x",  NIBBLE2(byte1), byte2);    
      break;
    case 0xc:
      printf("V%01X = RND & #%02x", NIBBLE2(byte1), byte2);
      break;
    case 0xd:
      printf("Draw V%01X,V%01X,#%01x", NIBBLE2(byte1), NIBBLE1(byte2), NIBBLE2(byte2));
      break;
    case 0xf:
    {
      uint_fast8_t reg = NIBBLE2(byte1);
      switch (byte2) {
      case 0x0a:
        printf("V%01X = get_key()", reg);
        break;
      case 0x1e:
        printf("I += V%01X", reg); 
        break;
      case 0x29:
        printf("I = sprite_addr[V%01X]", reg);
        break;
      case 0x55:
        printf("Reg_dump(V%01X, &I)", reg);
        break;
      case 0x65:
        printf("Reg_load(V%01X, &I)", reg);
        break;
      default:
        printUnknown(byte1, byte2);
      }
      break;
    }
    default:
      printUnknown(byte1, byte2);
  }  
  printf("\n");
}