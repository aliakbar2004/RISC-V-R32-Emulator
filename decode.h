// decoder.h
#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>

// Instruction formats - R-type only for now
typedef enum {
    R_TYPE,
    UNKNOWN_TYPE
} InstructionType;

// Decoded instruction structure
typedef struct {
    uint32_t opcode;        // Operation code
    uint32_t rd;            // Destination register
    uint32_t rs1;           // Source register 1
    uint32_t rs2;           // Source register 2
    uint32_t funct3;        // Function code
    uint32_t funct7;        // Function code extension
    int32_t imm;            // Immediate value (not used for R-type)
    InstructionType type;   // Instruction format type
} Instruction;

// Main decoding function
void decode_instruction(uint32_t raw_inst, Instruction *inst);

// Field extraction functions
uint32_t extract_opcode(uint32_t inst);
uint32_t extract_rd(uint32_t inst);
uint32_t extract_funct3(uint32_t inst);
uint32_t extract_rs1(uint32_t inst);
uint32_t extract_rs2(uint32_t inst);
uint32_t extract_funct7(uint32_t inst);

// Helper functions
InstructionType get_instruction_type(uint32_t opcode);

#endif