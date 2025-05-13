#include "cpu.h"

void write(uint8_t ram[], uint16_t address, uint8_t data)
{
    bus_write(ram, address, data);
}

uint8_t read(uint8_t ram[], uint16_t address)
{
    return bus_read(ram, address, 0);
}

Cpu *initialize_cpu()
{
    Cpu *cpu = malloc(sizeof(Cpu));

    cpu->write = write;
    cpu->read = read;

    cpu->accumulator = 0x00;
    cpu->x_register = 0x00;
    cpu->y_register = 0x00;
    cpu->stack_pointer = 0x00;
    cpu->pc = 0x0000; // 16-bit
    cpu->status = 0x00;

    cpu->fetched_data = 0x00;
    cpu->address_abs = 0x0000;
    cpu->address_rel = 0x0000;
    cpu->opcode = 0x00;
    cpu->cycles = 0;

    return cpu;
}

const Instruction opcode_lookup[OPCODE_COUNT] = {
    {"BRK", BRK, IMP, 7}, {"ORA", ORA, INX, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 3}, {"ORA", ORA, ZP0, 3}, {"ASL", ASL, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"PHP", PHP, IMP, 3}, {"ORA", ORA, IMM, 2}, {"ASL", ASL, IMP, 2}, {"XXX", XXX, IMP, 2}, {"XXX", NOP, IMP, 4}, {"ORA", ORA, ABS, 4}, {"ASL", ASL, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BPL", BPL, REL, 2}, {"ORA", ORA, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"ORA", ORA, ZPX, 4}, {"ASL", ASL, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"CLC", CLC, IMP, 2}, {"ORA", ORA, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"ORA", ORA, ABX, 4}, {"ASL", ASL, ABX, 7}, {"XXX", XXX, IMP, 7},
    {"JSR", JSR, ABS, 6}, {"AND", AND, INX, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"BIT", BIT, ZP0, 3}, {"AND", AND, ZP0, 3}, {"ROL", ROL, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"PLP", PLP, IMP, 4}, {"AND", AND, IMM, 2}, {"ROL", ROL, IMP, 2}, {"XXX", XXX, IMP, 2}, {"BIT", BIT, ABS, 4}, {"AND", AND, ABS, 4}, {"ROL", ROL, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BMI", BMI, REL, 2}, {"AND", AND, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"AND", AND, ZPX, 4}, {"ROL", ROL, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"SEC", SEC, IMP, 2}, {"AND", AND, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"AND", AND, ABX, 4}, {"ROL", ROL, ABX, 7}, {"XXX", XXX, IMP, 7},
    {"RTI", RTI, IMP, 6}, {"EOR", EOR, INX, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 3}, {"EOR", EOR, ZP0, 3}, {"LSR", LSR, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"PHA", PHA, IMP, 3}, {"EOR", EOR, IMM, 2}, {"LSR", LSR, IMP, 2}, {"XXX", XXX, IMP, 2}, {"JMP", JMP, ABS, 3}, {"EOR", EOR, ABS, 4}, {"LSR", LSR, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BVC", BVC, REL, 2}, {"EOR", EOR, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"EOR", EOR, ZPX, 4}, {"LSR", LSR, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"CLI", CLI, IMP, 2}, {"EOR", EOR, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"EOR", EOR, ABX, 4}, {"LSR", LSR, ABX, 7}, {"XXX", XXX, IMP, 7},
    {"RTS", RTS, IMP, 6}, {"ADC", ADC, INX, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 3}, {"ADC", ADC, ZP0, 3}, {"ROR", ROR, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"PLA", PLA, IMP, 4}, {"ADC", ADC, IMM, 2}, {"ROR", ROR, IMP, 2}, {"XXX", XXX, IMP, 2}, {"JMP", JMP, IND, 5}, {"ADC", ADC, ABS, 4}, {"ROR", ROR, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BVS", BVS, REL, 2}, {"ADC", ADC, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"ADC", ADC, ZPX, 4}, {"ROR", ROR, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"SEI", SEI, IMP, 2}, {"ADC", ADC, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"ADC", ADC, ABX, 4}, {"ROR", ROR, ABX, 7}, {"XXX", XXX, IMP, 7},
    {"XXX", NOP, IMP, 2}, {"STA", STA, INX, 6}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 6}, {"STY", STY, ZP0, 3}, {"STA", STA, ZP0, 3}, {"STX", STX, ZP0, 3}, {"XXX", XXX, IMP, 3}, {"DEY", DEY, IMP, 2}, {"XXX", NOP, IMP, 2}, {"TXA", TXA, IMP, 2}, {"XXX", XXX, IMP, 2}, {"STY", STY, ABS, 4}, {"STA", STA, ABS, 4}, {"STX", STX, ABS, 4}, {"XXX", XXX, IMP, 4},
    {"BCC", BCC, REL, 2}, {"STA", STA, INY, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 6}, {"STY", STY, ZPX, 4}, {"STA", STA, ZPX, 4}, {"STX", STX, ZPY, 4}, {"XXX", XXX, IMP, 4}, {"TYA", TYA, IMP, 2}, {"STA", STA, ABY, 5}, {"TXS", TXS, IMP, 2}, {"XXX", XXX, IMP, 5}, {"XXX", XXX, IMP, 5}, {"STA", STA, ABX, 5}, {"XXX", XXX, IMP, 5}, {"XXX", XXX, IMP, 5},
    {"LDY", LDY, IMM, 2}, {"LDA", LDA, INX, 6}, {"LDX", LDX, IMM, 2}, {"XXX", XXX, IMP, 6}, {"LDY", LDY, ZP0, 3}, {"LDA", LDA, ZP0, 3}, {"LDX", LDX, ZP0, 3}, {"XXX", XXX, IMP, 3}, {"TAY", TAY, IMP, 2}, {"LDA", LDA, IMM, 2}, {"TAX", TAX, IMP, 2}, {"XXX", XXX, IMP, 2}, {"LDY", LDY, ABS, 4}, {"LDA", LDA, ABS, 4}, {"LDX", LDX, ABS, 4}, {"XXX", XXX, IMP, 4},
    {"BCS", BCS, REL, 2}, {"LDA", LDA, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 5}, {"LDY", LDY, ZPX, 4}, {"LDA", LDA, ZPX, 4}, {"LDX", LDX, ZPY, 4}, {"XXX", XXX, IMP, 4}, {"CLV", CLV, IMP, 2}, {"LDA", LDA, ABY, 4}, {"TSX", TSX, IMP, 2}, {"XXX", XXX, IMP, 4}, {"LDY", LDY, ABX, 4}, {"LDA", LDA, ABX, 4}, {"LDX", LDX, ABY, 4}, {"XXX", XXX, IMP, 4},
    {"CPY", CPY, IMM, 2}, {"CMP", CMP, INX, 6}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 8}, {"CPY", CPY, ZP0, 3}, {"CMP", CMP, ZP0, 3}, {"DEC", DEC, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"INY", INY, IMP, 2}, {"CMP", CMP, IMP, 2}, {"DEX", DEX, IMP, 2}, {"XXX", XXX, IMP, 2}, {"CPY", CPY, ABS, 4}, {"CMP", CMP, ABS, 4}, {"DEC", DEC, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BNE", BNE, REL, 2}, {"CMP", CMP, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"CMP", CMP, ZPX, 4}, {"DEC", DEC, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"CLD", CLD, IMP, 2}, {"CMP", CMP, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"CMP", CMP, ABX, 4}, {"DEC", DEC, ABX, 7}, {"XXX", XXX, IMP, 7},
    {"CPX", CPX, IMM, 2}, {"SBC", SBC, INX, 6}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"CPX", CPX, ZP0, 3}, {"SBC", SBC, ZP0, 3}, {"INC", INC, ZP0, 5}, {"XXX", XXX, IMP, 5}, {"INX", INX, IMP, 2}, {"SBC", SBC, IMM, 2}, {"NOP", NOP, IMP, 2}, {"XXX", NOP, IMP, 2}, {"CPX", CPX, ABS, 4}, {"SBC", SBC, ABS, 4}, {"INC", INC, ABS, 6}, {"XXX", XXX, IMP, 6},
    {"BEQ", BEQ, REL, 2}, {"SBC", SBC, INY, 5}, {"XXX", XXX, IMP, 2}, {"XXX", XXX, IMP, 8}, {"XXX", NOP, IMP, 4}, {"SBC", SBC, ZPX, 4}, {"INC", INC, ZPX, 6}, {"XXX", XXX, IMP, 6}, {"SED", SED, IMP, 2}, {"SBC", SBC, ABY, 4}, {"XXX", NOP, IMP, 2}, {"XXX", XXX, IMP, 7}, {"XXX", NOP, IMP, 4}, {"SBC", SBC, ABX, 4}, {"INC", INC, ABX, 7}, {"XXX", XXX, IMP, 7}
};

void clock(Cpu *cpu, Bus *bus)
{
    if (cpu->cycles == 0)
    {
        cpu->opcode = cpu->read(bus->ram, cpu->pc);
        cpu->pc++;
    
        cpu->cycles = opcode_lookup[cpu->opcode].cycles;
    
        uint8_t address_additional_cycle = opcode_lookup[cpu->opcode].address_mode(cpu, bus);
        uint8_t operation_additional_cycle = opcode_lookup[cpu->opcode].operation(cpu, bus);

        cpu->cycles += (address_additional_cycle & operation_additional_cycle);
    }
    cpu->cycles--;
}

uint8_t get_flag(Cpu *cpu, Flags flag)
{
    return ((cpu->status & flag) > 0) ? 1 : 0;
}

void set_flag(Cpu *cpu, Flags flag, bool value)
{
    if (value)
        cpu->status |= flag;
    else
        cpu->status &= ~flag;
}

void reset(Cpu *cpu, Bus *bus)
{
    cpu->accumulator = 0x00;
    cpu->x_register = 0x00;
    cpu->y_register = 0x00;
    cpu->stack_pointer = 0xFD;
    cpu->status = 0x00 | U;

    cpu->address_abs = 0xFFFC; // Provided by the program, 0xFFFC stores an address of the first address needed by the program counter
    uint16_t low_byte = cpu->read(bus->ram, cpu->address_abs + 0);
    uint16_t high_byte = cpu->read(bus->ram, cpu->address_abs + 1);
    cpu->pc = (high_byte << 8) | low_byte;

    cpu->fetched_data = 0x00;
    cpu->address_abs = 0x0000;
    cpu->address_rel = 0x0000;

    cpu->cycles = 8; // Reset takes 8 clock cycles
}

void irq(Cpu *cpu, Bus *bus)
{
    if (get_flag(cpu, I) == 0) // Checking Disable Interrupt flag
    {
        cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, (cpu->pc >> 8) & 0x00FF);
        cpu->stack_pointer--;
        cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->pc & 0x00FF);
        cpu->stack_pointer--;

        set_flag(cpu, B, 0);
        set_flag(cpu, U, 1);
        set_flag(cpu, I, 1);
        
        cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->status);
        cpu->stack_pointer--;

        cpu->address_abs = 0xFFFE; // Provided by the program, 0xFFFE stores a value for the new program counter
        uint16_t low_byte = cpu->read(bus->ram, cpu->address_abs + 0);
        uint16_t high_byte = cpu->read(bus->ram, cpu->address_abs + 1);
        cpu->pc = (high_byte << 8) | low_byte;

        cpu->cycles = 7; // Interrupt takes 7 clock cycles
    }
}

