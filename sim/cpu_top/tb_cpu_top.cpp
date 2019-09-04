#include "Vcpu_top.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

uint8_t ram[1024];

uint ram_read(uint addr) {
    uint val = 0;
    val |= ram[addr];
    val |= ram[addr+1]<<8;
    val |= ram[addr+2]<<16;
    val |= ram[addr+3]<<24;

    return val;
}

void ram_write(int en, int we, uint addr, int byte_sel, uint val) {
    if(en) {
        if(we) {
            if(byte_sel==0)
                ram[addr] = (uint8_t)val;
            else if(byte_sel==1) {
                ram[addr] = (uint8_t)val;
                ram[addr+1] = (uint8_t)(val>>8);
            }
            else if(byte_sel==2) {
                ram[addr] = (uint8_t)val;
                ram[addr+1] = (uint8_t)(val>>8);
                ram[addr+2] = (uint8_t)(val>>16);
                ram[addr+3] = (uint8_t)(val>>24);
            }
        }
    }
}

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    Vcpu_top* top = new Vcpu_top; // Change this

    SIM_INIT;

    // Write program to RAM
    //
    // ADDI r0, r1, 40
    // 000000101000 00000 000 00001 0010011
    // 0000 0010 1000 0000 0000 0000 1001 0011 = 0x02800093
    //
    // ADDI r0, r2, 2
    // 000000000010 00000 000 00010 0010011
    // 0000 0000 0010 0000 0000 0001 0001 0011 = 0x00200113
    //
    // ADD r1, r2, r3
    // 0000000 00010 00001 000 00011 0110011
    // 0000 0000 0010 0000 1000 0001 1011 0011 = 0x002081B3
    //
    // SB r0+100, r3
    // 0000011 00011 00000 000 00100 0100011
    // 0000 0110 0011 0000 0000 0010 0010 0011 = 0x06300223
    //
    // ADDI r0, r4, 28
    // 000000011100 00000 000 00100 0010011
    // 0000 0001 1100 0000 0000 0010 0001 0011 = 0x01c00213
    //
    // LB r0+100, r5
    // 000001100100 00000 000 00101 0000011
    // 0000 0110 0100 0000 0000 0010 1000 0011 = 0x06400283
    //
    // BLT r4, r5, 8
    // rs1 = r4, rs2 = r5, imm = 8 = 0 0 000000 0100
    // imm[12|10:5] rs2 rs1 100 imm[4:1|11] 1100011
    // 0 000000 00101 00100 100 01000 1100011
    // 0000 0000 0101 0010 0100 0100 0110 0011 = 0x00524463
    //
    // SW r0+240, r4
    // 0000111 00100 00000 010 10000 0100011
    // 0000 1110 0100 0000 0010 1000 0010 0011 = 0x0e402823
    // 
    // SW r0+200, r2
    // 0000110 00010 00000 010 01000 0100011
    // 0000 1100 0010 0000 0010 0100 0010 0011 = 0x0c202423


    ram_write(1, 1, 0, 2, 0x02800093);
    ram_write(1, 1, 4, 2, 0x00200113);
    ram_write(1, 1, 8, 2, 0x002081B3);
    ram_write(1, 1, 12, 2, 0x06300223);
    ram_write(1, 1, 16, 2, 0x01c00213);
    ram_write(1, 1, 20, 2, 0x06400283);
    ram_write(1, 1, 24, 2, 0x00524463);
    ram_write(1, 1, 28, 2, 0x0e402823);
    ram_write(1, 1, 32, 2, 0x0c202423);

    top->halt = 0;

    top->rst_n = 1;
    while(time < 10) {
        top->eval();
        time++;
    }

    top->rst_n = 0; // Assert reset
    while(time < 20) {
        top->eval();
        time++;
    }
    top->rst_n = 1; // Deassert reset
    top->eval();

    std::cout << "Start PC = " << (int)top->cpu_top__DOT__i_fetch__DOT__pc << std::endl;

   SIM_START(120)

        // Clock
        CLOCK(top->clk, CLK_PERIOD);

        // RAM
        top->inst_i = ram_read(top->fetch_addr);
        if(time%10 == 0)
            ram_write(top->mem_en, top->mem_we, top->mem_addr, top->mem_byte_sel, top->mem_wdata);
        top->mem_rdata = ram_read(top->mem_addr);



        top->eval();

        if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2) {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << std::hex;
            std::cout << "PC = " << (int)top->pc_o << std::endl;
            std::cout << "NPC = " << (int)top->cpu_top__DOT__npc << std::endl;
            std::cout << "pc_offset = " << (int)top->cpu_top__DOT__i_execute__DOT__pc_offset << std::endl;
            std::cout << "branch = " << (int)top->cpu_top__DOT__branch << ", take_branch = " << (int)top->cpu_top__DOT__i_execute__DOT__take_branch << std::endl;
            std::cout << "r1 = " << (int)top->cpu_top__DOT__i_regfile__DOT__regs[1] << ", r2 = " << (int)top->cpu_top__DOT__i_regfile__DOT__regs[2] << ", r3 = " << (int)top->cpu_top__DOT__i_regfile__DOT__regs[3] << ", r4 = " << (int)top->cpu_top__DOT__i_regfile__DOT__regs[4] << ", r5 = " << (int)top->cpu_top__DOT__i_regfile__DOT__regs[5] << std::endl;
            // std::cout << "res_o = " << (int)top->res_o << std::endl;
            std::cout << std::dec;
            std::cout << std::endl;
        }

        SIM_ADV_TIME; // Advance simulation time
    SIM_END;

    std::cout << "addr200 = " << std::hex << ram_read(200) << std::endl; // Should be 2
    std::cout << "addr240 = " << ram_read(240) << std::endl; // Should be 0

    if(ram_read(200)!=2) errors++;
    if(ram_read(240)!=0) errors++;

    TEST_PASSFAIL;

    delete top;
    exit(0);
}
