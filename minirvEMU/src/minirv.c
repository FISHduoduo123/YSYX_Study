#include <stdio.h>
#include <stdlib.h>
#include "minirv.h"
#include "file_bin_read/file_read.h"

#define PATH "../logisim-bin/sum.bin"

CPU_State cpu = {
    .reg = {0},
    .pc = 0
};
uint8_t *memory = NULL;

uint8_t jalr = 0;

int inst_cycle()    
{
    // 取指令
    uint32_t inst = (memory[cpu.pc] |
                 memory[cpu.pc+1] << 8 |
                 memory[cpu.pc+2] << 16 |
                 memory[cpu.pc+3] << 24);

    // 取操作码
    uint32_t opcode = inst & 0x7F;
    uint8_t load_type = (inst >> 12) & 0x7;
    uint8_t store_type = (inst >> 12) & 0x7;

    // 解析指令格式
    R_Type r_type = {
        .rd = (inst >> 7) & 0x1F,
        .rs1 = (inst >> 15) & 0x1F,
        .rs2 = (inst >> 20) & 0x1F
    };
    I_Type i_type = {
        .rd = (inst >> 7) & 0x1F,
        .rs1 = (inst >> 15) & 0x1F,
        .imm = (int32_t)inst >> 20
    };
    S_Type s_type = {
        .rs1 = (inst >> 15) & 0x1F,
        .rs2 = (inst >> 20) & 0x1F,
        .imm = ((int32_t)inst >> 20 & 0xFFFFFFE0) | ((inst >> 7) & 0x1F)
    };
    U_Type u_type = {
        .rd = (inst >> 7) & 0x1F,
        .imm = (int32_t)(inst & 0xFFFFF000)
    };

    // 执行指令
    switch (opcode) {
        case OP_ADD: {
            if (r_type.rd != 0) cpu.reg[r_type.rd] = ((r_type.rs1==0)?0:cpu.reg[r_type.rs1]) + ((r_type.rs2==0)?0:cpu.reg[r_type.rs2]);
            cpu.pc += 4;
            break;
        }
        case OP_ADDI: {
            if (i_type.rd != 0) cpu.reg[i_type.rd] = ((i_type.rs1==0)?0:cpu.reg[i_type.rs1]) + i_type.imm;
            cpu.pc += 4;
            break;
        }
        case LOAD: {
            uint32_t address = i_type.imm + ((i_type.rs1==0)?0:cpu.reg[i_type.rs1]);
            // LW
            if (load_type == 0x2) {
                if (i_type.rd != 0) cpu.reg[i_type.rd] = 
                    ((uint32_t)memory[address+3] << 24)|
                    ((uint32_t)memory[address+2] << 16)|
                    ((uint32_t)memory[address+1] << 8)|
                    (uint32_t)(memory[address]);
            }
            // LH
            else if (load_type == 0x1) {
                uint16_t H_byte = (uint16_t)memory[address+1] << 8;
                uint16_t L_byte = (uint16_t)memory[address];
                if (i_type.rd != 0) cpu.reg[i_type.rd] = (int32_t)(int16_t)(H_byte | L_byte);
            }
            // LHU
            else if (load_type == 0x5) {
                uint32_t H_byte = (uint32_t)memory[address+1] << 8;
                uint32_t L_byte = (uint32_t)memory[address];
                if (i_type.rd != 0) cpu.reg[i_type.rd] = H_byte|L_byte;
            }
            // LB
            else if (load_type == 0x0) {
                if (i_type.rd != 0) cpu.reg[i_type.rd] = (int32_t)(int8_t)memory[address];
            }
            // LBU
            else if (load_type == 0x4) {
                if (i_type.rd != 0) cpu.reg[i_type.rd] = (uint32_t)memory[address];  
            }  
            cpu.pc += 4;
            break;
        }
        case STORE: {
            uint32_t address = s_type.imm + ((s_type.rs1==0)?0:cpu.reg[s_type.rs1]);
            // SW
            if (store_type == 0x2) {
                memory[address] = (uint8_t)(((s_type.rs2==0)?0:cpu.reg[s_type.rs2]) & 0xFF);
                memory[address+1] = (uint8_t)(((s_type.rs2==0)?0:(cpu.reg[s_type.rs2]) >> 8) & 0xFF);
                memory[address+2] = (uint8_t)(((s_type.rs2==0)?0:(cpu.reg[s_type.rs2]) >> 16) & 0xFF);
                memory[address+3] = (uint8_t)(((s_type.rs2==0)?0:(cpu.reg[s_type.rs2]) >> 24) & 0xFF);
            }
            // SH
            else if (store_type == 0x1) {
                memory[address] = (uint8_t)(((s_type.rs2==0)?0:cpu.reg[s_type.rs2]) & 0xFF);
                memory[address+1] = (uint8_t)(((s_type.rs2==0)?0:(cpu.reg[s_type.rs2]) >> 8) & 0xFF);
            }
            // SB
            else if (store_type == 0x0) {
                memory[address] = (uint8_t)(((s_type.rs2==0)?0:cpu.reg[s_type.rs2]) & 0xFF);
            }
            cpu.pc += 4;
            break;
        }
        case OP_LUI: {
            if (u_type.rd != 0) cpu.reg[u_type.rd] = u_type.imm;
            cpu.pc += 4;
            break;
        } 
        case OP_JALR: {
            uint32_t temp = ((i_type.rs1==0)?0:cpu.reg[i_type.rs1]);
            if (i_type.rd != 0) cpu.reg[i_type.rd] = cpu.pc + 4;
            cpu.pc = (temp + i_type.imm) & 0xFFFFFFFE; //低位置零
            jalr++;
            break;
        } 
        case OP_EBREAK: {
            if (i_type.imm == 0x1 && cpu.reg[10]) {
                printf("HIT BAD TRAP\n");
                exit(0);
            }
            else if (i_type.imm == 0x1 && !cpu.reg[10]) {
                printf("HIT GOOD TRAP\n");
                exit(0);
            }
            break;
        }
        default: {
            printf("Unknown opcode: 0x%X\n", opcode);
            cpu.pc += 4;
            break;
        }
    }
    return 0;
}

void print_cpu_state(int ctr) 
{
    if (ctr) {
        printf("======== CPU State ========\n");
        printf("\tPC: 0x%08X\n", cpu.pc);
        for (int i = 0; i < 32; i++) {
            printf("\tx%d: 0x%08X\n", i, cpu.reg[i]);
        }
        printf("=================\n\n");
    }
}

int main ()
{
    // 加载指令到ROM
    load_file_to_rom (&memory, PATH);
    

    /*while (fgetc(stdin) == '\n') {
        inst_cycle();
        print_cpu_state(1);
        while (fgetc(stdin) != '\n' );
    }*/
    
    while (1) {
        if (jalr < UINT64_MAX){inst_cycle();
        print_cpu_state(1);}
    }
    
   return 0;
}