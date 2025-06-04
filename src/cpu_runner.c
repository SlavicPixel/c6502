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

Line *find_first_entry(Line *disassembled, uint16_t start_address)
{
    Line *entry = NULL;
    HASH_FIND(hh, disassembled, &start_address, sizeof(uint16_t), entry);
    if (!entry)
    {
        printf("Start address not found in disassembly.\n");
        return NULL;
    }
    return entry;
}

void print_disassembled(Line *disassembled, uint16_t start_address)
{
    printf("\nDisassembled code:\n");

    Line *entry = find_first_entry(disassembled, start_address);

    if (!entry)
        return;

    int brk_count = 0;
    Line *tmp;
    for (tmp = entry; tmp != NULL; tmp = tmp->hh.next)
    {
        printf("%s\n", tmp->line);

        if (strstr(tmp->line, "BRK  {IMP}"))
        {
            brk_count++;
            if (brk_count == 3)
                break;
        }
        else
        {
            brk_count = 0;
        }
    }
}


