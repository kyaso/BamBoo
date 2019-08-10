module regfile (
    input wire clk,
    input wire [4:0] rs1_sel,
    input wire [4:0] rs2_sel,
    input wire [4:0] rd_sel,
    input wire we,
    input wire [31:0] w_val_i,
    output reg [31:0] rs1_val_o,
    output reg [31:0] rs2_val_o
);

    reg [31:0] regs [31:0];

    always @(posedge clk)
    begin
        if(we)
            begin
                regs[rd_sel] <= w_val_i;
            end
    end

    always @*
    begin
        if(rs1_sel == 0)
            rs1_val_o = 32'b0;
        else
            rs1_val_o = regs[rs1_sel];
        
        if(rs2_sel == 0)
            rs2_val_o = 32'b0;
        else
            rs2_val_o = regs[rs2_sel];
    end

endmodule