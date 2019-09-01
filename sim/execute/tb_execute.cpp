#include "Vexecute.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    Vexecute* top = new Vexecute; // Change this

    SIM_INIT;

    SIM_START(14)

        // Test OP-IMM
        /* ADDI */
        SET_SIG(0, top->rs1_i, 40);
        SET_SIG(0, top->imm_i, 2);
        SET_SIG(0, top->funct3, 0);
        SET_SIG(0, top->op_sel, 2); // @0: alu_res = 2a

        /* SLLI */
        SET_SIG(1, top->rs1_i, 50);
        SET_SIG(1, top->imm_i, 3);
        SET_SIG(1, top->funct3, 1);
        SET_SIG(1, top->funct7, 0);
        SET_SIG(1, top->op_sel, 2); // @1: alu_res = 190

        // Test LUI
        SET_SIG(2, top->imm_i, 0x69);
        SET_SIG(2, top->op_sel, 2);
        SET_SIG(2, top->lui, 1); // @2: alu_res = 69
        SET_SIG(3, top->lui, 0);

        // Test AUIPC
        SET_SIG(3, top->imm_i, 0x42);
        SET_SIG(3, top->pc_i, 0xDEAD);
        SET_SIG(3, top->op_sel, 3);
        SET_SIG(3, top->auipc, 1); // @3: alu_res = deef
        SET_SIG(4, top->auipc, 0);

        // Test OP
        /* SUB */
        SET_SIG(4, top->rs1_i, 0x45);
        SET_SIG(4, top->rs2_i, 0x23);
        SET_SIG(4, top->funct3, 0);
        SET_SIG(4, top->funct7, 1);
        SET_SIG(4, top->op_sel, 0); // @4: alu_res = 22

        /* SRL */
        SET_SIG(5, top->rs2_i, 1);
        SET_SIG(5, top->funct3, 1);
        SET_SIG(5, top->funct7, 0);
        SET_SIG(5, top->op_sel, 0); // @5: alu_res = 8a // Until here everything correct so far

        // Test JAL
        SET_SIG(6, top->pc_i, 0xAFFE);
        SET_SIG(6, top->imm_i, 0xFFFFFFFC); //-4
        SET_SIG(6, top->op_sel, 3);
        SET_SIG(6, top->jal, 1); // @6: next_pc_o = affa
        SET_SIG(7, top->jal, 0);

        // Test JALR
        SET_SIG(7, top->rs1_i, 0xBEEF);
        SET_SIG(7, top->imm_i, 0x10);
        SET_SIG(7, top->op_sel, 2);
        SET_SIG(7, top->jalr, 1); // @7: next_pc_o = befe (note: jalr sets LSB to 0!)
        SET_SIG(8, top->jalr, 0);

        // Test BRANCH
        /* BEQ */
        SET_SIG(8, top->rs1_i, 34);
        SET_SIG(8, top->rs2_i, 34);
        SET_SIG(8, top->funct3, 0);
        SET_SIG(8, top->imm_i, 89);
        SET_SIG(8, top->pc_i, 100);
        SET_SIG(8, top->branch, 1);
        SET_SIG(8, top->op_sel, 0); // @8: next_pc_o = bd
        SET_SIG(9, top->branch, 0);

        /* BGE */
        SET_SIG(9, top->rs1_i, 34);
        SET_SIG(9, top->rs2_i, 34);
        SET_SIG(9, top->funct3, 5);
        SET_SIG(9, top->imm_i, 89);
        SET_SIG(9, top->pc_i, 100);
        SET_SIG(9, top->branch, 1);
        SET_SIG(9, top->op_sel, 0); // @9: next_pc_o = bd
        SET_SIG(10, top->branch, 0);

        SET_SIG(10, top->rs1_i, 34);
        SET_SIG(10, top->rs2_i, 35);
        SET_SIG(10, top->funct3, 5);
        SET_SIG(10, top->imm_i, 89);
        SET_SIG(10, top->pc_i, 100);
        SET_SIG(10, top->branch, 1);
        SET_SIG(10, top->op_sel, 0); // @10: next_pc_o = 68
        SET_SIG(11, top->branch, 0);

        // Test LOAD
        SET_SIG(11, top->rs1_i, 1000);
        SET_SIG(11, top->imm_i, 24);
        SET_SIG(11, top->funct3, 1);
        SET_SIG(11, top->op_sel, 2);
        SET_SIG(11, top->load, 1); // @11: next_pc_o = 68, alu_res = 400
        SET_SIG(12, top->load, 0);

        // Test STORE
        SET_SIG(12, top->rs1_i, 1000);
        SET_SIG(12, top->imm_i, 24);
        SET_SIG(12, top->funct3, 1);
        SET_SIG(12, top->op_sel, 2);
        SET_SIG(12, top->store, 1); // @12: next_pc_o = 68, alu_res = 400
        SET_SIG(13, top->store, 0);


        top->eval();

        // Check outputs
        CHECK(0, top->res_o, 0x2a);

        CHECK(1, top->res_o, 0x190);

        CHECK(2, top->res_o, 0x69);

        CHECK(3, top->res_o, 0xdeef);

        CHECK(4, top->res_o, 0x22);

        CHECK(5, top->res_o, 0x8a);

        CHECK(6, top->next_pc_o, 0xaffa);

        CHECK(7, top->next_pc_o, 0xbefe);

        CHECK(8, top->next_pc_o, 0xbd);

        CHECK(9, top->next_pc_o, 0xbd);

        CHECK(10, top->next_pc_o, 0x68);

        CHECK(11, top->next_pc_o, 0x68);
        CHECK(11, top->res_o, 0x400);

        CHECK(12, top->next_pc_o, 0x68);
        CHECK(12, top->res_o, 0x400);

        /*if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2)*/ {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << std::hex;
            std::cout << "next_pc_o = " << (int)top->next_pc_o << std::endl;
            std::cout << "res_o = " << (int)top->res_o << std::endl;
            std::cout << std::dec;
            std::cout << std::endl;
        }

        SIM_ADV_TIME; // Advance simulation time
    SIM_END;

    TEST_PASSFAIL;

    delete top;
    exit(0);
}
