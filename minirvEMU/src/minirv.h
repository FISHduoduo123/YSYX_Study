#ifndef __MINIRV_H__
#define __MINIRV_H__

#include <stdint.h>

/*======== CPU State ========*/
typedef struct {
    uint32_t reg[32];
    uint32_t pc;
} CPU_State;

/*======== Opcodes ========*/
#define OP_ADD 0x33
#define OP_ADDI 0x13
#define LOAD 0x03
#define STORE 0x23
#define OP_LUI 0x37
#define OP_JALR 0x67
#define OP_EBREAK 0X73

/*======== Instruction Formats ========*/
// R-Type
typedef struct {
    uint32_t rd;
    uint32_t rs1;
    uint32_t rs2;
} R_Type;

// I-Type
typedef struct {
    uint32_t rd;
    uint32_t rs1;
    int32_t imm;
} I_Type;

// S-Type
typedef struct {
    uint32_t rs1;
    uint32_t rs2;
    int32_t imm;
} S_Type;

// U-Type
typedef struct {
    uint32_t rd;
    int32_t imm;
} U_Type;


#endif