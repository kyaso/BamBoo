#include "Vbitselect.h" // Change this
#include "verilated.h"
#include "../include/sim_common.h"
#include <iostream>

#define CLK_PERIOD 10

int main(int argc, char** argv, char** env) {
    Verilated::commandArgs(argc, argv);

    uint64_t time = 0;

    Vbitselect* top = new Vbitselect; // Change this

    while(time < 10) {
        
        /// Test byte
        // Set val_i = 0xF0000082
        SET_SIG(0, top->val_i, 0xF0000082);

        // Set funct3 = 0 (signed)
        SET_SIG(0, top->funct3, 0); // @0: val_o = 0xFFFFFF82

        // Set funct3 = 4 (unsigned)
        SET_SIG(1, top->funct3, 4); // @1: val_o = 0x00000082


        /// Test half-word
        // Set val_i = 0x8EAD
        SET_SIG(2, top->val_i, 0x8EAD);

        // Set funct3 = 1 (signed)
        SET_SIG(2, top->funct3, 1); // @2: val_o = 0xFFFF8EAD

        // Set funct3 = 5 (unsigned)
        SET_SIG(3, top->funct3, 5); // @3: val_o = 0x00008EAD


        /// Test word
        // Set val_i = 0xAFFE1234
        SET_SIG(4, top->val_i, 0xAFFE1234);

        // Set funct3 = 2
        SET_SIG(4, top->funct3, 2); // @4: val_o = 0xAFFE1234


        /// Test byte with MSB=0
        // Set val_i = 0x42
        SET_SIG(5, top->val_i, 0x42);

        // Set funct3 = 0 (signed)
        SET_SIG(5, top->funct3, 0); // @5: val_o = 0x00000042

        // Set funct3 = 4 (unsigned)
        SET_SIG(6, top->funct3, 4); // @6: val_o = 0x00000042


        /// Test half-word with MSB=0
        // Set val_i = 0x7EAD
        SET_SIG(7, top->val_i, 0x7EAD);

        // Set funct3 = 1 (signed)
        SET_SIG(7, top->funct3, 1); // @7: val_o = 0x00007EAD

        // Set funct3 = 5 (unsigned)
        SET_SIG(8, top->funct3, 5); // @8: val_o = 0x00007EAD


        top->eval();

        // Read output
        std::cout << ">>> Time = " << time << std::endl;
        std::cout << "val_o = " << std::hex << top->val_o << std::dec << std::endl;
        std::cout << std::endl;

        time++; // Advance simulation time
    }

    delete top;
    exit(0);
}
