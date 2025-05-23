#ifndef CPU_H
#define CPU_H
#define OPCODE_COUNT 256
#define UNUSED(x) (void)(x)

#include "bus.h"
#include "disassembler_ht.h"

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

void cpu_write(uint8_t ram[], uint16_t address, uint8_t data);
uint8_t cpu_read(uint8_t ram[], uint16_t address);
uint8_t get_flag(Cpu *cpu, Flags flag);
void set_flag(Cpu *cpu, Flags flag, bool value);
void cpu_clock(Cpu *cpu, Bus *bus);
void reset(Cpu *cpu, Bus *bus);
void irq(Cpu *cpu, Bus *bus); // Interupt Request Signal; Can be ignored depending on 'enable interupt' flag
void nmi(Cpu *cpu, Bus *bus); // Non-maskable Interupt Signal; Cannot be disabled
uint8_t fetch_data(Cpu *cpu, Bus *bus);

Cpu *initialize_cpu();

// Adressing modes
uint8_t IMP(Cpu *cpu, Bus *bus);	uint8_t IMM(Cpu *cpu, Bus *bus); // IMP and IMM don't require bus
uint8_t ZP0(Cpu *cpu, Bus *bus);	uint8_t ZPX(Cpu *cpu, Bus *bus);
uint8_t ZPY(Cpu *cpu, Bus *bus);	uint8_t REL(Cpu *cpu, Bus *bus);
uint8_t ABS(Cpu *cpu, Bus *bus);	uint8_t ABX(Cpu *cpu, Bus *bus);
uint8_t ABY(Cpu *cpu, Bus *bus);	uint8_t IND(Cpu *cpu, Bus *bus);
uint8_t IZX(Cpu *cpu, Bus *bus);	uint8_t IZY(Cpu *cpu, Bus *bus);

// Legal Opcodes
uint8_t ADC(Cpu *cpu, Bus *bus);	uint8_t AND(Cpu *cpu, Bus *bus);	uint8_t ASL(Cpu *cpu, Bus *bus);	uint8_t BCC(Cpu *cpu, Bus *bus);
uint8_t BCS(Cpu *cpu, Bus *bus);	uint8_t BEQ(Cpu *cpu, Bus *bus);	uint8_t BIT(Cpu *cpu, Bus *bus);	uint8_t BMI(Cpu *cpu, Bus *bus);
uint8_t BNE(Cpu *cpu, Bus *bus);	uint8_t BPL(Cpu *cpu, Bus *bus);	uint8_t BRK(Cpu *cpu, Bus *bus);	uint8_t BVC(Cpu *cpu, Bus *bus);
uint8_t BVS(Cpu *cpu, Bus *bus);	uint8_t CLC(Cpu *cpu, Bus *bus);	uint8_t CLD(Cpu *cpu, Bus *bus);	uint8_t CLI(Cpu *cpu, Bus *bus);
uint8_t CLV(Cpu *cpu, Bus *bus);	uint8_t CMP(Cpu *cpu, Bus *bus);	uint8_t CPX(Cpu *cpu, Bus *bus);	uint8_t CPY(Cpu *cpu, Bus *bus);
uint8_t DEC(Cpu *cpu, Bus *bus);	uint8_t DEX(Cpu *cpu, Bus *bus);	uint8_t DEY(Cpu *cpu, Bus *bus);	uint8_t EOR(Cpu *cpu, Bus *bus);
uint8_t INC(Cpu *cpu, Bus *bus);	uint8_t INX(Cpu *cpu, Bus *bus);	uint8_t INY(Cpu *cpu, Bus *bus);	uint8_t JMP(Cpu *cpu, Bus *bus);
uint8_t JSR(Cpu *cpu, Bus *bus);	uint8_t LDA(Cpu *cpu, Bus *bus);	uint8_t LDX(Cpu *cpu, Bus *bus);	uint8_t LDY(Cpu *cpu, Bus *bus);
uint8_t LSR(Cpu *cpu, Bus *bus);	uint8_t NOP(Cpu *cpu, Bus *bus);	uint8_t ORA(Cpu *cpu, Bus *bus);	uint8_t PHA(Cpu *cpu, Bus *bus);
uint8_t PHP(Cpu *cpu, Bus *bus);	uint8_t PLA(Cpu *cpu, Bus *bus);	uint8_t PLP(Cpu *cpu, Bus *bus);	uint8_t ROL(Cpu *cpu, Bus *bus);
uint8_t ROR(Cpu *cpu, Bus *bus);	uint8_t RTI(Cpu *cpu, Bus *bus);	uint8_t RTS(Cpu *cpu, Bus *bus);	uint8_t SBC(Cpu *cpu, Bus *bus);
uint8_t SEC(Cpu *cpu, Bus *bus);	uint8_t SED(Cpu *cpu, Bus *bus);	uint8_t SEI(Cpu *cpu, Bus *bus);	uint8_t STA(Cpu *cpu, Bus *bus);
uint8_t STX(Cpu *cpu, Bus *bus);	uint8_t STY(Cpu *cpu, Bus *bus);	uint8_t TAX(Cpu *cpu, Bus *bus);	uint8_t TAY(Cpu *cpu, Bus *bus);
uint8_t TSX(Cpu *cpu, Bus *bus);	uint8_t TXA(Cpu *cpu, Bus *bus);	uint8_t TXS(Cpu *cpu, Bus *bus);	uint8_t TYA(Cpu *cpu, Bus *bus);

// Illegal Opcodes
uint8_t XXX(); // Captures all illegal opcodes

typedef struct {
    const char *name;
    uint8_t (*operation)(Cpu *, Bus *);
    uint8_t (*address_mode)(Cpu *, Bus *);
    uint8_t cycles;
} Instruction;

Line *disassembler(Bus *bus, uint16_t start_address, uint16_t end_address);

#endif