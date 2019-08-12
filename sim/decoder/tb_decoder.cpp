#include "Vdecoder.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Vdecoder* top = new Vdecoder; // Change this

    while(time < 9) {

        // ADDI r2 <-- r1 + 1001 1001 0110
        // 100110010110 00001 000 00010 0010011
        // 1001 1001 0110 0000 1000 0001 0001 0011 = 0x99608113
        //
        // GOLDEN:
        // rs1 = 1, rs2 = x
        // rd = 2
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = x
        // funct3 = 0
        // op_sel = 2
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 0
        // imm_o = fffff996
        SET_SIG(0, top->ir_i, 0x99608113)

        // XOR r24 <-- r4, r15
        // 0000000 01111 00100 100 11000 0110011
        // 0000 0000 1111 0010 0100 1100 0011 0011 = 0x00F24C33
        //
        // GOLDEN:
        // rs1 = 4, rs2 = f
        // rd = 18
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = 0
        // funct3 = 4
        // op_sel = 0
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 0
        // imm_o = x
        SET_SIG(1, top->ir_i, 0x00F24C33)

        // JAL 1 00111010 1 1000111011, r6
        // 1 1000111011 1 00111010 00110 1101111
        // 1100 0111 0111 0011 1010 0011 0110 1111 = 0xC773A36F
        //
        // GOLDEN:
        // rs1 = x, rs2 = x
        // rd = 6
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = x
        // funct3 = x
        // op_sel = 3
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 1
        // imm_o = FFF3AC76
        SET_SIG(2, top->ir_i, 0xC773A36F)

        // JALR 1100 1101 0101, rs1=r29, rd=r17
        // 110011010101 11101 000 10001 1100111
        // 1100 1101 0101 1110 1000 1000 1110 0111 = 0xCD5E88E7
        //
        // GOLDEN:
        // rs1 = 1d, rs2 = x
        // rd = 11
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = x
        // funct3 = 0
        // op_sel = 2
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 1
        // imm_o = fffffcd5
        SET_SIG(3, top->ir_i, 0xCD5E88E7)

        // BGE r18, r2, 1 0 101100 0110
        // 1 101100 00010 10010 101 0110 0 1100011
        // 1101 1000 0010 1001 0101 0110 0110 0011 = 0xD8295663
        //
        // GOLDEN:
        // rs1 = 12, rs2 = 2
        // rd = x
        // reg_we = 0
        // load = 0, store = 0
        // funct7 = x
        // funct3 = 5
        // op_sel = 0
        // lui = 0
        // auipc = 0
        // branch = 1
        // jal/r = 0
        // imm_o = FFFFF58C
        SET_SIG(4, top->ir_i, 0xD8295663)

        // LUI 10010011101100011010, r5
        // 10010011101100011010 00101 0110111
        // 1001 0011 1011 0001 1010 0010 1011 0111 = 0x93B1A2B7
        //
        // GOLDEN:
        // rs1 = x, rs2 = x
        // rd = 5
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = x
        // funct3 = x
        // op_sel = 2
        // lui = 1
        // auipc = 0
        // branch = 0
        // jal/r = 0
        // imm_o = 93b1a000
        SET_SIG(5, top->ir_i, 0x93B1A2B7)

        // AUIPC 10010011101100011010, r7
        // 10010011101100011010 00111 0010111
        // 1001 0011 1011 0001 1010 0011 1001 0111 = 0x93B1A397
        //
        // GOLDEN:
        // rs1 = x, rs2 = x
        // rd = 7
        // reg_we = 1
        // load = 0, store = 0
        // funct7 = x
        // funct3 = x
        // op_sel = 3
        // lui = 0
        // auipc = 1
        // branch = 0
        // jal/r = 0
        // imm_o = 93b1a000
        SET_SIG(6, top->ir_i, 0x93B1A397)

        // LW r3 <-- [r8 + 1001 0011 0100]
        // 100100110100 01000 010 00011 0000011
        // 1001 0011 0100 0100 0010 0001 1000 0011 = 0x93442183
        //
        // GOLDEN:
        // rs1 = 8, rs2 = x
        // rd = 3
        // reg_we = 1
        // load = 1, store = 0
        // funct7 = x
        // funct3 = 2
        // op_sel = 2
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 0
        // imm_o = fffff934
        SET_SIG(7, top->ir_i, 0x93442183)

        // SB rs1=r1, rs2=r23, imm=1000110 10001
        // 1000110 10111 00001 000 10001 0100011
        // 1000 1101 0111 0000 1000 1000 1010 0011 = 0x8D7088A3
        //
        // GOLDEN:
        // rs1 = 1, rs2 = x
        // rd = x
        // reg_we = 0
        // load = 0, store = 1
        // funct7 = x
        // funct3 = 0
        // op_sel = 2
        // lui = 0
        // auipc = 0
        // branch = 0
        // jal/r = 0
        // imm_o = fffff8d1
        SET_SIG(8, top->ir_i, 0x8D7088A3)

        top->eval();

        /*if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2)*/ {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << std::hex;
            std::cout << "rs1 = " << (int)top->rs1 << ", rs2 = " << (int)top->rs2 << std::endl;
            std::cout << "rd = " << (int)top->rd << std::endl;
            std::cout << "reg_we = " << (int)top->reg_we << std::endl;
            std::cout << "load = " << (int)top->load << ", store = " << (int)top->store << std::endl;
            std::cout << "funct7 = " << (int)top->funct7 << std::endl;
            std::cout << "funct3 = " << (int)top->funct3 << std::endl;
            std::cout << "op_sel = " << (int)top->op_sel << std::endl;
            std::cout << "lui = " << (int)top->lui << std::endl;
            std::cout << "auipc = " << (int)top->auipc << std::endl;
            std::cout << "branch = " << (int)top->branch << std::endl;
            std::cout << "jal/r = " << (int)top->jalr << std::endl;
            std::cout << "imm_o = " << (int)top->imm_o << std::endl;
            std::cout << std::dec;
            std::cout << std::endl;
        }

        time++; // Advance simulation time
    }

    delete top;
    exit(0);
}
