// cpu.c
#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


int i;
uint32_t addr;


// Initialize CPU with separate instruction and data memory
void cpu_init(CPU *cpu, unsigned int inst_mem_size, unsigned int data_mem_size) {
    // Zero out all registers
    for (i = 0; i < 32; i++) {
        cpu->regs[i] = 0;
    }
    
    cpu->pc = 0;
    cpu->inst_mem_size = inst_mem_size;
    cpu->data_mem_size = data_mem_size;
    
    // Allocate instruction memory
    cpu->inst_memory = calloc(1, inst_mem_size);
    if (!cpu->inst_memory) {
        printf("Failed to allocate instruction memory\n");
        exit(1);
    }
    
    // Allocate data memory
    cpu->data_memory = calloc(1, data_mem_size);
    if (!cpu->data_memory) {
        printf("Failed to allocate data memory\n");
        free(cpu->inst_memory);
        exit(1);
    }
    
    cpu->halted = 0;
    cpu->instruction_count = 0;
}

// Free allocated memory
void cpu_destroy(CPU *cpu) {
    if (cpu->inst_memory) {
        free(cpu->inst_memory);
        cpu->inst_memory = NULL;
    }
    if (cpu->data_memory) {
        free(cpu->data_memory);
        cpu->data_memory = NULL;
    }
}

// Reset CPU state (but keep memory contents)
void cpu_reset(CPU *cpu) {
    for (i = 0; i < 32; i++) {
        cpu->regs[i] = 0;
    }
    cpu->pc = 0;
    cpu->halted = 0;
    cpu->instruction_count = 0;
}

