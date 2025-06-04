#ifndef CPU_RUNNER
#define CPU_RUNNER

#include "cpu.h"

void load_test_program(Cpu *cpu, Bus *bus, const char *filename);
void run(Cpu *cpu, Bus *bus, int instruction_count);
Line *find_first_entry(Line *disassembled, uint16_t start_address);
void print_disassembled(Line *disassembled, uint16_t start_address);

#endif
