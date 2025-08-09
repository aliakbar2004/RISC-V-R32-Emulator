// decode.c
#include "decode.h"
#include <stdio.h>

// Extract opcode (bits [6:0])
uint32_t extract_opcode(uint32_t inst) {
    return inst & 0x7F;
}

// Extract rd (bits [11:7])
uint32_t extract_rd(uint32_t inst) {
    return (inst >> 7) & 0x1F;
}

// Extract funct3 (bits [14:12])
uint32_t extract_funct3(uint32_t inst) {
    return (inst >> 12) & 0x7;
}

// Extract rs1 (bits [19:15])
uint32_t extract_rs1(uint32_t inst) {
    return (inst >> 15) & 0x1F;
}

// Extract rs2 (bits [24:20])
uint32_t extract_rs2(uint32_t inst) {
    return (inst >> 20) & 0x1F;
}

// Extract funct7 (bits [31:25])
uint32_t extract_funct7(uint32_t inst) {
    return (inst >> 25) & 0x7F;
}

// Get instruction type based on opcode
InstructionType get_instruction_type(uint32_t opcode) {
    switch (opcode) {
        case 0x33:  // R-type: ADD, SUB, etc.
            return R_TYPE;
        default:
            return UNKNOWN_TYPE;
    }
}

// Main decode function - R-type only
void decode_instruction(uint32_t raw_inst, Instruction *inst) {
    // Extract opcode first
    inst->opcode = extract_opcode(raw_inst);
    
    // Debug print to verify extraction
    printf("Decoding instruction: 0x%08x\n", raw_inst);
    
    // Determine instruction type
    inst->type = get_instruction_type(inst->opcode);
    
    // Extract fields for R-type only
    if (inst->type == R_TYPE) {
        inst->rd = extract_rd(raw_inst);
        inst->rs1 = extract_rs1(raw_inst);
        inst->rs2 = extract_rs2(raw_inst);
        inst->funct3 = extract_funct3(raw_inst);
        inst->funct7 = extract_funct7(raw_inst);
        inst->imm = 0;  // R-type has no immediate
        
        // Debug print to verify all fields
        printf("  Opcode: 0x%02x (R-type)\n", inst->opcode);
        printf("  rd: x%d, rs1: x%d, rs2: x%d\n", inst->rd, inst->rs1, inst->rs2);
        printf("  funct3: 0x%x, funct7: 0x%02x\n", inst->funct3, inst->funct7);
        
        // Verify the instruction encoding
        uint32_t reconstructed = (inst->funct7 << 25) | 
                                (inst->rs2 << 20) | 
                                (inst->rs1 << 15) | 
                                (inst->funct3 << 12) | 
                                (inst->rd << 7) | 
                                inst->opcode;
        
        if (reconstructed != raw_inst) {
            printf("  ERROR: Instruction reconstruction failed!\n");
            printf("  Original: 0x%08x, Reconstructed: 0x%08x\n", raw_inst, reconstructed);
        }
    } else {
        // Zero out all fields for non-R-type instruction
        inst->rd = 0;
        inst->rs1 = 0;
        inst->rs2 = 0;
        inst->funct3 = 0;
        inst->funct7 = 0;
        inst->imm = 0;
        
        printf("  ERROR: Not an R-type instruction (opcode: 0x%02x)\n", inst->opcode);
    }
}