#include "Vfetch.h"
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Vfetch* top = new Vfetch;

    top->rst_n = 0; // Assert reset
    while(time < 10) {
        time++;
    }
    top->rst_n = 1; // Deassert reset


    while(time < 200) {

        CLOCK(top->clk, CLK_PERIOD);
        
        SET_SIG(50, top->next_pc_i, 42);
        SET_SIG(100, top->next_pc_i, 1024);
        SET_SIG(175, top->inst_i, 589);

        top->eval();

        if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2) {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << "PC = " << top->pc_o << std::endl;
            std::cout << "IR = " << top->ir_o << std::endl;
            std::cout << std::endl;
        }

        time++; // Advance simulation time
    }

    delete top;
    exit(0);
}
