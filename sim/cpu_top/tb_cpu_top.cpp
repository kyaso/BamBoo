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

void ram_write(int we, uint addr, int byte_sel, uint val) {
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

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Vcpu_top* top = new Vcpu_top; // Change this

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
    // SB r0+16, r3
    // 0000000 00011 00000 000 10000 0100011
    // 0000 0000 0011 0000 0000 1000 0010 0011 = 0x00300823

    ram_write(1, 0, 2, 0x02800093);
    ram_write(1, 4, 2, 0x00200113);
    ram_write(1, 8, 2, 0x002081B3);
    ram_write(1, 12, 2, 0x00300823);

    // Disable reset
    top->rst_n = 1;

    while(time < 60) {

        // Clock
        CLOCK(top->clk, CLK_PERIOD);

        // RAM
        top->inst_i = ram_read(top->fetch_addr);
        if(time%10 == 0)
            ram_write(top->mem_we, top->mem_addr, top->mem_byte_sel, top->mem_wdata);
        top->mem_rdata = ram_read(top->mem_addr);



        top->eval();

        /*if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2)*/ {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << std::hex;
            std::cout << "PC = " << (int)top->fetch_addr << std::endl;
            // std::cout << "res_o = " << (int)top->res_o << std::endl;
            std::cout << std::dec;
            std::cout << std::endl;
        }

        time++; // Advance simulation time
    }

    std::cout << "Result = " << std::hex << ram_read(16) << std::endl; // Should be 42

    delete top;
    exit(0);
}