void nmi(Cpu *cpu, Bus *bus)
{
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, (cpu->pc >> 8) & 0x00FF);
    cpu->stack_pointer--;
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->pc & 0x00FF);
    cpu->stack_pointer--;

    set_flag(cpu, B, 0);
    set_flag(cpu, U, 1);
    set_flag(cpu, I, 1);
    
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->status);
    cpu->stack_pointer--;

    cpu->address_abs = 0xFFFE; // Provided by the program, 0xFFFE stores a value for the new program counter
    uint16_t low_byte = cpu->read(bus->ram, cpu->address_abs + 0);
    uint16_t high_byte = cpu->read(bus->ram, cpu->address_abs + 1);
    cpu->pc = (high_byte << 8) | low_byte;

    cpu->cycles = 8; // Non-maskable interrupt takes 8 clock cycles
}

uint8_t fetch_data(Cpu *cpu, Bus *bus)
{
    if (!(opcode_lookup[cpu->opcode].address_mode == IMP)) // There is no data to fetch in Implied addressing mode
        cpu->fetched_data = cpu->read(bus->ram, cpu->address_abs);
    return cpu->fetched_data;
}


// Addressing modes

// Implied
// No data in the instruction
// Some instruction need accumulator 
uint8_t IMP(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->fetched_data = cpu->accumulator;
    return 0;
}

