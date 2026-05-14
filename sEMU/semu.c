/*
 * PC位宽为4位, 初值为0
 * GPR有4个, 位宽均为8位
 * 支持如下3条指令:
 * 7  6 5  4 3   2 1   0
 * +----+----+-----+-----+
 * | 00 | rd | rs1 | rs2 | R[rd]=R[rs1]+R[rs2]       add指令, 寄存器相加
 * +----+----+-----+-----+
 * | 01 |   -      | rs2 | out R[rs2]
 * +----+----+-----+-----+
 * | 10 | rd |    imm    | R[rd]=imm                 li指令, 装入立即数, 高位补0
 * +----+----+-----+-----+
 * | 11 |   addr   | rs2 | if (R[0]!=R[rs2]) PC=addr bner0指令, 若不等于R[0]则跳转
 * ----+----------+-----+
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

// 定义寄存器和程序计数器
uint8_t PC = 0;             // 4位程序计数器
uint8_t R[4] = {0};         // 4个8位寄存
uint8_t M[16] = {144, 160, 177, 23, 41, 205, 66};   // 16字节内存, 存储指令和数据

// 一个指令周期
int inst_cycle()
{
    // 取指令
    uint8_t op_code = M[PC] >> 6;      // 操作码
    uint8_t rd = M[PC] >> 4 & 0x03;    // 目标寄存器
    uint8_t rs1 = M[PC] >> 2 & 0x03;   // 源寄存器1
    uint8_t rs2 = M[PC] & 0x03;        // 源寄存器2

    uint8_t imm = M[PC] & 0x0F;        // 立即数
    uint8_t addr = M[PC] >> 2 & 0x0F;  // 目标内存地址

    // 译码并执行
    switch (op_code) {
        // add
        case 0b00 : {
            R[rd] = R[rs1] + R[rs2];
            break;
        }
        // out
        case 0b01 : printf("R%" PRIu8 ": %" PRIu8 "\n", rs2, R[rs2]); break;
        // li
        case 0b10 : {
            R[rd] = imm;
            break;
        }
        // ben0
        case 0b11 :
            if (R[0] != R[rs2]) {
                PC = addr;
                return 1;
            }
    }
    return 0;
}

// 模型状态监视
void st_print () 
{
    printf("op: %" PRIu8 "-------------------\n", M[PC] >> 6);
    printf("PC: %" PRIu8 "\n", PC);
    printf("R0: %" PRIu8 ", R1: %" PRIu8 ", R2: %" PRIu8 ", R3: %" PRIu8 "\n", R[0], R[1], R[2], R[3]);
    printf("-------------------\n");
}

int main (int argc, char *argv[])
{
    uint8_t count = 0;    // 指令周期计数器

    R[0] = atoi(argv[1]); // 初始化R[0]为输入参数

    while (count < UINT8_MAX) {
        if (PC >= 16) break;  // 地址溢出结束运行
        if (inst_cycle()) count++;
        else PC++;
        //st_print();
    }
    return 0;
}