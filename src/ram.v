module ram
#(parameter DEPTH=256)
(
    input wire clk,
    input wire [31:0] data_i,
    input wire mem_we,
    input wire [1:0] byte_sel,
    /* verilator lint_off UNUSED */
    input wire [31:0] addr_a,
    input wire [31:0] addr_b,
    /* verilator lint_on UNUSED */
    output [31:0] data_a,
    output [31:0] data_b
);

    reg [7:0] mem [DEPTH-1:0];

    always @(posedge clk)
    begin
        if(mem_we)
            begin
                if(byte_sel==0) // SB
                    begin
                        mem[addr_a] <= data_i   [7:0];
                    end
                else if(byte_sel==1) // SH
                    begin
                        mem[addr_a]     <= data_i   [7:0];
                        mem[addr_a + 1] <= data_i   [15:8];
                    end
                else if(byte_sel==2) // SW
                    begin
                        mem[addr_a]     <= data_i   [7:0];
                        mem[addr_a + 1] <= data_i   [15:8];
                        mem[addr_a + 2] <= data_i   [23:16];
                        mem[addr_a + 3] <= data_i   [31:24];
                    end
            end
    end

    assign data_a = { mem[addr_a+3], mem[addr_a+2], mem[addr_a+1], mem[addr_a] };
    assign data_b = { mem[addr_b+3], mem[addr_b+2], mem[addr_b+1], mem[addr_b] };

endmodule