// Immediate
// Data is supplied by the instruction
// Data gets put in the address absolute, 
// this way the instruction knows where to read the data when it needs to
uint8_t IMM(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->address_abs = cpu->pc++;
    return 0;
}

// Zero Page Addressing
// Byte of data the instruction needs is in the zero page of memory
uint8_t ZP0(Cpu *cpu, Bus *bus)
{
    cpu->address_abs = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;
    cpu->address_abs &= 0x00FF; // reading only the low byte of zero page address
    return 0;
}

// Zero Page Adressing with X register offset
uint8_t ZPX(Cpu *cpu, Bus *bus)
{
    cpu->address_abs = (cpu->read(bus->ram, cpu->pc) + cpu->x_register);
    cpu->pc++;
    cpu->address_abs &= 0x00FF;
    return 0;
}

// Zero Page Adressing with Y register offset
uint8_t ZPY(Cpu *cpu, Bus *bus)
{
    cpu->address_abs = (cpu->read(bus->ram, cpu->pc) + cpu->y_register);
    cpu->pc++;
    cpu->address_abs &= 0x00FF;
    return 0;
}

// Absolute mode addressing
// Needed when specifying the full address range
// Has to check the high byte for page address (unlike ZP mode),
// and low byte for page offset
// 6502 is little-endian
// Read the low byte first then the high byte
uint8_t ABS(Cpu *cpu, Bus *bus)
{
    uint16_t low_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;
    uint16_t high_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    cpu->address_abs = (high_byte << 8) | low_byte;

    return 0;
}

