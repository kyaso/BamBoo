#include "Vfetch.h"
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    Vfetch* top = new Vfetch;

    SIM_INIT;

    top->rst_n = 0; // Assert reset
    while(time < 10) {
        time++;
    }
    top->rst_n = 1; // Deassert reset


    SIM_START(200)

        CLOCK(top->clk, CLK_PERIOD);
        
        SET_SIG(50, top->next_pc_i, 42);
        SET_SIG(100, top->next_pc_i, 1024);
        SET_SIG(175, top->inst_i, 589);

        SET_SIG(176, top->halt, 1);
        SET_SIG(179, top->next_pc_i, 1245); // @180: PC = 1024

        SET_SIG(184, top->halt, 0); // @190: PC = 1245

        top->eval();

        // Check outputs
        CHECK(50, top->pc_o, 42);
        CHECK(100, top->pc_o, 1024);
        CHECK(180, top->pc_o, 1024);
        CHECK(180, top->ir_o, 0);
        CHECK(190, top->pc_o, 1245);
        CHECK(190, top->ir_o, 589);

        if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2) {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << "PC = " << top->pc_o << std::endl;
            std::cout << "IR = " << top->ir_o << std::endl;
            std::cout << std::endl;
        }

        SIM_ADV_TIME // Advance simulation time
    SIM_END

    TEST_PASSFAIL;

    delete top;
    exit(0);
}
