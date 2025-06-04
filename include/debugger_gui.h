#ifndef DEBUG_GUI
#define DEBUG_GUI

#include "cpu.h"

void step_instruction(Cpu *cpu, Bus *bus);
void gui_debugger(Cpu *cpu, Bus *bus,uint16_t start_address, Line *first_entry);

#endif