// Absolute mode addressing with X register offset
uint8_t ABX(Cpu *cpu, Bus *bus)
{
    uint16_t low_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;
    uint16_t high_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    cpu->address_abs = (high_byte << 8) | low_byte;
    cpu->address_abs += cpu->x_register;

    // Checking if page boundary has been crossed after adding x
    if ((cpu->address_abs & 0xFF00) != (high_byte << 8))
        return 1; // Overflow, page changed
    else
        return 0;
}

// Absolute mode addressing with Y register offset
uint8_t ABY(Cpu *cpu, Bus *bus)
{
    uint16_t low_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;
    uint16_t high_byte = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    cpu->address_abs = (high_byte << 8) | low_byte;
    cpu->address_abs += cpu->x_register;

    // Checking if page boundary has been crossed after adding y
    if ((cpu->address_abs & 0xFF00) != (high_byte << 8))
        return 1; // Overflow, page changed
    else
        return 0;
}

// Indirect addressing
// 6502's way of pointer implementation
// Interrogate provided address to get the address in which data resides
uint8_t IND(Cpu *cpu, Bus *bus)
{
    uint16_t low_byte_pointer = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;
    uint16_t high_byte_pointer = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;   

    uint16_t pointer_address = (high_byte_pointer << 8) | low_byte_pointer;

    if (low_byte_pointer == 0x00FF) // Simulate page boundary hardware bug
    {
        cpu->address_abs = (cpu->read(bus->ram, pointer_address & 0xFF00) << 8) | cpu->read(bus->ram, pointer_address + 0);
    }
    else // Normal behavior
    {
        cpu->address_abs = (cpu->read(bus->ram, pointer_address + 1) << 8) | cpu->read(bus->ram, pointer_address + 0);
    }

    return 0;
}

// Indirect zero page addressing with X register offset
uint8_t IZX(Cpu *cpu, Bus *bus)
{
    uint16_t supplied_address = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    uint16_t low_byte = cpu->read(bus->ram, (uint16_t)(supplied_address + (uint16_t)cpu->x_register) & 0x00FF);
    uint16_t high_byte = cpu->read(bus->ram, (uint16_t)(supplied_address + (uint16_t)cpu->x_register + 1) & 0x00FF);

    cpu->address_abs = (high_byte << 8) | low_byte;

    return 0;
}

