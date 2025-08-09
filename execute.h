// execute.h
#ifndef EXECUTE_H
#define EXECUTE_H

#include "cpu.h"
#include "decode.h"

// Main execute function
void execute_instruction(CPU *cpu, Instruction *inst);

// R-type instruction execution
void execute_r_type(CPU *cpu, Instruction *inst);

#endif