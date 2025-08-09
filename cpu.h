// cpu.h
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

typedef struct {
    uint32_t regs[32];              // x0-x31 registers
    uint32_t pc;                    // Program counter
    uint8_t *inst_memory;           // Instruction memory
    uint8_t *data_memory;           // Data memory
    unsigned int inst_mem_size;     // Instruction memory size
    unsigned int data_mem_size;     // Data memory size
    int halted;                     // CPU halt flag
    uint64_t instruction_count;     // Instructions executed
} CPU;

// Core CPU functions
void cpu_init(CPU *cpu, unsigned int inst_mem_size, unsigned int data_mem_size);
void cpu_destroy(CPU *cpu);
void cpu_reset(CPU *cpu);

// Register operations
uint32_t cpu_get_reg(CPU *cpu, int reg);
void cpu_set_reg(CPU *cpu, int reg, uint32_t value);

// Memory operations - now specify which memory
uint32_t cpu_read_inst_word(CPU *cpu, uint32_t addr);
uint32_t cpu_read_data_word(CPU *cpu, uint32_t addr);
void cpu_write_data_word(CPU *cpu, uint32_t addr, uint32_t value);
uint16_t cpu_read_data_halfword(CPU *cpu, uint32_t addr);
void cpu_write_data_halfword(CPU *cpu, uint32_t addr, uint16_t value);
uint8_t cpu_read_data_byte(CPU *cpu, uint32_t addr);
void cpu_write_data_byte(CPU *cpu, uint32_t addr, uint8_t value);

// Execution functions
uint32_t cpu_fetch_instruction(CPU *cpu);
void cpu_step(CPU *cpu);
void cpu_run(CPU *cpu);

// Program loading
void cpu_load_inst_program(CPU *cpu, uint32_t *program, int count);
int cpu_load_inst_binary(CPU *cpu, const char *filename);

// Debugging functions
void cpu_dump_registers(CPU *cpu);
void cpu_dump_inst_memory(CPU *cpu, uint32_t start, uint32_t length);
void cpu_dump_data_memory(CPU *cpu, uint32_t start, uint32_t length);

// PC operations
uint32_t cpu_get_pc(CPU *cpu);
void cpu_set_pc(CPU *cpu, uint32_t pc);

// Control functions
int cpu_should_halt(CPU *cpu);
void cpu_halt(CPU *cpu);
uint64_t cpu_get_instruction_count(CPU *cpu);

#endif