// Indirect zero page addressing with Y register offset
uint8_t IZY(Cpu *cpu, Bus *bus)
{
    uint16_t supplied_address = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    uint16_t low_byte = cpu->read(bus->ram, supplied_address & 0x00FF);
    uint16_t high_byte = cpu->read(bus->ram, (supplied_address + 1) & 0x00FF);

    cpu->address_abs = (high_byte << 8) | low_byte;
    cpu->address_abs += cpu->y_register;

        // Checking if page boundary has been crossed after adding y
        if ((cpu->address_abs & 0xFF00) != (high_byte << 8))
        return 1; // Overflow, page changed
    else
        return 0;
}

// Relative addressing mode
uint8_t REL(Cpu *cpu, Bus *bus)
{
    cpu->address_rel = cpu->read(bus->ram, cpu->pc);
    cpu->pc++;

    if (cpu->address_rel & 0x80) // CHecking if the address is a signed data type
    {
        cpu->address_rel |= 0xFF00; 
    }

    return 0;
}

// Instructions

uint8_t ADC(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)cpu->accumulator + (uint16_t)cpu->fetched_data + (uint16_t)get_flag(cpu, C);

    set_flag(cpu, C, temp>255);
    set_flag(cpu, Z, (temp & 0x00FF) == 0);
    set_flag(cpu, V, (~((uint16_t)cpu->accumulator ^ (uint16_t)cpu->fetched_data) & ((uint16_t)cpu->accumulator ^ (uint16_t)temp)) & 0x0080);
    set_flag(cpu, N, temp & 0x80);

    cpu->accumulator = temp & 0x00FF;

    return 1;
}

uint8_t AND(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->accumulator = cpu->accumulator & cpu->fetched_data;

    set_flag(cpu, Z, cpu->accumulator == 0x00); // Set if accumulator = 0
    set_flag(cpu, N, (cpu->accumulator & (1 << 7)) != 0); // Set if bit 7 is set

    return 1;
}

uint8_t ASL(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)cpu->fetched_data << 1;
    set_flag(cpu, C, (temp & 0xFF00) > 0);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x00);
    set_flag(cpu, N, temp & 0x80);

    if (opcode_lookup[cpu->opcode].address_mode == &IMP)
        cpu->accumulator = temp & 0x00FF;
    else
        cpu->write(bus->ram, cpu->address_abs, temp & 0x00FF);
    
    return 0;
}

uint8_t BCC(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BCS(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BEQ(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BIT(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);
    uint16_t temp = cpu->accumulator & cpu->fetched_data;

    set_flag(cpu, Z, (temp & 0x00FF) == 0x00);
    set_flag(cpu, V, cpu->fetched_data & (1 << 6));
    set_flag(cpu, V, cpu->fetched_data & (1 << 7));

    return 0;
}

uint8_t BMI(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BNE(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BPL(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BRK(Cpu *cpu, Bus *bus)
{
    cpu->pc++;

    set_flag(cpu, I, 1);
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, (cpu->pc >> 8) & 0x00FF);
    cpu->stack_pointer--;
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->pc & 0x00FF);
    cpu->stack_pointer--;

    set_flag(cpu, B, 1);
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->status);
    cpu->stack_pointer--;
    set_flag(cpu, B, 0);

    cpu->pc = (uint16_t)cpu->read(bus->ram, 0xFFFE) | ((uint16_t)cpu->read(bus->ram, 0xFFFF) << 8);

    return 0;
}

uint8_t BVC(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t BVS(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    if (get_flag(cpu, C) == 1)
    {
        cpu->cycles++;
        cpu->address_abs = cpu->pc + cpu->address_rel;

        if ((cpu->address_abs & 0xFF00) != (cpu->pc & 0xFF00)) // check if branch occurs to different page
            cpu->cycles++;

        cpu->pc = cpu->address_abs;
    }
    
    return 0;
}

uint8_t CLC(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, C, false);
    return 0;
}

uint8_t CLD(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, D, false);
    return 0;
}

uint8_t CLI(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, I, false);
    return 0;
}

uint8_t CLV(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, V, false);
    return 0;
}

uint8_t CMP(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)cpu->accumulator - (uint16_t)cpu->fetched_data;

    set_flag(cpu, C, cpu->accumulator >= cpu->fetched_data);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 1;
}

