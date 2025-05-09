#ifndef CPU_H
#define CPU_H
#define OPCODE_COUNT 256

#include "bus.h"

typedef enum {
    C = (1 << 0),  // Carry Bit
    Z = (1 << 1),  // Zero
    I = (1 << 2),  // Disable Interupts
    D = (1 << 3),  // Decimal Mode
    B = (1 << 4),  // Break
    U = (1 << 5),  // Unused
    V = (1 << 6),  // Overflow
    N = (1 << 7),  // Negative
} Flags;

typedef struct {

    uint8_t accumulator, x_register, y_register;            // Acumulator, X and Y registers
    uint8_t stack_pointer;      // Points to a location on bus
    uint16_t pc;                // Program counter
    uint8_t status;             // Status Register

    uint8_t fetched_data;
    uint16_t address_abs; // absolute address
    uint16_t address_rel; // relative address
    uint8_t opcode;
    uint8_t cycles;       // Number of cycles needed to finish an instruction

    void (*write)(uint8_t [], uint16_t, uint8_t);
    uint8_t (*read)(uint8_t [], uint16_t);
} Cpu;

void write(uint8_t ram[], uint16_t address, uint8_t data);
uint8_t read(uint8_t ram[], uint16_t address);
uint8_t get_flag(Cpu *cpu, Flags flag);
void set_flag(Cpu *cpu, Flags flag, bool value);
void clock(Cpu *cpu,Bus *bus);
void reset(Cpu *cpu);
void irq(Cpu *cpu); // Interupt Request Signal; Can be ignored depending on 'enable interupt' flag
void nmi(Cpu *cpu); // Non-maskable Interupt Signal; Cannot be disabled
uint8_t fetch_data(Cpu *cpu);

Cpu *initialize_cpu();

// Adressing modes
uint8_t IMP(Cpu *cpu, Bus *bus);	uint8_t IMM(Cpu *cpu, Bus *bus); // IMP and IMM don't require bus
uint8_t ZP0(Cpu *cpu, Bus *bus);	uint8_t ZPX(Cpu *cpu, Bus *bus);
uint8_t ZPY(Cpu *cpu, Bus *bus);	uint8_t REL(Cpu *cpu, Bus *bus);
uint8_t ABS(Cpu *cpu, Bus *bus);	uint8_t ABX(Cpu *cpu, Bus *bus);
uint8_t ABY(Cpu *cpu, Bus *bus);	uint8_t IND(Cpu *cpu, Bus *bus);
uint8_t IZX(Cpu *cpu, Bus *bus);	uint8_t IZY(Cpu *cpu, Bus *bus);

// Legal Opcodes
uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

// Illegal Opcodes
uint8_t XXX(); // Captures all illegal opcodes

typedef struct {
    const char *name;
    uint8_t (*operation)(Cpu *);
    uint8_t (*address_mode)(Cpu *, Bus *);
    uint8_t cycles;
} Instruction;

#endif