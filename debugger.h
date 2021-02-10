#ifndef DEBUGGER
#define DEBUGGER

void printMem ();
void printReg ();
void disasm(const uint8_t *mem, const int pc);

#endif