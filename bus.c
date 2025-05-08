#include "bus.h"
#include <string.h>

void clear_ram(uint8_t ram[])
{
    memset(ram, 0, RAM_SIZE);
}

void bus_write(uint8_t ram[], uint16_t address, uint8_t data)
{
    if (address >= 0x0000 && address <= 0xFFFF)
        ram[address] = data;
}

uint8_t bus_read(uint8_t ram[], uint16_t address, bool bReadOnly)
{
    if (address >= 0x0000 && address <= 0xFFFF)
        return ram[address];
    
    return 0x00;
}