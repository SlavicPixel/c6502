#include <stdlib.h>
#include <stdio.h>
#include "cpu_runner.h"

void load_test_program(Cpu *cpu, Bus *bus, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: file %s not found\n", filename);
        exit(1);
    }

    uint16_t start_address = 0x0000;

    uint8_t byte;
    while (fread(&byte, 1, 1, file)) {
        //printf("%d", byte);
        cpu->write(bus->ram, start_address++, byte);
    }

    fclose(file);

}

void run(Cpu *cpu, Bus *bus, int instruction_count)
{
    printf("\nCPU log (max %d lines): \n", instruction_count);
    do
    {
        cpu_clock(cpu, bus);
        printf("PC: %04X  OPCODE: %02X  A:%02X X:%02X Y:%02X  STATUS:%02X\n",
            cpu->pc, cpu->opcode, cpu->accumulator, cpu->x_register, cpu->y_register, cpu->status);
    } while (instruction_count--);

}

void print_disassembled(Line *disassembled)
{
    Line *entry, *tmp;

    printf("\nDisassembled code: \n");
    HASH_ITER(hh, disassembled, entry, tmp){
        printf("%s\n", entry->line);
    }

}