uint8_t CPX(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)cpu->x_register - (uint16_t)cpu->fetched_data;

    set_flag(cpu, C, cpu->x_register >= cpu->fetched_data);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t CPY(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)cpu->y_register - (uint16_t)cpu->fetched_data;

    set_flag(cpu, C, cpu->y_register >= cpu->fetched_data);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t DEC(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = cpu->fetched_data - 1;

    cpu->write(bus->ram, cpu->address_abs, temp & 0x00FF);

    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t DEX(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = cpu->x_register - 1;

    cpu->x_register = temp & 0x00FF;
    
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t DEY(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = cpu->y_register - 1;

    cpu->y_register = temp & 0x00FF;
    
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t EOR(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->accumulator = cpu->accumulator ^ cpu->fetched_data;

    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator == 0x80);

    return 1;
}

uint8_t INC(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = cpu->fetched_data + 1;

    cpu->write(bus->ram, cpu->address_abs, temp & 0x00FF);

    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    return 0;
}

uint8_t INX(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->x_register++;

    set_flag(cpu, Z, cpu->x_register == 0x00);
    set_flag(cpu, N, cpu->x_register & 0x80);

    return 0;
}

uint8_t INY(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->y_register++;

    set_flag(cpu, Z, cpu->y_register == 0x00);
    set_flag(cpu, N, cpu->y_register & 0x80);

    return 0;
}

uint8_t JMP(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->pc = cpu->address_abs;
    return 0;
}

uint8_t JSR(Cpu *cpu, Bus *bus)
{
    cpu->pc--;

    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, (cpu->pc >> 8) & 0x00FF);
    cpu->stack_pointer--;
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->pc & 0x00FF);
    cpu->stack_pointer--;

    cpu->pc = cpu->address_abs;

    return 0;
}

uint8_t LDA(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->accumulator = cpu->fetched_data;

    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator & 0x80);

    return 1;
}

uint8_t LDX(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->x_register = cpu->fetched_data;

    set_flag(cpu, Z, cpu->x_register == 0x00);
    set_flag(cpu, N, cpu->x_register & 0x80);

    return 1;
}

uint8_t LDY(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->y_register = cpu->fetched_data;

    set_flag(cpu, Z, cpu->y_register == 0x00);
    set_flag(cpu, N, cpu->y_register & 0x80);

    return 1;
}

uint8_t LSR(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    set_flag(cpu, C, cpu->fetched_data & 0x0001);
    
    uint16_t temp = cpu->fetched_data >> 1;

    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    if (opcode_lookup[cpu->opcode].address_mode == &IMP)
        cpu->accumulator = temp & 0x00FF;
    else
        cpu->write(bus->ram, cpu->address_abs, temp & 0x00FF);
   
    return 0;
}

uint8_t NOP(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    switch (cpu->opcode) {
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC:
		    return 1;
		    break;
	}
	return 0;
}

uint8_t ORA(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    cpu->accumulator = cpu->accumulator | cpu->fetched_data;

    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator & 0x80);

    return 1;
}

uint8_t PHA(Cpu *cpu, Bus *bus)
{
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, cpu->accumulator); // Pushes a copy of accumulator on the stack
    cpu->stack_pointer--;
    return 0;
}

uint8_t PHP(Cpu *cpu, Bus *bus)
{
    uint8_t temp = cpu->status | B | U;
    cpu->write(bus->ram, 0x0100 + cpu->stack_pointer, temp);
    
    set_flag(cpu, B, 0);
    set_flag(cpu, U, 0);

    cpu->stack_pointer--;

    return 0;
}

uint8_t PLA(Cpu *cpu, Bus *bus)
{
    cpu->stack_pointer++;
    cpu->accumulator = cpu->read(bus->ram, 0x0100 + cpu->stack_pointer);
    
    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator & 0x80);

    return 0;
}

uint8_t PLP(Cpu *cpu, Bus *bus)
{
    cpu->stack_pointer++;
    cpu->status = cpu->read(bus->ram, 0x0100 + cpu->stack_pointer);
    set_flag(cpu, U, 1);
    return 0;
}

uint8_t ROL(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)(cpu->fetched_data << 1) | get_flag(cpu, C);

    set_flag(cpu, C, temp & 0xFF00);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    temp = temp & 0x00FF;
    if (opcode_lookup[cpu->opcode].address_mode == IMP)
        cpu->accumulator = temp;
    else
        cpu->write(bus->ram, cpu->address_abs, temp);

    return 0;

}