// Read 32-bit word from instruction memory
uint32_t cpu_read_inst_word(CPU *cpu, uint32_t addr) {
    if (addr + 3 >= cpu->inst_mem_size) {
        printf("Instruction memory read out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    if (addr % 4 != 0) {
        printf("Misaligned instruction read at 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    uint32_t value = cpu->inst_memory[addr] |
                     (cpu->inst_memory[addr + 1] << 8) |
                     (cpu->inst_memory[addr + 2] << 16) |
                     (cpu->inst_memory[addr + 3] << 24);
    return value;
}

// Read 32-bit word from data memory
uint32_t cpu_read_data_word(CPU *cpu, uint32_t addr) {
    if (addr + 3 >= cpu->data_mem_size) {
        printf("Data memory read out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    if (addr % 4 != 0) {
        printf("Misaligned data word read at 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    uint32_t value = cpu->data_memory[addr] |
                     (cpu->data_memory[addr + 1] << 8) |
                     (cpu->data_memory[addr + 2] << 16) |
                     (cpu->data_memory[addr + 3] << 24);
    return value;
}

// Write 32-bit word to data memory
void cpu_write_data_word(CPU *cpu, uint32_t addr, uint32_t value) {
    if (addr + 3 >= cpu->data_mem_size) {
        printf("Data memory write out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return;
    }
    
    if (addr % 4 != 0) {
        printf("Misaligned data word write at 0x%08x\n", addr);
        cpu->halted = 1;
        return;
    }
    
    cpu->data_memory[addr] = value & 0xFF;
    cpu->data_memory[addr + 1] = (value >> 8) & 0xFF;
    cpu->data_memory[addr + 2] = (value >> 16) & 0xFF;
    cpu->data_memory[addr + 3] = (value >> 24) & 0xFF;
}

// Read 16-bit halfword from data memory
uint16_t cpu_read_data_halfword(CPU *cpu, uint32_t addr) {
    if (addr + 1 >= cpu->data_mem_size) {
        printf("Data memory read out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    if (addr % 2 != 0) {
        printf("Misaligned halfword read at 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    uint16_t value = cpu->data_memory[addr] |
                     (cpu->data_memory[addr + 1] << 8);
    return value;
}

// Write 16-bit halfword to data memory
void cpu_write_data_halfword(CPU *cpu, uint32_t addr, uint16_t value) {
    if (addr + 1 >= cpu->data_mem_size) {
        printf("Data memory write out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return;
    }
    
    if (addr % 2 != 0) {
        printf("Misaligned halfword write at 0x%08x\n", addr);
        cpu->halted = 1;
        return;
    }
    
    cpu->data_memory[addr] = value & 0xFF;
    cpu->data_memory[addr + 1] = (value >> 8) & 0xFF;
}

// Read 8-bit byte from data memory
uint8_t cpu_read_data_byte(CPU *cpu, uint32_t addr) {
    if (addr >= cpu->data_mem_size) {
        printf("Data memory read out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return 0;
    }
    
    return cpu->data_memory[addr];
}

// Write 8-bit byte to data memory
void cpu_write_data_byte(CPU *cpu, uint32_t addr, uint8_t value) {
    if (addr >= cpu->data_mem_size) {
        printf("Data memory write out of bounds: 0x%08x\n", addr);
        cpu->halted = 1;
        return;
    }
    
    cpu->data_memory[addr] = value;
}

// Fetch instruction from instruction memory at PC
uint32_t cpu_fetch_instruction(CPU *cpu) {
    return cpu_read_inst_word(cpu, cpu->pc);
}

// Load program into instruction memory
void cpu_load_inst_program(CPU *cpu, uint32_t *program, int count) {
    for (i = 0; i < count; i++) {
        uint32_t addr = i * 4;
        if (addr + 3 < cpu->inst_mem_size) {
            cpu->inst_memory[addr] = program[i] & 0xFF;
            cpu->inst_memory[addr + 1] = (program[i] >> 8) & 0xFF;
            cpu->inst_memory[addr + 2] = (program[i] >> 16) & 0xFF;
            cpu->inst_memory[addr + 3] = (program[i] >> 24) & 0xFF;
        }
    }
    cpu->pc = 0;
}

// Load binary file into instruction memory
int cpu_load_inst_binary(CPU *cpu, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", filename);
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size > cpu->inst_mem_size) {
        printf("File too large for instruction memory: %ld bytes\n", size);
        fclose(file);
        return -1;
    }
    
    size_t read = fread(cpu->inst_memory, 1, size, file);
    fclose(file);
    
    if (read != size) {
        printf("Failed to read entire file\n");
        return -1;
    }
    
    printf("Loaded %ld bytes from %s\n", size, filename);
    cpu->pc = 0;
    return 0;
}

// Dump registers
void cpu_dump_registers(CPU *cpu) {
    printf("\n=== Register Dump ===\n");
    for (i = 0; i < 32; i += 4) {
        printf("x%-2d: %08x  x%-2d: %08x  x%-2d: %08x  x%-2d: %08x\n",
               i, cpu_get_reg(cpu, i),
               i+1, cpu_get_reg(cpu, i+1),
               i+2, cpu_get_reg(cpu, i+2),
               i+3, cpu_get_reg(cpu, i+3));
    }
    printf("PC:  %08x\n", cpu->pc);
    printf("Instructions executed: %llu\n", cpu->instruction_count);
    printf("===================\n\n");
}

// Dump instruction memory
void cpu_dump_inst_memory(CPU *cpu, uint32_t start, uint32_t length) {
    printf("\n=== Instruction Memory Dump [0x%08x - 0x%08x] ===\n", 
           start, start + length - 1);
    
    for (uint32_t addr = start; addr < start + length && addr < cpu->inst_mem_size; addr += 4) {
        uint32_t inst = cpu_read_inst_word(cpu, addr);
        printf("%08x: %08x\n", addr, inst);
    }
    printf("=====================================\n\n");
}

// // Dump data memory
// void cpu_dump_data_memory(CPU *cpu, uint32_t start, uint32_t length) {
//     printf("\n=== Data Memory Dump [0x%08x - 0x%08x] ===\n", 
//            start, start + length - 1);
    
//     for (uint32_t addr = start; addr < start + length; addr += 16) {
//         printf("%08x: ", addr);
        
//         for (i = 0; i < 16; i++) {
//             if (addr + i < cpu->data_mem_size) {
//                 printf("%02x ", cpu->data_memory[addr + i]);
//             } else {
//                 printf("   ");
//             }
//             if (i == 7) printf(" ");
//         }
        
//         printf(" |");
//         for (i = 0; i < 16; i++) {
//             if (addr + i < cpu->data_mem_size) {
//                 uint8_t byte = cpu->data_memory[addr + i];
//                 if (byte >= 32 && byte < 127) {
//                     printf("%c", byte);
//                 } else {
//                     printf(".");
//                 }
//             }
//         }
//         printf("|\n");
//     }
//     printf("=====================================\n\n");
// }

// Register operations
uint32_t cpu_get_reg(CPU *cpu, int reg) {
    if (reg < 0 || reg >= 32) {
        printf("Invalid register: %d\n", reg);
        return 0;
    }
    if (reg == 0) {
        return 0;
    }
    return cpu->regs[reg];
}

void cpu_set_reg(CPU *cpu, int reg, uint32_t value) {
    if (reg < 0 || reg >= 32) {
        printf("Invalid register: %d\n", reg);
        return;
    }
    if (reg != 0) {
        cpu->regs[reg] = value;
    }
}

// PC operations
uint32_t cpu_get_pc(CPU *cpu) {
    return cpu->pc;
}

void cpu_set_pc(CPU *cpu, uint32_t pc) {
    cpu->pc = pc;
}

// Control functions
int cpu_should_halt(CPU *cpu) {
    return cpu->halted;
}

void cpu_halt(CPU *cpu) {
    cpu->halted = 1;
}

uint64_t cpu_get_instruction_count(CPU *cpu) {
    return cpu->instruction_count;
}

// Execute one instruction
void cpu_step(CPU *cpu) {
    if (cpu->halted) {
        return;
    }
    
    uint32_t inst = cpu_fetch_instruction(cpu);
    
    if (inst == 0x00000000) {
        cpu->halted = 1;
        return;
    }
    
    
    printf("PC: 0x%08x, Instruction: 0x%08x\n", cpu->pc, inst);
    
    cpu->pc += 4;
    cpu->instruction_count++;
}

// Run until halt
void cpu_run(CPU *cpu) {
    while (!cpu->halted) {
        cpu_step(cpu);
        
        if (cpu->pc >= cpu->inst_mem_size) {
            printf("PC out of instruction memory bounds: 0x%08x\n", cpu->pc);
            cpu->halted = 1;
        }
    }
    
    printf("\nCPU halted after %llu instructions\n", cpu->instruction_count);
}
