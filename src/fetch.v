module fetch (
    input wire clk,
    input wire rst_n,
    input wire [31:0] inst_i,
    input wire [31:0] next_pc_i,
    output reg [31:0] pc_o,
    output reg [31:0] ir_o
);

    reg [31:0] pc;
    reg [31:0] ir;

    always @(posedge clk, negedge rst_n)
    begin
        if(~rst_n)
            begin
                pc <= 0;
                ir <= 0;
            end
        else
            begin
                pc <= next_pc_i;
                ir <= inst_i;
            end
    end

    assign pc_o = pc;
    assign ir_o = ir;

endmodule