#include <iomanip>
#include <deque>
extern Instruction_Selector my_instruction_selector;
unsigned subcode(unsigned &code, int from, int to)
{
	unsigned length = to - from + 1;
	unsigned tpnum = (1 << length) - 1;
	return (code >> from) & tpnum;
}
struct INS
{
	unsigned parta;
	int partb;
	int instype;
};
unsigned realoutput(unsigned code)
{
	printf("%02X ", code & 0b11111111);
	printf("%02X ", (code & (0b11111111 << 8)) >> 8);
	printf("%02X ", (code & (0b11111111 << 16)) >> 16);
	printf("%02X ", (code & (0b11111111 << 24)) >> 24);
}
class Trans_instructionman
{
public:
	int offset, cnt;
	map<int, int> instructions_to_ASM_table;
	vector<INS> instructions;
	int next_asm_num;
	Trans_instructionman()
	{
		offset = 0;
		cnt = offset;
		next_asm_num = -1;
	};
	void push(INS a)
	{
		instructions_to_ASM_table[next_asm_num] = cnt;
		// cout << dec << cnt << "   " << next_asm_num << endl;
		next_asm_num = -1;
		instructions.push_back(a);
		cnt += 4;
	}
	void final_realoutput()
	{
		for(auto i = instructions.begin(); i != instructions.end(); i++)
		{
			if(i->partb == -1)
				realoutput(i->parta);
			else if(i->instype == 0)
			{
				int partaa = i->parta + (instructions_to_ASM_table[my_instruction_selector.linenum_to_instructions_table[i->partb]] << 20);
				realoutput(partaa);
			}
			else if(i->instype == 1)
			{
				unsigned tpcd = instructions_to_ASM_table[my_instruction_selector.linenum_to_instructions_table[i->partb]];
				unsigned tpcdd = (subcode(tpcd, 12, 12) << 31) + (subcode(tpcd, 5, 10) << 25) + (subcode(tpcd, 1, 4) << 8) + (subcode(tpcd, 11, 11) << 7);
				int partaa = i->parta + tpcdd;
				realoutput(partaa);
			}
		}
	}
};
extern Trans_instructionman mytrans_instructionman;
void output(unsigned code, int partb = -1, int instype = -1)
{
	mytrans_instructionman.push({code, partb, instype});
}
unsigned output_new_block(unsigned code = 0)
{
	if(code != 0)
		cout << endl;
	cout << "@" << hex << setw(8) << setfill('0') << code << endl;
}
unsigned Ins2_ADDI(unsigned rd, unsigned rs1, unsigned imm);
unsigned Ins2_ADD(unsigned rd, unsigned rs1, unsigned rs2);
unsigned Ins2_LW(unsigned rd, unsigned rs1, unsigned imm);
unsigned Ins2_SW(unsigned imm, unsigned rs1, unsigned rs2);
class Trans_memoryman
{
public:
	int offset;
	map<string, int> symbol_table_to_memory_addr;
	Trans_memoryman(){};
	void translate_it()
	{
		cout << "----------------------------------" << endl << endl;
		offset = 1024;
		for(auto i = my_instruction_selector.symbol_table.begin(); i != my_instruction_selector.symbol_table.end(); i++)
		{
			offset += 4;
			symbol_table_to_memory_addr[i->first] = offset;
			cout << "var " << i->first << "  @" << offset << endl;
		}
		cout << "----------------------------------" << endl << endl;
	}
};
extern Trans_memoryman mytrans_memoryman;
class Trans_registerman
{
public:
	int reg_occupied[32];
	int nearest_not_occupied = 2;
	map<string, int> symbol_table_to_register;
	deque<string> var_in_reg;
	Trans_registerman()
	{
		memset(reg_occupied, 0, sizeof(reg_occupied));
	}
	int assign_new(string a)
	{
		int i;
		for(i = 2; reg_occupied[i] == 1 && i <= 31; i++);
			nearest_not_occupied = i;
		if(nearest_not_occupied == 32)
		{
			string tostore = var_in_reg.front();
			var_in_reg.pop_front();
			output(Ins2_ADDI(1, 0, mytrans_memoryman.symbol_table_to_memory_addr[tostore]));
			// cout << "tostoreaddr: " << dec << mytrans_memoryman.symbol_table_to_memory_addr[tostore] << endl;
			output(Ins2_SW(0, 1, symbol_table_to_register[tostore]));
			int emptyreg = symbol_table_to_register[tostore];
			nearest_not_occupied = emptyreg;
			symbol_table_to_register[tostore] = 0;
		}
		symbol_table_to_register[a] = nearest_not_occupied;
		var_in_reg.push_back(a);
		reg_occupied[nearest_not_occupied] = 1;
		my_instruction_selector.ini_symbol_table[a] = 1; 
		return nearest_not_occupied;
	}
	int assign(string a, int b = -1)
	{
		int i;
		for(i = 2; reg_occupied[i] == 1 && i <= 31; i++);
			nearest_not_occupied = i;
		if(nearest_not_occupied == 32 && b == -1)
		{
			string tostore = var_in_reg.front();
			var_in_reg.pop_front();
			output(Ins2_ADDI(1, 0, mytrans_memoryman.symbol_table_to_memory_addr[tostore]));
			// cout << "tostoreaddr: " << mytrans_memoryman.symbol_table_to_memory_addr[tostore] << endl;
			output(Ins2_SW(0, 1, symbol_table_to_register[tostore]));
			int emptyreg = symbol_table_to_register[tostore];
			nearest_not_occupied = emptyreg;
			symbol_table_to_register[tostore] = 0;
		}
		if(b == -1)
			b = nearest_not_occupied;
		symbol_table_to_register[a] = b;
		var_in_reg.push_back(a);
		reg_occupied[b] = 1; 
		output(Ins2_LW(b, 0, mytrans_memoryman.symbol_table_to_memory_addr[a]));
		return b;
	}
	int find(string a)
	{
		// cout << "symbol_table_to_register" << symbol_table_to_register[a] << endl;
		if(my_instruction_selector.ini_symbol_table[a] == 0)
			return assign_new(a);
		else if(symbol_table_to_register[a] != 0)
			return symbol_table_to_register[a];
		else
			return assign(a);
		// {
		// 	for(i = 2; reg_occupied[i] == 1 && i <= 31; i++);
		// 	nearest_not_occupied = i;
		// 	if(nearest_not_occupied == 32 && b = -1)
		// 	{
		// 		string tostore = var_in_reg.front();
		// 		var_in_reg.pop_front();
		// 		output(Ins2_ADDI(1, 0, mytrans_memoryman.symbol_table_to_memory_addr[tostore]));
		// 		cout << "tostoreaddr: " << mytrans_memoryman.symbol_table_to_memory_addr[tostore] << endl;
		// 		output(Ins2_SW(symbol_table_to_register[tostore]), 1, 0);
		// 		int emptyreg = symbol_table_to_register[tostore];
		// 		nearest_not_occupied = emptyreg;
		// 		symbol_table_to_register[tostore] = 0;
		// 		reg_occupied[nearest_not_occupied] = 0;
		// 	}
		// 	if(b == -1)
		// 		b = nearest_not_occupied;
		// 	symbol_table_to_register[a] = b;
		// 	var_in_reg.push_back(a);
		// 	reg_occupied[b] = 1;
		// 	symbol_table_to_memory_addr[a]
		// }
	}
};
extern Trans_registerman mytrans_registerman;
unsigned Ins2_Input(unsigned rd)
{
	unsigned code = 0;
	code = (rd << 7) + 0b1111111;
	return code;
}
unsigned Ins2_JALR(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned code = 0;
	code = (imm << 20) + (rs1 << 15) + (rd << 7) + 0b1100111;
	return code;
}
unsigned Ins2_LW(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned code = 0;
	code = (imm << 20) + (rs1 << 15) + (rd << 7) + 0b010000000000011;
	return code;
}
unsigned Ins2_SW(unsigned imm, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code =  (subcode(imm, 5, 11) << 20) + (rs2 << 20) + (rs1 << 15) + (subcode(imm, 0, 4) << 7) + 0b010000000100011;
	return code;
}
unsigned Ins2_ADDI(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned code = 0;
	code = (imm << 20) + (rs1 << 15) + (rd << 7) + 0b0010011;
	return code;
}
unsigned Ins2_ADD(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code = (rs2 << 20) + (rs1 << 15) + (rd << 7) + 0b0110011;
	return code;
}
unsigned Ins2_SUB(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code = (0b0100000 << 25) + (rs2 << 20) + (rs1 << 15) + (rd << 7) + 0b0110011;
	return code;
}
unsigned Ins2_MULTII(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned code = 0;
	code = (imm << 20) + (rs1 << 15) + (rd << 7) + 0b0000001;
	return code;
}
unsigned Ins2_MULTI(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code = (rs2 << 20) + (rs1 << 15) + (rd << 7) + 0b0000010;
	return code;
}
unsigned Ins2_DIVIDEI(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned code = 0;
	code = (imm << 20) + (rs1 << 15) + (rd << 7) + 0b0000100;
	return code;
}
unsigned Ins2_DIVIDE(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code = (rs2 << 20) + (rs1 << 15) + (rd << 7) + 0b0001000;
	return code;
}
unsigned Ins2_SLT(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned code = 0;
	code = (rs2 << 20) + (rs1 << 15) + (rd << 7) + 0b010000000110011;
	return code;
}
unsigned Ins2_BNE(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned code = 0;
	code = (rs2 << 20) + (rs1 << 15) + 0b001000001100011;
	return code;
}
class error_translator{};
class Translator
{
public:
	Translator(){};
	void do_it()
	{
		output_new_block();
		for(auto i = my_instruction_selector.instructions.begin(); i != my_instruction_selector.instructions.end(); i++)
		{
			mytrans_instructionman.next_asm_num = i->first;
			int find1, find2, find3;
			// cout << i->second->ins_op << endl;
			switch(i->second->ins_op)
			{
				case ASM_INPUT:
					output(Ins2_Input(mytrans_registerman.assign_new(i->second->rd)));
					break;
				case ASM_GOTO:
					output(Ins2_JALR(0, 0, 0), stoi(i->second->rd), 0);
					break;
				case ASM_LET_NUM:
					find1 = mytrans_registerman.find(i->second->rd);
					output(Ins2_ADDI(find1, 0, stoi(i->second->rs1)));
					break;
				case ASM_LET_ID:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					output(Ins2_ADDI(find1, find2, 0));
					break;
				case ASM_EXIT_NUM:
					output(Ins2_ADDI(10, 0, stoi(i->second->rd)));
					break;
				case ASM_EXIT_ID:
					output(Ins2_ADD(10, 0, mytrans_registerman.find(i->second->rd)));
					break;
				case ASM_JMP_ONCOND:
					find1 = mytrans_registerman.find(i->second->rs1);
					output(Ins2_BNE(find1, 0, 0), stoi(i->second->rd), 1);
					break;
				case ASM_ADDI:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					output(Ins2_ADDI(find1, find2, stoi(i->second->rs2)));
					break;
				case ASM_ADD:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					find3 = mytrans_registerman.find(i->second->rs2);
					output(Ins2_ADD(find1, find2, find3));
					break;
				case ASM_SUB:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					find3 = mytrans_registerman.find(i->second->rs2);
					output(Ins2_SUB(find1, find2, find3));
					break;
				case ASM_MULTII:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					output(Ins2_MULTII(find1, find2, stoi(i->second->rs2)));
					break;
				case ASM_MULTI:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					find3 = mytrans_registerman.find(i->second->rs2);
					output(Ins2_MULTI(find1, find2, find3));
					break;
				case ASM_DIVIDEI:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					output(Ins2_DIVIDEI(find1, find2, stoi(i->second->rs2)));
					break;
				case ASM_DIVIDE:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					find3 = mytrans_registerman.find(i->second->rs2);
					output(Ins2_DIVIDE(find1, find2, find3));
					break;
				case ASM_XIAOYU:
					find1 = mytrans_registerman.find(i->second->rd);
					find2 = mytrans_registerman.find(i->second->rs1);
					find3 = mytrans_registerman.find(i->second->rs2);
					output(Ins2_SLT(find1, find2, find3));
				default:
					// throw error_translator();
					break;
			}
		}
		mytrans_instructionman.final_realoutput();
		realoutput(0x0ff00513);
		cout << endl;
	}
};
