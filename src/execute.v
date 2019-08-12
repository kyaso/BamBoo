module alu (
    input [31:0] op1,
    input [31:0] op2,
    input [2:0] funct3,
    input funct7,
    input jal,
    input lui,
    input auipc,
    input load,
    input store,
    input has_imm, // Connect op_sel[1]
    output reg take_branch,
    output reg [31:0] alu_res
);

    localparam  EQ  = 3'b000,
                NE  = 3'b001,
                LT  = 3'b010,
                LTU = 3'b011,
                GE  = 3'b100,
                GEU = 3'b101;


    wire [2:0] comp;

    // Arithmetic
    always @*
    begin
        case (funct3)
            3'b000:                                                 // ADD(I), SUB
                begin
                    if(has_imm || (!has_imm & !funct7))
                        alu_res = op1 + op2;
                    else /* !has & funct7 */
                        alu_res = op1 - op2;
                end
            3'b010: alu_res = (comp == LT) ? 32'b1 : 32'b0;         // SLT(I)
            3'b011: alu_res = (comp == LTU) ? 32'b1 : 32'b0;        // SLT(I)U
            3'b100: alu_res = op1 ^ op2;                            // XOR(I)
            3'b110: alu_res = op1 | op2;                            // OR(I)
            3'b111: alu_res = op1 & op2;                            // AND(I)
            3'b001: alu_res = op1 << op2[4:0]                       // SLL(I)
            3'b101:                                                 // SRL(I), SRA(I)
                begin
                    if(!funct7) // SRL(I)
                        alu_res = op1 >> op2[4:0];
                    else        // SRA(I)
                        alu_res = op1 >>> op2[4:0];
                end
        endcase

        if(lui)
            alu_res = op2;
        
        if(jal | load | store | auipc)
            alu_res = op1 + op2;
    end

    // Comparison
    always @*
    begin
        if(op1 == op1)                          // EQ
            comp = EQ;
        else if(op1 != op2)                     // NE
            comp = NE;
        else if($signed(op1) < $signed(op2))    // LT
            comp = LT;
        else if(op1 < op2)                      // LTU
            comp = LTU;
        else if($signed(op1) >= $signed(op2))   // GE
            comp = GE;
        else if(op1 >= op2)                     // GEU
            comp = GEU;
        else
            comp = 3'b111;
    end

    // Branch
    always @*
    begin
        case(funct3)
            3'b000: take_branch = (comp==EQ) ? 1'b1 : 1'b0;         // BEQ
            3'b001: take_branch = (comp==NE) ? 1'b1 : 1'b0;         // BNE
            3'b100: take_branch = (comp==LT) ? 1'b1 : 1'b0;         // BLT
            3'b101: take_branch = (comp==GE) ? 1'b1 : 1'b0;         // BGE
            3'b110: take_branch = (comp==LTU) ? 1'b1 : 1'b0;        // BLTU
            3'b111: take_branch = (comp==GEU) ? 1'b1 : 1'b0;        // BGEU
            default: take_branch = 1'b0;
        endcase
    end

endmodule

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