// execute.c
#include "execute.h"
#include <stdio.h>

// Execute R-type instructions
void execute_r_type(CPU *cpu, Instruction *inst) {

    // Get source register values
    
    uint32_t rs1_val = cpu_get_reg(cpu, inst->rs1);
    uint32_t rs2_val = cpu_get_reg(cpu, inst->rs2);
    uint32_t result = 0;
    
    // Decode based on funct3 and funct7
    switch (inst->funct3) {
        case 0x0: // ADD/SUB
            if (inst->funct7 == 0x00) {

                // ADD: rd = rs1 + rs2

                result = rs1_val + rs2_val;
                
                printf("ADD x%d, x%d, x%d (0x%08x + 0x%08x = 0x%08x)\n", 
                       inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);

            } else if (inst->funct7 == 0x20) {

                // SUB: rd = rs1 - rs2

                result = rs1_val - rs2_val;

                printf("SUB x%d, x%d, x%d (0x%08x - 0x%08x = 0x%08x)\n", 
                       inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);
                
            } else {

                printf("Unknown R-type instruction: funct3=0x%x, funct7=0x%02x\n", 
                       inst->funct3, inst->funct7);

                cpu_halt(cpu);

                return;
            }
            break;
            
        case 0x1: // SLL (Shift Left Logical)
            // rd = rs1 << (rs2 & 0x1F)

            result = rs1_val << (rs2_val & 0x1F);

            printf("SLL x%d, x%d, x%d (0x%08x << %d = 0x%08x)\n", 
                   inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val & 0x1F, result);
            break;
            
        case 0x2: // SLT (Set Less Than - signed)
            // rd = (rs1_signed < rs2_signed) ? 1 : 0

            result = ((int32_t)rs1_val < (int32_t)rs2_val) ? 1 : 0;

            printf("SLT x%d, x%d, x%d (%d < %d = %d)\n", 
                   inst->rd, inst->rs1, inst->rs2, (int32_t)rs1_val, (int32_t)rs2_val, result);
            break;
            
        case 0x3: // SLTU (Set Less Than Unsigned)
            // rd = (rs1 < rs2) ? 1 : 0

            result = (rs1_val < rs2_val) ? 1 : 0;

            //printf("SLTU x%d, x%d, x%d (%u < %u = %d)\n", 
                   //inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);
            break;
            
        case 0x4: // XOR
            // rd = rs1 ^ rs2

            result = rs1_val ^ rs2_val;

            printf("XOR x%d, x%d, x%d (0x%08x ^ 0x%08x = 0x%08x)\n", 
                   inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);
            break;
            
        case 0x5: // SRL/SRA (Shift Right Logical/Arithmetic)
            if (inst->funct7 == 0x00) {
                // SRL: rd = rs1 >> (rs2 & 0x1F) (logical - fill with zeros)

                result = rs1_val >> (rs2_val & 0x1F);

                printf("SRL x%d, x%d, x%d (0x%08x >> %d = 0x%08x)\n", 
                       inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val & 0x1F, result);

            } else if (inst->funct7 == 0x20) {

                // SRA: rd = rs1 >> (rs2 & 0x1F) (arithmetic - sign extend)

                result = (int32_t)rs1_val >> (rs2_val & 0x1F);

                printf("SRA x%d, x%d, x%d (0x%08x >> %d = 0x%08x) [arithmetic]\n", 
                       inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val & 0x1F, result);

            } else {

                printf("Unknown R-type shift instruction: funct7=0x%02x\n", inst->funct7);

                cpu_halt(cpu);

                return;
            }
            break;
            
        case 0x6: // OR
            // rd = rs1 | rs2

            result = rs1_val | rs2_val;

            printf("OR x%d, x%d, x%d (0x%08x | 0x%08x = 0x%08x)\n", 
                   inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);
            break;
            
        case 0x7: // AND
            // rd = rs1 & rs2

            result = rs1_val & rs2_val;

            printf("AND x%d, x%d, x%d (0x%08x & 0x%08x = 0x%08x)\n", 
                   inst->rd, inst->rs1, inst->rs2, rs1_val, rs2_val, result);
            break;
            
        default:
            printf("Unknown R-type instruction: funct3=0x%x, funct7=0x%02x\n", 
                   inst->funct3, inst->funct7);
            cpu_halt(cpu);
            return;
    }
    
    // Write result to destination register
    cpu_set_reg(cpu, inst->rd, result);
    
    // Advance PC
    cpu->pc += 4;
}

// Main execute function
void execute_instruction(CPU *cpu, Instruction *inst) {

    switch (inst->opcode) {

        case 0x33: // R-type

            execute_r_type(cpu, inst);

            break;
            
        
        
        default:
            printf("Unknown opcode: 0x%02x at PC=0x%08x\n", inst->opcode, cpu->pc);
            cpu_halt(cpu);
            break;
    }
}
