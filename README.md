# RISC-V RV32I Emulator

## Project Overview
This project is a RISC-V RV32I emulator, implemented in C, designed to simulate the fetch-decode-execute cycle of a 32-bit RISC-V processor.  
The current version supports R-type instructions only, which include register-to-register arithmetic and logical operations.  
The emulator replicates the core components of a basic RISC-V CPU, providing a foundational platform for further development and learning in computer architecture.

The emulator mimics essential components of a RISC-V processor, including:
- 32 general-purpose registers (x0–x31)
- A 32-bit Program Counter (PC)
- Static instruction memory
- A basic Arithmetic Logic Unit (ALU)

---

## Architecture
The RV32I architecture is a 32-bit base integer instruction set architecture (ISA) in the RISC-V family. It follows a Reduced Instruction Set Computer (RISC) design philosophy, emphasizing simplicity, modularity, and efficiency.  

RV32I includes a compact set of highly optimized instructions, which reduces hardware complexity and supports low-power, high-speed implementations—making it ideal for education, research, and embedded systems.  

This emulator implements a minimal RV32I execution model by simulating instruction fetch, decode, and execution stages. It currently focuses only on R-type instructions but is structured to allow extension to other instruction types and memory models.

---

## Key Features of This Emulator

### 1. Program Counter (PC)
- A 32-bit register that holds the address of the current instruction.
- Used to fetch the next instruction from instruction memory after each execution cycle.

### 2. Instruction Memory
- A static array of 32-bit machine instructions hardcoded into the program.
- Acts as the read-only memory from which instructions are fetched sequentially.

### 3. Register File
- Contains 32 general-purpose 32-bit registers (x0 to x31).
- Register x0 is hardwired to zero, as specified by the RISC-V specification.

### 4. Instruction Decoder
- Extracts bit fields from each 32-bit instruction using bit masking and shifting.
- Identifies the opcode, funct3, funct7, and register indices to determine the required operation.

### 5. Arithmetic Logic Unit (ALU)
- Performs arithmetic and logical operations such as:  
  `add`, `sub`, `and`, `or`, `xor`, `sll`, `srl`, `sra`, `slt`, and `sltu`.
- Accepts two 32-bit register inputs and returns a single 32-bit result.
- The output is written back to the destination register.

### 6. Execution Unit
- Receives decoded instruction fields and performs the corresponding ALU operation.
- Updates the program counter to point to the next instruction after execution.

