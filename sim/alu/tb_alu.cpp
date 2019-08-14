#include "Valu.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Valu* top = new Valu; // Change this

    while(time < 40) {

        // Test ADD
        SET_SIG(0, top->op1, 0x42);
        SET_SIG(0, top->op2, 0x56);
        SET_SIG(0, top->funct3, 0x0);
        SET_SIG(0, top->funct7, 0x0);
        SET_SIG(0, top->jal_r, 0x0);
        SET_SIG(0, top->lui, 0x0);
        SET_SIG(0, top->auipc, 0x0);
        SET_SIG(0, top->load, 0x0);
        SET_SIG(0, top->store, 0x0);
        SET_SIG(0, top->has_imm, 0x0); // alu_res = 98

        // Test SUB
        SET_SIG(1, top->funct7, 1); // alu_res = FFEC

        // Test ADDI
        SET_SIG(2, top->has_imm, 0x1); // alu_res = 98

        // Test SLT(I)
        SET_SIG(3, top->funct3, 2); // alu_res = 1

        // Test SLT(I)U
        SET_SIG(4, top->funct3, 3); // alu_res = 1

        // Test XOR(I)
        SET_SIG(5, top->funct3, 4); // alu_res = 14

        // Test OR(I)
        SET_SIG(6, top->funct3, 6); // alu_res = 56

        // Test AND(I)
        SET_SIG(7, top->funct3, 7); // alu_res = 42

        // Test SLL(I)
        SET_SIG(8, top->op2, 0x1);
        SET_SIG(8, top->funct3, 1); // alu_res = 84

        // Test SRL(I)
        SET_SIG(9, top->op2, 0x2);
        SET_SIG(9, top->funct7, 0);
        SET_SIG(9, top->funct3, 5); // alu_res = 10

        // Test SRA(I)
        SET_SIG(10, top->funct7, 0x1); // alu_res = 10

        // Test LUI
        SET_SIG(11, top->op2, 0xDEAD); // alu_res = dead
        SET_SIG(11, top->lui, 0x1);

        // Test AUIPC
        SET_SIG(12, top->lui, 0x0);
        SET_SIG(12, top->auipc, 0x1); // alu_res = deef

        // Test JAl/R
        SET_SIG(13, top->auipc, 0x0);
        SET_SIG(13, top->jal_r, 0x1); // alu_res = deef

        // Test BRANCH
        SET_SIG(14, top->jal_r, 0x0);
        /* BEQ */
        SET_SIG(14, top->funct3, 0x000);
        SET_SIG(14, top->op1, 0x45);
        SET_SIG(14, top->op2, 0x45); // take_branch = 1
        
        SET_SIG(15, top->op1, 0x44); // take_branch = 0

        /* BNE */
        SET_SIG(16, top->funct3, 0x001);
        SET_SIG(16, top->op1, 0x45);
        SET_SIG(16, top->op2, 0x42); // take_branch = 1
        
        SET_SIG(17, top->op1, 0x42); // take_branch = 0

        /* BLT (signed) */
        SET_SIG(18, top->funct3, 4);
        SET_SIG(18, top->op1, 0x45);
        SET_SIG(18, top->op2, 0x89); // take_branch = 1
        
        SET_SIG(19, top->op1, 0x89); // take_branch = 0
        SET_SIG(20, top->op1, 0x90); // take_branch = 0

        SET_SIG(21, top->op1, 0xFFFFFFD3); // take_branch = 1

        /* BLTU */
        SET_SIG(22, top->funct3, 6);
        SET_SIG(22, top->op1, 0x45);
        SET_SIG(22, top->op2, 0x89); // take_branch = 1
        
        SET_SIG(23, top->op1, 0x89); // take_branch = 0
        SET_SIG(24, top->op1, 0x90); // take_branch = 0

        SET_SIG(25, top->op1, 0xFFFFFFD3); // take_branch = 0

        /* BGE (signed) */
        SET_SIG(26, top->funct3, 5);
        SET_SIG(26, top->op1, 0x95);
        SET_SIG(26, top->op2, 0x89); // take_branch = 1
        
        SET_SIG(27, top->op1, 0x89); // take_branch = 1
        SET_SIG(28, top->op1, 0x50); // take_branch = 0

        SET_SIG(29, top->op2, 0xFFFFFFD3); // take_branch = 1

        /* BGEU */
        SET_SIG(30, top->funct3, 7);
        SET_SIG(30, top->op1, 0x95);
        SET_SIG(30, top->op2, 0x89); // take_branch = 1
        
        SET_SIG(31, top->op1, 0x89); // take_branch = 1
        SET_SIG(32, top->op1, 0x50); // take_branch = 0

        SET_SIG(33, top->op2, 0xFFFFFFD3); // take_branch = 0

        // Test LOAD
        SET_SIG(34, top->load, 0x1);
        SET_SIG(34, top->op1, 0x95);
        SET_SIG(34, top->op2, 0x89); // alu_res = 11e

        // Test STORE
        SET_SIG(35, top->load, 0x0);
        SET_SIG(36, top->store, 0x1);
        SET_SIG(36, top->op1, 0x95);
        SET_SIG(36, top->op2, 0x89); // alu_res = 11e


        top->eval();

        /*if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2)*/ {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << std::hex;
            std::cout << "take_branch = " << (int)top->take_branch << std::endl;
            std::cout << "alu_res = " << (int)top->alu_res << std::endl;
            std::cout << std::dec;
            std::cout << std::endl;
        }

        time++; // Advance simulation time
    }

    delete top;
    exit(0);
}
