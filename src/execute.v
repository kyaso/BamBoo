module execute (
    input [31:0] rs1_i,
    input [31:0] rs2_i,
    input [31:0] pc_i,
    input [31:0] imm_i,
    input [2:0] funct3,
    input funct7,
    input [1:0] op_sel,
    input lui,
    input auipc,
    input branch,
    input jal,
    input jalr,
    output reg [31:0] next_pc_o
    output reg [31:0] alu_res_o
);

    wire [31:0] op1;
    wire [31:0] op2;
    wire [31:0] pc_offset;
    wire take_branch;
    wire next_pc_branch;
    wire next_pc_jal_r;
    wire jal_r;
    wire [31:0] alu_res;

    assign jal_r = jal | jalr;

    // Operand selection MUXes
    always @*
    begin
        case (op_sel[0])
            1'b0: op1 = rs1_i;
            1'b1: op1 = pc_i;
        endcase

        case (op_sel[1])
            1'b0: op2 = rs2_i;
            1'b1: op2 = imm_i;
        endcase
    end

    // PC offset MUX (for conditional branch and default PC increment)
    always @*
    begin
        case (take_branch & branch)
            1'b0: pc_offset = 4;
            1'b1: pc_offset = imm_i;
        endcase
    end

    // Default/Branch PC offset
    assign next_pc_branch = pc_i + pc_offset;

    // JAL/R PC offset
    assign next_pc_jal_r = {alu_res[31:1], jalr ? 1'b0 : alu_res[0] };

    // Next PC MUX (selects between next pc coming from conditional branch (or simply pc+4, if branch not taken) or jal/r
    always @*
    begin
        case(jal_r)
            1'b0: next_pc_o = next_pc_branch;
            1'b1: next_pc_o = next_pc_jal_r;
        endcase
    end

    // ALU result
    assign alu_res_o = jal_r ? next_pc_branch : alu_res;

endmodule