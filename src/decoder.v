module decoder (
    /* verilator lint_off UNUSED */
    input [31:0] ir_i,
    /* verilator lint_on UNUSED */
    output [4:0] rs1,
    output [4:0] rs2,
    output [4:0] rd,
    output reg_we,
    output load,
    output store,
    output [31:0] imm_o,
    output funct7,
    output [2:0] funct3,
    output [1:0] op_sel,
    output lui,
    output auipc,
    output branch,
    output jalr
);

    localparam  OP_IMM  = 5'b00100,
                LUI     = 5'b01101,
                AUIPC   = 5'b00101,
                OP      = 5'b01100,
                JAL     = 5'b11011,
                JALR    = 5'b11001,
                BRANCH  = 5'b11000,
                LOAD    = 5'b00000,
                STORE   = 5'b01000,

                OP1_REG = 1'b0,
                OP1_PC  = 1'b1,
                OP2_REG = 1'b0,
                OP2_IMM = 1'b1;

    assign rs1 = ir_i[19:15];
    assign rs2 = ir_i[24:20];
    assign rd  = ir_i[11:7];
    assign funct3 = ir_i[14:12];
    assign funct7 = ir_i[30];

    always @*
    begin
        // Default values
        op_sel[0] = OP1_REG;
        op_sel[1] = OP2_REG;
        reg_we = 1'b0;
        lui = 1'b0;
        auipc = 1'b0;
        branch = 1'b0;
        jalr = 1'b0;
        load = 1'b0;
        store = 1'b0;

        case (ir_i[6:2])
            OP_IMM:
                begin
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                end
            OP:
                begin
                    reg_we = 1'b1;
                end
            JAL:
                begin
                    op_sel[0] = OP1_PC;
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                    jalr = 1'b1;
                end
            JALR:
                begin
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                    jalr = 1'b1;
                end
            BRANCH:
                begin
                    branch = 1'b1;
                end
            LUI:
                begin
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                    lui = 1'b1;
                end
            AUIPC:
                begin
                    op_sel[0] = OP1_PC;
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                    auipc = 1'b1;
                end
            LOAD:
                begin
                    op_sel[1] = OP2_IMM;
                    reg_we = 1'b1;
                    load = 1'b1;
                end
            STORE:
                begin
                    op_sel[1] = OP2_IMM;
                    store = 1'b1;
                end
            default:
                begin

                end

        endcase
    end

    /*
    Immediate decoding
    */
    always @*
    begin
        case (ir_i[6:2])
            OP_IMM, JALR, LOAD:
                begin
                    imm_o = { {20{ir_i[31]}}, ir_i[31:20] };
                end
            JAL:
                begin
                    imm_o = { {11{ir_i[31]}}, ir_i[31], ir_i[19:12], ir_i[20], ir_i[30:21], 1'b0 };
                end
            BRANCH:
                begin
                    imm_o = { {19{ir_i[31]}}, ir_i[31], ir_i[7], ir_i[30:25], ir_i[11:8], 1'b0 };
                end
            LUI, AUIPC:
                begin
                    imm_o = { ir_i[31:12], {12{1'b0}}};
                end
            STORE:
                begin
                    imm_o = { {20{ir_i[31]}}, ir_i[31:25], ir_i[11:7] };
                end
            default:
                begin
                    imm_o = 32'b0;
                end

        endcase
    end

endmodule