uint8_t ROR(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp = (uint16_t)(get_flag(cpu, C) << 7) | (cpu->fetched_data >> 1);

    set_flag(cpu, C, temp & 0x01);
    set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
    set_flag(cpu, N, temp & 0x0080);

    temp = temp & 0x00FF;
    if (opcode_lookup[cpu->opcode].address_mode == IMP)
        cpu->accumulator = temp;
    else
        cpu->write(bus->ram, cpu->address_abs, temp);

    return 0;

}

uint8_t RTI(Cpu *cpu, Bus *bus)
{
    cpu->stack_pointer++;
    cpu->status = cpu->read(bus->ram, 0x0100 + cpu->stack_pointer);
    cpu->status &= ~B;
    cpu->status &= ~U;

    cpu->stack_pointer++;
    cpu->stack_pointer = (uint16_t)cpu->read(bus->ram, 0x0100 + cpu->stack_pointer);
    cpu->stack_pointer++;
    cpu->stack_pointer |= (uint16_t)cpu->read(bus->ram, 0x0100 + cpu->stack_pointer) << 8;

    return 0;
}

uint8_t RTS(Cpu *cpu, Bus *bus)
{
    cpu->stack_pointer++;
    cpu->pc = (uint16_t)cpu->read(bus->ram, 0x0100 + cpu->stack_pointer);

    cpu->stack_pointer++;
    cpu->pc |= (uint16_t)cpu->read(bus->ram, 0x0100 + cpu->stack_pointer) << 8;

    cpu->pc++;

    return 0;
}

uint8_t SBC(Cpu *cpu, Bus *bus)
{
    fetch_data(cpu, bus);

    uint16_t temp1 = ((uint16_t)cpu->fetched_data) ^ 0x00FF;

    uint16_t temp2 = (uint16_t)cpu->accumulator + (uint16_t)cpu->fetched_data + (uint16_t)get_flag(cpu, C);

    set_flag(cpu, C, temp2 & 0xFF00);
    set_flag(cpu, Z, (temp2 & 0x00FF) == 0);
    set_flag(cpu, V, (temp2 ^ (uint16_t)cpu->accumulator) & (temp2 ^ temp1) & 0x0080);
    set_flag(cpu, N, temp2 & 0x0080);

    cpu->accumulator = temp2 & 0x00FF;

    return 1;
}

uint8_t SEC(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, C, true);
    return 0;
}

uint8_t SED(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, D, true);
    return 0;
}

uint8_t SEI(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    set_flag(cpu, I, true);
    return 0;
}

uint8_t STA(Cpu *cpu, Bus *bus)
{
    cpu->write(bus->ram, cpu->address_abs, cpu->accumulator);
    return 0;
}

uint8_t STX(Cpu *cpu, Bus *bus)
{
    cpu->write(bus->ram, cpu->address_abs, cpu->x_register);
    return 0;
}

uint8_t STY(Cpu *cpu, Bus *bus)
{
    cpu->write(bus->ram, cpu->address_abs, cpu->y_register);
    return 0;
}

uint8_t TAX(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    
    cpu->x_register = cpu->accumulator;
    set_flag(cpu, Z, cpu->x_register == 0x00);
    set_flag(cpu, N, cpu->x_register & 0x80);

    return 0;
}

uint8_t TAY(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    
    cpu->y_register = cpu->accumulator;
    set_flag(cpu, Z, cpu->y_register == 0x00);
    set_flag(cpu, N, cpu->y_register & 0x80);

    return 0;
}

uint8_t TSX(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);

    cpu->x_register = cpu->stack_pointer;
    set_flag(cpu, Z, cpu->x_register == 0x00);
    set_flag(cpu, N, cpu->x_register & 0x80);

    return 0;
}

uint8_t TXA(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);

    cpu->accumulator = cpu->x_register;
    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator & 0x80);

    return 0;
}

uint8_t TXS(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);
    cpu->stack_pointer = cpu->x_register;
    return 0;
}

uint8_t TYA(Cpu *cpu, Bus *bus)
{
    UNUSED(bus);

    cpu->accumulator = cpu->y_register;
    set_flag(cpu, Z, cpu->accumulator == 0x00);
    set_flag(cpu, N, cpu->accumulator & 0x80);

    return 0;
}

uint8_t XXX(Cpu *cpu, Bus *bus)
{
    UNUSED(cpu);
    UNUSED(bus);
    return 0;
}