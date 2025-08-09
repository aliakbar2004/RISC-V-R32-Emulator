// main.c
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "decode.h"
#include "execute.h"

int main() {
    CPU cpu;
    
    // Initialize CPU
    printf("Initializing CPU...\n");
    cpu_init(&cpu, 64 * 1024, 64 * 1024);
    
    // Comprehensive R-type test program
    uint32_t program[] = {
        // Test ADD with different registers
        0x003100B3,  // add  x1, x2, x3     x1 = 20 + 15 = 35
        0x00520233,  // add  x4, x4, x5     x4 = 8 + (-10) = -2
        0x006283B3,  // add  x7, x5, x6     x7 = -10 + 0xFF = 245
        
        // Test SUB
        0x40218433,  // sub  x8, x3, x2     x8 = 15 - 20 = -5
        0x404404B3,  // sub  x9, x8, x4     x9 = -5 - (-2) = -3
        0x40538533,  // sub  x10, x7, x5    x10 = 245 - (-10) = 255
        
        // Test AND
        0x0033F5B3,  // and  x11, x7, x3    x11 = 245 & 15 = 5
        0x00637633,  // and  x12, x6, x6    x12 = 0xFF & 0xFF = 0xFF
        0x003376B3,  // and  x13, x6, x3    x13 = 0xFF & 15 = 15
        
        // Test OR
        0x0011E733,  // or   x14, x3, x1    x14 = 15 | 35 = 47
        0x0062E7B3,  // or   x15, x5, x6    x15 = -10 | 0xFF = -1
        0x00A56833,  // or   x16, x10, x10  x16 = 255 | 255 = 255
        
        // Test XOR
        0x004348B3,  // xor  x17, x6, x4    x17 = 0xFF ^ (-2) = -255
        0x00114933,  // xor  x18, x2, x1    x18 = 20 ^ 35 = 55
        0x012849B3,  // xor  x19, x16, x18  x19 = 255 ^ 55 = 200
        
        // Test SLL (shift left logical)
        0x00211A33,  // sll  x20, x2, x2    x20 = 20 << 20 = 0 (overflow)
        0x00319AB3,  // sll  x21, x3, x3    x21 = 15 << 15 = 491520
        0x00209B33,  // sll  x22, x1, x2    x22 = 35 << 20 = 0 (overflow)
        
        // Test SRL (shift right logical)
        0x0020DBB3,  // srl  x23, x1, x2    x23 = 35 >> 20 = 0
        0x001FDC33,  // srl  x24, x31, x1   x24 = value >> 35 = 0
        0x00AADCB3,  // srl  x25, x21, x10  x25 = 491520 >> 255 = 0
        
        // Test SRA (shift right arithmetic)
        0x40435D33,  // sra  x26, x6, x4    x26 = 0xFF >> (-2 & 0x1F) = 0x3F
        0x40945DB3,  // sra  x27, x8, x9    x27 = -5 >> (-3 & 0x1F) = -1
        0x40A2DE33,  // sra  x28, x5, x10   x28 = -10 >> (255 & 0x1F) = -1
        
        // Test SLT (set less than signed)
        0x00312EB3,  // slt  x29, x2, x3    x29 = (20 < 15) = 0
        0x00522F33,  // slt  x30, x4, x5    x30 = (-2 < -10) = 0
        0x00428FB3,  // slt  x31, x5, x4    x31 = (-10 < -2) = 1
        
        // Test SLTU (set less than unsigned)
        0x00313233,  // sltu x4, x2, x3     
        0x005232B3,  // sltu x5, x4, x5     
        0x00823333,  // sltu x6, x4, x8     
        
        0x00000000   // halt
    };
    
    // Load program into instruction memory
    printf("\nLoading program into instruction memory...\n");
    cpu_load_inst_program(&cpu, program, sizeof(program)/sizeof(uint32_t));
    
    // Pre-load test values into registers
    printf("\nPre-loading test values:\n");
    cpu_set_reg(&cpu, 2, 20);       // x2 = 20
    cpu_set_reg(&cpu, 3, 15);       // x3 = 15
    cpu_set_reg(&cpu, 4, 8);        // x4 = 8
    cpu_set_reg(&cpu, 5, -10);      // x5 = -10 
    cpu_set_reg(&cpu, 6, 0xFF);     // x6 = 255
    cpu_set_reg(&cpu, 31, 0x80000); // x31 = 524288
    
    printf("Initial register values:\n");

    printf("x2=%d, x3=%d, x4=%d, x5=%d, x6=%d, x31=%d\n", 
           cpu_get_reg(&cpu, 2), cpu_get_reg(&cpu, 3), 
           cpu_get_reg(&cpu, 4), (int32_t)cpu_get_reg(&cpu, 5), 
           cpu_get_reg(&cpu, 6), cpu_get_reg(&cpu, 31));
    
    // Show loaded instructions
    printf("\nFirst few instructions:\n");
    cpu_dump_inst_memory(&cpu, 0, 40);
    
    printf("\n=== Executing ===\n\n");
    
    // Execute instructions
    while (!cpu_should_halt(&cpu)) {
        // Fetch
        uint32_t inst = cpu_fetch_instruction(&cpu);
        
        
        if (inst == 0x00000000) {
            printf("\nHALT\n");
            cpu_halt(&cpu);
            break;
        }
        
        // Decode
        Instruction decoded;
        decode_instruction(inst, &decoded);
        
        // Execute
        execute_instruction(&cpu, &decoded);
        
        // Increment instruction count
        cpu.instruction_count++;
        
        
        if (cpu.instruction_count > 50) {
            printf("Instruction limit reached\n");
            break;
        }
    }
    
    // Show results
    printf("\n=== Final State ===\n");
    cpu_dump_registers(&cpu);
    
    // Clean up
    cpu_destroy(&cpu);
    
    return 0;
}