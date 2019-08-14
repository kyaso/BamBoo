module alu (
    input [31:0] op1,
    input [31:0] op2,
    input [2:0] funct3,
    input funct7,
    input jal_r,
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


    reg equal; // inv = NE
    reg less_than; // inv = GE
    reg less_than_u; // inv = GEU

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
            3'b010: alu_res = (less_than) ? 32'b1 : 32'b0;          // SLT(I)
            3'b011: alu_res = (less_than_u) ? 32'b1 : 32'b0;        // SLT(I)U
            3'b100: alu_res = op1 ^ op2;                            // XOR(I)
            3'b110: alu_res = op1 | op2;                            // OR(I)
            3'b111: alu_res = op1 & op2;                            // AND(I)
            3'b001: alu_res = op1 << op2[4:0];                      // SLL(I)
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
        
        if(jal_r | load | store | auipc)
            alu_res = op1 + op2;
    end

    // Comparison
    always @*
    begin
        if(op1 == op2)                          // EQ
            equal = 1'b1;
        else                                    // NE
            equal = 1'b0;

        if($signed(op1) < $signed(op2))         // LT
            less_than = 1'b1;
        else                                    // GE
            less_than = 1'b0;

        if(op1 < op2)                           // LTU
            less_than_u = 1'b1;
        else                                    // GEU
            less_than_u = 1'b0;
    end

    // Branch
    always @*
    begin
        case(funct3)
            3'b000: take_branch = (equal) ? 1'b1 : 1'b0;                // BEQ
            3'b001: take_branch = (!equal) ? 1'b1 : 1'b0;               // BNE
            3'b100: take_branch = (less_than) ? 1'b1 : 1'b0;            // BLT
            3'b101: take_branch = (!less_than) ? 1'b1 : 1'b0;           // BGE
            3'b110: take_branch = (less_than_u) ? 1'b1 : 1'b0;          // BLTU
            3'b111: take_branch = (!less_than_u) ? 1'b1 : 1'b0;         // BGEU
            default: take_branch = 1'b0;
        endcase
    end

endmodule