# BamBoo
BamBoo is a WIP Verilog implementation of the RISC-V instruction set architecture. It implements the RV32I spec. The core is currently not fully compliant with the RISC-V spec; there is no support for CSR instructions and system calls (e.g., `ecall`) yet.

It has also been tested on an [Arty S7-50](https://reference.digilentinc.com/reference/programmable-logic/arty-s7/start) FPGA board.

## Feature wishlist (descending priority)

- [ ] Wishbone master
- [ ] Instruction pipeline
- [ ] UART
- [ ] L1 I/D caches
- [ ] FPU
- [ ] TileLink master
