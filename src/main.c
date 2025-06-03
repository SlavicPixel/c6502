#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "debugger_gui.h"
#include "cpu_runner.h"

void print_cpu(Cpu *cpu)
{
    printf("PC: %04X  OPCODE: %02X  A:%02X X:%02X Y:%02X  STATUS:%02X\n",
        cpu->pc, cpu->opcode, cpu->accumulator, cpu->x_register, cpu->y_register, cpu->status);
}


int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        printf("Error: binary file not found\n\nUsage: c6502 [FILE] [OPTIONS]\n");
        return 1;
    }

    const char *binary = argv[1];
    size_t len = strlen(binary);

    if (len < 4 || strcmp(&binary[len - 4], ".bin") != 0) 
    {
        printf("Error: invalid file\n");
        return 1;
    }

    Bus *bus = malloc(sizeof(Bus));
    Cpu *cpu = initialize_cpu();

    load_test_program(cpu, bus, binary);
    reset(cpu, bus);
    cpu->pc = 0x0400;

    Line *disassembled = disassembler(bus, 0x0400, 0x0423);

    if (argc > 2 && strcmp(argv[2], "--gui") == 0) {
        gui_debugger(cpu, bus, cpu->pc);
    } else {
        print_disassembled(disassembled);
        run(cpu, bus, 50);
    }

    free(cpu);
    free(bus);

    return 0;
}
