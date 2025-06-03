#ifndef BUS_H
#define BUS_H
#define RAM_SIZE (64*1024)

#include <stdint.h> // renames standard types into explicit types
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    uint8_t ram[RAM_SIZE];
} Bus;

void clear_ram(uint8_t ram[]);
void bus_write(uint8_t ram[], uint16_t address, uint8_t data);
uint8_t bus_read(uint8_t ram[], uint16_t address, bool bReadOnly); // bReadOnly = false

#endif
