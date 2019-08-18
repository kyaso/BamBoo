module cpu_top (
    input clk,
    input rst_n,
    input [31:0] inst_i,
    input [31:0] mem_rdata,
    input halt,
    output [31:0] fetch_addr,
    output [31:0] mem_addr,
    output [1:0] mem_byte_sel,
    output [31:0] mem_wdata,
    output mem_we
);
    wire [31:0] ir;
    wire [4:0] rs1_sel;
    wire [4:0] rs2_sel;
    wire [4:0] rd_sel;
    wire load;
    wire store;
    wire [31:0] imm;
    wire [2:0] funct3;
    wire funct7;
    wire [1:0] op_sel;
    wire lui;
    wire auipc;
    wire branch;
    wire jal;
    wire jalr;
    wire reg_we;
    wire [31:0] rs1_val;
    wire [31:0] rs2_val;
    wire [31:0] pc;
    wire [31:0] npc;
    wire [31:0] alu_res;
    wire [31:0] reg_w_val;
    wire [31:0] bitselect_i;
    wire [31:0] bitselect_o;

    assign fetch_addr = npc;
    assign mem_addr = alu_res;
    assign mem_byte_sel = funct3[1:0];
    assign mem_wdata = bitselect_o;
    assign mem_we = store;
    
    assign reg_w_val = load ? bitselect_o : alu_res;
    assign bitselect_i = load ? mem_rdata : rs2_val;

    fetch i_fetch (
        .clk(clk),
        .rst_n(rst_n),
        .inst_i(inst_i),
        .next_pc_i(npc),
        .halt(halt),
        .pc_o(pc),
        .ir_o(ir)
    );

    decoder i_decoder (
        .ir_i(ir),
        .rs1(rs1_sel),
        .rs2(rs2_sel),
        .rd(rd_sel),
        .reg_we(reg_we),
        .load(load),
        .store(store),
        .imm_o(imm),
        .funct7(funct7),
        .funct3(funct3),
        .op_sel(op_sel),
        .lui(lui),
        .auipc(auipc),
        .branch(branch),
        .jal(jal),
        .jalr(jalr)
    );

    regfile i_regfile (
        .clk(clk),
        .rs1_sel(rs1_sel),
        .rs2_sel(rs2_sel),
        .rd_sel(rd_sel),
        .we(reg_we),
        .w_val_i(reg_w_val),
        .rs1_val_o(rs1_val),
        .rs2_val_o(rs2_val)
    );

    bitselect i_bitselect (
        .val_i(bitselect_i),
        .val_o(bitselect_o),
        .funct3(funct3)
    );

    execute i_execute (
        .rs1_i(rs1_val),
        .rs2_i(rs2_val),
        .pc_i(pc),
        .imm_i(imm),
        .funct3(funct3),
        .funct7(funct7),
        .op_sel(op_sel),
        .lui(lui),
        .auipc(auipc),
        .branch(branch),
        .jal(jal),
        .jalr(jalr),
        .load(load),
        .store(store),
        .next_pc_o(npc),
        .res_o(alu_res)
    );

endmodule;