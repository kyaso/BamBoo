#include "Vregfile.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    Vregfile* top = new Vregfile; // Change this

    SIM_INIT;

    SIM_START(200)

        CLOCK(top->clk, CLK_PERIOD);
        
        // Write to x1
        SET_SIG(4, top->rd_sel, 1);
        SET_SIG(4, top->w_val_i, 42);
        SET_SIG(4, top->we, 1);
        SET_SIG(14, top->we, 0);

        // Write to x31
        SET_SIG(16, top->rd_sel, 31);
        SET_SIG(16, top->w_val_i, 569);
        SET_SIG(16, top->we, 1);
        SET_SIG(26, top->we, 0);

        // Read x1 over rs1
        SET_SIG(31, top->rs1_sel, 1); // @35: rs1 = 42

        // Read x2 over rs1
        SET_SIG(46, top->rs1_sel, 2); // @50: rs1 = 0

        // Read x31 over rs2
        SET_SIG(48, top->rs2_sel, 31); // @50: rs2 = 569

        // Read x0 (for rs1 and rs2)
        SET_SIG(51, top->rs1_sel, 0); // @55: rs1 = 0
        SET_SIG(65, top->rs2_sel, 0); // @65: rs2 = 0

        // Write to x0
        SET_SIG(79, top->rd_sel, 0);
        SET_SIG(79, top->w_val_i, 123);
        SET_SIG(79, top->we, 1);
        SET_SIG(89, top->we, 0); // @80: rs1, rs2 = 0


        top->eval();

        if((time % CLK_PERIOD) == 0 || (time % CLK_PERIOD) == CLK_PERIOD/2) {   // Read outputs
            std::cout << ">>> Time = " << time << std::endl;
            std::cout << "rs1 = " << top->rs1_val_o << std::endl;
            std::cout << "rs2 = " << top->rs2_val_o << std::endl;
            std::cout << std::endl;
        }

        // Check outputs
        CHECK(35, top->rs1_val_o, 42);

        CHECK(50, top->rs1_val_o, 0);

        CHECK(50, top->rs2_val_o, 569);

        CHECK(55, top->rs1_val_o, 0);
        CHECK(65, top->rs2_val_o, 0);

        CHECK(80, top->rs1_val_o, 0);
        CHECK(80, top->rs1_val_o, 0);

        SIM_ADV_TIME // Advance simulation time
    SIM_END

    TEST_PASSFAIL

    delete top;
    exit(0);
}
