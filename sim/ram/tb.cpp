#include "Vram.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Vram* top = new Vram; // Change this

    while(time < 200) {

        CLOCK(top->clk, CLK_PERIOD);
        
        SET_SIG(45, top->data_i, 42);
        SET_SIG(45, top->addr_a, 8); // data_a should be 42 @ 50
        SET_SIG(45, top->mem_we, 1);
        SET_SIG(55, top->mem_we, 0);

        SET_SIG(57, top->data_i, 1024); // data_a should not change @ 60

        SET_SIG(75, top->addr_b, 8); // data_b should be 42

        SET_SIG(88, top->addr_a, 9);
        SET_SIG(88, top->mem_we, 1); // data_a should be 1024 @ 90
        SET_SIG(98, top->mem_we, 0);

        SET_SIG(123, top->addr_b, 9); // data_b should be 1024
        SET_SIG(134, top->addr_a, 2); // data_a should be 0


        top->eval();

        if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2) {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << "data_a = " << top->data_a << std::endl;
            std::cout << "data_b = " << top->data_b << std::endl;
            std::cout << std::endl;
        }

        time++; // Advance simulation time
    }

    delete top;
    exit(0);
}
