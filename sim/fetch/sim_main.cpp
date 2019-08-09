#include "Vfetch.h"
#include "verilated.h"
#include <iostream>

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

        if((time % 10) == 0) top->clk = 1; // Rising clk edge
        if((time % 10) == 5) top->clk = 0; // Falling clk edge

        if(time == 50) { // TODO: Put these in easy macro
            top->next_pc_i = 42;
        }

        if(time == 100) {
            top->next_pc_i = 1024;
        }

        if(time == 175) {
            top->inst_i = 589;
        }

        top->eval();

        if((time % 10) == 0 || (time % 10) == 5) {   // Read outputs
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
