enum Ins_Op
{
	ASM_EMPTY, ASM_EXIT_NUM, ASM_EXIT_ID, ASM_INPUT, ASM_GOTO,
	ASM_LET_NUM, ASM_LET_ID, ASM_JMP_ONCOND, ASM_JMP_NOTONCOND,
	ASM_AND, ASM_ANDI,
	ASM_OR, ASM_ORI,
	ASM_ADDI, ASM_ADD,
	ASM_SUBI, ASM_SUB,
	ASM_MULTII, ASM_MULTI,
	ASM_DIVIDEI, ASM_DIVIDE,
	ASM_MODI, ASM_MOD,
	ASM_XIAOYUI, ASM_XIAOYU,
	ASM_XIAOYUDENGYUI, ASM_XIAOYUDENGYU,
	ASM_DAYUI, ASM_DAYU,
	ASM_DAYUDENGYUI, ASM_DAYUDENGYU,
	ASM_NOT_EQUAL, ASM_NOT_EQUALI,
	ASM_EQUAL, ASM_EQUALI
};
enum Ins2_Kind {u, j, i, b, s, r, ii, empty};
enum Ins2_Op
{
	EMPTY,
	LUI, AUIPC,   // u
	JAL,     // j
	JALR,    // i
	BEQ, BNE, BLT, BGE, BLTU, BGEU, // b
	LB, LH, LW, LBU, LHU, //i，加载指令
	SB, SH, SW, //s，保存指令
	ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, // i，立即数运算
	ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,  // r，运算
	ERR
};
enum Tokentype
{
	tok_empty, tok_identifier, tok_number, tok_op, tok_eof,
	tok_let, tok_input, tok_exit, tok_goto, tok_if, tok_then, tok_for, tok_endfor,
	tok_rem, tok_comma, tok_fenhao, tok_zuokuohao, tok_youkuohao
};
enum Optype
{
	op_empty, op_or, op_and, op_not_equal, op_equal, op_fuzhi,
	op_xiaoyu, op_xiaoyudengyu, op_dayu, op_dayudengyu,
	op_add, op_sub, op_multi, op_divide, op_mod
};