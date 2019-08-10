module ram
#(parameter ADDR_WIDTH=32, DATA_WIDTH=32, DEPTH=256)
(
    input wire clk,
    input wire [DATA_WIDTH-1:0] data_i,
    input wire mem_we,
    /* verilator lint_off UNUSED */
    input wire [ADDR_WIDTH-1:0] addr_a,
    input wire [ADDR_WIDTH-1:0] addr_b,
    /* verilator lint_on UNUSED */
    output reg [DATA_WIDTH-1:0] data_a,
    output reg [DATA_WIDTH-1:0] data_b
);

    reg [DATA_WIDTH-1:0] mem [DEPTH-1:0];

    always @(posedge clk)
    begin
        if(mem_we)
            begin
                mem[addr_a] <= data_i;
            end
    end

    assign data_a = mem[addr_a];
    assign data_b = mem[addr_b];

endmodule