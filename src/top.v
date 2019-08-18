module top (
    input clk,
    input rst_n,
    input [31:0] mem_wdata_dbg,
    input [31:0] mem_addr_dbg,
    input [1:0] mem_byte_sel_dbg,
    input mem_we_dbg,
    input debug,
    output [31:0] mem_rdata_dbg
);

    wire [31:0] mem_wdata_cpu;
    reg [31:0] mem_wdata;
    reg mem_we;
    wire mem_we_cpu;
    reg [1:0] byte_sel;
    wire [1:0] byte_sel_cpu;
    reg [31:0] mem_addr;
    wire [31:0] mem_addr_cpu;
    wire [31:0] fetch_addr;
    wire [31:0] mem_rdata;
    wire [31:0] instr;

    always @*
    begin
        if(debug)
            begin
                mem_wdata = mem_wdata_dbg;
                mem_we = mem_we_dbg;
                byte_sel = mem_byte_sel_dbg;
                mem_addr = mem_addr_dbg;
            end
        else
            begin
                mem_wdata = mem_wdata_cpu;
                mem_we = mem_we_cpu;
                byte_sel = byte_sel_cpu;
                mem_addr = mem_addr_cpu;
            end
    end

    assign mem_rdata_dbg = mem_rdata;

    ram i_ram (
        .clk(clk),
        .data_i(mem_wdata),
        .mem_we(mem_we),
        .byte_sel(byte_sel),
        .addr_a(mem_addr),
        .addr_b(fetch_addr),
        .data_a(mem_rdata),
        .data_b(instr)
    );

    cpu_top i_cpu (
        .clk(clk),
        .rst_n(rst_n),
        .inst_i(instr),
        .mem_rdata(mem_rdata),
        .halt(debug),
        .fetch_addr(fetch_addr),
        .mem_addr(mem_addr_cpu),
        .mem_byte_sel(byte_sel_cpu),
        .mem_wdata(mem_wdata_cpu),
        .mem_we(mem_we_cpu)
    );

endmodule