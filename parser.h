#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <map>
using namespace std;
extern Lexer mylexer;
class Instruction
{
public:
	Ins_Op ins_op;
	string rs1, rs2, rd;
	Instruction(Ins_Op ins_opp = ASM_EMPTY, string rdd= "", string rss1 = "", string rss2 = ""): ins_op(ins_opp), rs1(rss1), rs2(rss2), rd(rdd){}
};
class Instruction_Selector
{
public:
	map<int, Instruction*> instructions;
	map<string, int> symbol_table;
	map<string, int> ini_symbol_table;
	int tmpcnt = 0;
	int tmpcnti = 0;
	map<int, int> linenum_to_instructions_table;
	string opname[100] = {"ASM_EMPTY", "ASM_EXIT_NUM", "ASM_EXIT_ID", "ASM_INPUT", "ASM_GOTO",
	"ASM_LET_NUM", "ASM_LET_ID", "ASM_JMP_ONCOND", "ASM_JMP_NOTONCOND",
	"ASM_AND", "ASM_ANDI",
	"ASM_OR", "ASM_ORI",
	"ASM_ADDI", "ASM_ADD",
	"ASM_SUBI", "ASM_SUB",
	"ASM_MULTII", "ASM_MULTI",
	"ASM_DIVIDEI", "ASM_DIVIDE",
	"ASM_MODI", "ASM_MOD",
	"ASM_XIAOYUI", "ASM_XIAOYU",
	"ASM_XIAOYUDENGYUI", "ASM_XIAOYUDENGYU",
	"ASM_DAYUI", "ASM_DAYU",
	"ASM_DAYUDENGYUI", "ASM_DAYUDENGYU",
	"ASM_NOT_EQUAL", "ASM_NOT_EQUALI",
	"ASM_EQUAL", "ASM_EQUALI"};
	void push(Instruction* ins)
	{
		instructions[++tmpcnti] = ins;
		cout << "Instruction " << tmpcnti << ": " << opname[ins->ins_op]<< " " << ins->rd << " " << ins->rs1 << " " << ins->rs2 << endl;
	}
	void linenum_to_instruction(int ln = -1, int insn = -1)
	{
		if(insn == -1)
			insn = tmpcnti + 1;
		linenum_to_instructions_table[ln] = insn;
		cout << "linenum " << ln << " to ins " << insn << endl; 
	}
	void add_symbol_table(string a, int b, string &c)
	{
		
		if(a == "inbuild_tmp")
		{
			tmpcnt++;
			c = a + to_string(tmpcnt);
		}
		else 
			c = a;
		symbol_table[c] = b;
		cout << "st: " << c << " = " << b << endl;
	}
};
extern Instruction_Selector my_instruction_selector;






class unknown_op{};
class AST
{
public:
	virtual ~AST(){};
};
//expr相关ast
class IdentifierAST: public AST
{
	string name;
public:
	IdentifierAST(string &Name):name(Name){}
};
class ExprAST: public AST
{
public:
	int type;
};
class NumberAST: public ExprAST
{
public:
	int val;
	NumberAST(int Val):val(Val){type = 1;}
	string to_instruction()
	{
		return to_string(val);
	}
};
class IdentifierExprAST: public ExprAST
{
	string name;
public:
	IdentifierExprAST(string &Name):name(Name){type = 2;}
	string to_instruction()
	{
		return name;
	}
};
class BinartAST_to_instruction_error{};
Ins_Op BinaryAST_Opname[100] = {ASM_EMPTY, ASM_OR, ASM_AND, ASM_NOT_EQUAL, ASM_EQUAL, ASM_EMPTY, ASM_XIAOYU, ASM_XIAOYUDENGYU, ASM_DAYU, ASM_DAYUDENGYU, ASM_ADD, ASM_SUB, ASM_MULTI, ASM_DIVIDE, ASM_MOD};
Ins_Op BinaryAST_Opnamei[100] = {ASM_EMPTY,ASM_ORI,ASM_ANDI,ASM_NOT_EQUALI,ASM_EQUALI,ASM_EMPTY, ASM_XIAOYUI,ASM_XIAOYUDENGYUI,ASM_DAYUI,ASM_DAYUDENGYUI,ASM_ADDI,ASM_SUBI,ASM_MULTII,ASM_DIVIDEI,ASM_MODI};
class BinaryAST: public ExprAST
{
public:
	Optype optype;
	ExprAST *lhs, *rhs;
	BinaryAST(Optype optypee, ExprAST *lhss, ExprAST *rhss): optype(optypee), lhs(lhss), rhs(rhss){type = 3;}
	string to_instruction()
	{
		string tmpstrleft, tmpstrright; 
		if(lhs->type == 1)
			tmpstrleft = dynamic_cast<NumberAST*>(lhs)->to_instruction();
		else if(lhs->type == 2)
			tmpstrleft = dynamic_cast<IdentifierExprAST*>(lhs)->to_instruction();
		else if(lhs->type == 3)
			tmpstrleft = dynamic_cast<BinaryAST*>(lhs)->to_instruction();
		if(rhs->type == 1)
			tmpstrright = dynamic_cast<NumberAST*>(rhs)->to_instruction();
		else if(rhs->type == 2)
			tmpstrright = dynamic_cast<IdentifierExprAST*>(rhs)->to_instruction();
		else if(rhs->type == 3)
			tmpstrright = dynamic_cast<BinaryAST*>(rhs)->to_instruction();
		if(lhs->type == 1 && rhs->type == 1)
		{
			string retname;
			my_instruction_selector.add_symbol_table("inbuild_tmp", stoi(tmpstrleft) + stoi(tmpstrright), retname);
			return retname;
		}
		else if(lhs->type == 1 && (rhs->type == 2 || rhs->type == 3))
		{
			string tmpname;
			my_instruction_selector.add_symbol_table("inbuild_tmp", 0, tmpname);
			Instruction *tmpinstruction = new Instruction(BinaryAST_Opnamei[optype], tmpname, tmpstrright, tmpstrleft);
			my_instruction_selector.push(tmpinstruction);
			return tmpname;
		}
		else if((lhs->type == 2 || lhs->type == 3) && rhs->type == 1)
		{
			string tmpname;
			my_instruction_selector.add_symbol_table("inbuild_tmp", 0, tmpname);
			Instruction *tmpinstruction = new Instruction(BinaryAST_Opnamei[optype], tmpname, tmpstrleft, tmpstrright);
			my_instruction_selector.push(tmpinstruction);
			return tmpname;
		}
		else if((lhs->type == 2 || lhs->type == 3) && (rhs->type == 2 || rhs->type == 3))
		{
			string tmpname;
			my_instruction_selector.add_symbol_table("inbuild_tmp", 0, tmpname);
			Instruction *tmpinstruction = new Instruction(BinaryAST_Opname[optype], tmpname, tmpstrleft, tmpstrright);
			my_instruction_selector.push(tmpinstruction);
			return tmpname;
		}
		throw BinartAST_to_instruction_error();
	}
};
ExprAST *Error(char *Str)
{
	fprintf(stderr, "Error: %s\n", Str);
	return nullptr;
}






//expr相关parse函数
ExprAST* ParseExpression();
Token CurTok;
ExprAST* ParseNumberExpr()
{
	ExprAST *Result = new NumberAST(CurTok.num);
	CurTok = mylexer.gettok();
	return Result;
}
ExprAST *ParseParenExpr()
{
	CurTok = mylexer.gettok();
	ExprAST *V = ParseExpression();
	if (!V)
		return nullptr;
	if(CurTok.tokentype != tok_youkuohao)
		return Error("expected )");
	CurTok = mylexer.gettok();
	return V;
}
IdentifierExprAST* ParseIdentifierExpr()
{
	string IdName = CurTok.val;
	CurTok = mylexer.gettok();
	IdentifierExprAST* identifierexprAST = new IdentifierExprAST(IdName);
	return identifierexprAST;
}
ExprAST* ParsePrimary()
{
	if(CurTok.tokentype == tok_identifier)
		return ParseIdentifierExpr();
	else if(CurTok.tokentype == tok_number)
		return ParseNumberExpr();
	else if(CurTok.tokentype == tok_zuokuohao)
		return ParseParenExpr();
	else
		return Error("unknown token when expecting an expression");
} 
// 运算符优先级
int GetTokPrecedence()
{
	if(CurTok.optype == op_or)
		return 10;
	if(CurTok.optype == op_and)
		return 20;
	if(CurTok.optype == op_not_equal || CurTok.optype == op_equal)
		return 30;
	if(CurTok.optype == op_xiaoyu || CurTok.optype == op_xiaoyudengyu || CurTok.optype == op_dayu || CurTok.optype == op_dayudengyu)
		return 40;
	if(CurTok.optype == op_add || CurTok.optype == op_sub)
		return 60;
	if(CurTok.optype == op_multi || CurTok.optype == op_divide || CurTok.optype == op_mod)
		return 80;
	else
		return -1;
}
ExprAST* ParseBinOpRHS(int PrevPrec, ExprAST *lhs)
{
	while(1)
	{
		int NowPrec = GetTokPrecedence();
		if(NowPrec < PrevPrec)
			return lhs;
		Token BinOp = CurTok;
		CurTok = mylexer.gettok();
		ExprAST *rhs = ParsePrimary();
		if(!rhs)
			return nullptr;
		int NextPrec = GetTokPrecedence();
		if(NowPrec < NextPrec)
		{
			rhs = ParseBinOpRHS(NowPrec+1, rhs);
			if(rhs == 0)
				return nullptr;
		}
		lhs = new BinaryAST(BinOp.optype, lhs, rhs);
	}

}
ExprAST* ParseExpression()
{
	ExprAST *lhs = ParsePrimary();
	// cout << "fgsfsdfs" << dynamic_cast<NumberAST*>(lhs)->val << endl;
	if(!lhs)
		return nullptr;
  return ParseBinOpRHS(0, lhs);
}





//statement相关ast
// enum New_CFG_Node_Sign
// {
// 	sign_empty, sign_goto, sign_if
// };
class StatementAST: public AST
{
public:
	virtual void to_instruction(int a) = 0;
};
class REMStatementAST: public StatementAST
{
public:
	void to_instruction(int a){}
};
class LetStatementAST: public StatementAST
{
public:
	ExprAST *lvalue;
	ExprAST *rvalue;
	LetStatementAST(){};
	LetStatementAST(ExprAST* lvaluee, ExprAST* rvaluee): lvalue(lvaluee), rvalue(rvaluee){};
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		string tmpstrleft = dynamic_cast<IdentifierExprAST*>(lvalue)->to_instruction();
		if(rvalue->type == 1)
		{
			string tmpstrright = dynamic_cast<NumberAST*>(rvalue)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_LET_NUM, tmpstrleft, tmpstrright);
			my_instruction_selector.push(tmpinstruction);
		}
		else if(rvalue->type == 2)
		{
			string tmpstrright = dynamic_cast<IdentifierExprAST*>(rvalue)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_LET_NUM, tmpstrleft, tmpstrright);
			my_instruction_selector.push(tmpinstruction);
		}
		else if(rvalue-> type == 3)
		{
			string tmpstrright = dynamic_cast<BinaryAST*>(rvalue)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_LET_ID, tmpstrleft, tmpstrright);
			my_instruction_selector.push(tmpinstruction);
		}
	}
};
class InputStatementAST: public StatementAST
{
	vector<IdentifierExprAST*> identifiers;
public:
	InputStatementAST(){};
	void push(IdentifierExprAST* identifierExprAST)
	{
		identifiers.push_back(identifierExprAST);
	}
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		for(auto it = identifiers.begin(); it != identifiers.end(); it++)
		{
			string tmpstr;
			my_instruction_selector.add_symbol_table((*it)->to_instruction(), 0, tmpstr);
			Instruction *tmpinstruction = new Instruction(ASM_INPUT, tmpstr);
			my_instruction_selector.push(tmpinstruction);
		}
	}
};
class ExitStatementAST: public StatementAST
{
	ExprAST* exit_expr;
public:
	ExitStatementAST(){};
	ExitStatementAST(ExprAST* exit_exprr): exit_expr(exit_exprr){};
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		if(exit_expr->type == 1)
		{
			string tmpstr = dynamic_cast<NumberAST*>(exit_expr)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_EXIT_NUM, tmpstr);
			my_instruction_selector.push(tmpinstruction);
		}
		else if(exit_expr->type == 2)
		{
			string tmpstr = dynamic_cast<IdentifierExprAST*>(exit_expr)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_EXIT_ID, tmpstr);
			my_instruction_selector.push(tmpinstruction);
		}
		else if(exit_expr->type == 3)
		{
			string tmpstr = dynamic_cast<BinaryAST*>(exit_expr)->to_instruction();
			Instruction *tmpinstruction = new Instruction(ASM_EXIT_ID, tmpstr);
			my_instruction_selector.push(tmpinstruction);
		}
	}
};
class GotoStatementAST: public StatementAST
{
public:
	int goto_linenum;
	GotoStatementAST(int a = 0): goto_linenum(a)
	{
		// new_cfg_node_sign = sign_goto;
	};
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		Instruction *tmpinstruction = new Instruction(ASM_GOTO, to_string(goto_linenum));
		my_instruction_selector.push(tmpinstruction);
	}
};
class IfStatementAST: public StatementAST
{
	ExprAST* if_judgement;
	ExprAST* goto_expr;
public:
	IfStatementAST()
	{
		// new_cfg_node_sign = sign_if;
	}
	IfStatementAST(ExprAST* if_judgementt, ExprAST* goto_exprr): if_judgement(if_judgementt), goto_expr(goto_exprr)
	{
		// new_cfg_node_sign = sign_if;
	}
	int goto_value()
	{
		return dynamic_cast<NumberAST*>(goto_expr)->val;
	}
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		string tmpstrjudgement = dynamic_cast<BinaryAST*>(if_judgement)->to_instruction();
		Instruction *tmpinstruction = new Instruction(ASM_JMP_ONCOND, to_string(goto_value()), tmpstrjudgement);
		my_instruction_selector.push(tmpinstruction);
	}
};
class ForStatementAST: public StatementAST
{
	ExprAST* for_judgement;
	LetStatementAST* for_todo;
	map<int, StatementAST*> statements;
	vector<int> linenums;
public:
	ForStatementAST()
	{
		// new_cfg_node_sign = 1;
	};
	ForStatementAST(LetStatementAST* for_todoo, ExprAST* for_judgementt): for_todo(for_todoo), for_judgement(for_judgementt)
	{
		// new_cfg_node_sign = 1;
	};
	void push(int linenum, StatementAST* statement)
	{
		statements[linenum] = statement;
		linenums.push_back(linenum);
	}
	void to_instruction(int ln)
	{
		my_instruction_selector.linenum_to_instruction(ln);
		string tmpstrjudgement = dynamic_cast<BinaryAST*>(for_judgement)->to_instruction();
		int llnn = linenums.back();
		Instruction *tmpinstruction = new Instruction(ASM_JMP_NOTONCOND, to_string(llnn+2), tmpstrjudgement);
		my_instruction_selector.push(tmpinstruction);
		for(auto it = statements.begin(); it != statements.end(); it++)
			it->second->to_instruction(it->first);
		for_todo->to_instruction(-1);
		Instruction *tmpinstruction2 = new Instruction(ASM_GOTO, to_string(ln));
		my_instruction_selector.push(tmpinstruction2);
	}
};





//parser入口类
class ProgramAST: public AST
{
public:
	map<int, StatementAST*> statements;
	vector<int> linenums;
	void push(int linenum, StatementAST* statement)
	{
		statements[linenum] = statement;
		linenums.push_back(linenum);
	}
	void to_instruction()
	{
		for(auto it = statements.begin(); it != statements.end(); it++)
		{
			it->second->to_instruction(it->first);
		}
	}
};






//statement相关parse
class linenumerror
{
public:
	linenumerror()
	{
		cout << "Expecting linenum!" << endl;
	}
};
StatementAST* ParseStatement();
InputStatementAST* ParseStatementInput()
{
	CurTok = mylexer.gettok();
	InputStatementAST* myinputstatementAST = new InputStatementAST();
	myinputstatementAST->push(ParseIdentifierExpr());
	while(CurTok.tokentype == tok_comma)
	{
		CurTok = mylexer.gettok();
		myinputstatementAST->push(ParseIdentifierExpr());
	}
	return myinputstatementAST;
}
GotoStatementAST* ParseStatementGoto()
{
	CurTok = mylexer.gettok();
	GotoStatementAST* mygotostatementAST = new GotoStatementAST(CurTok.num);
	CurTok = mylexer.gettok();
	return mygotostatementAST;

}
ExitStatementAST* ParseStatementExit()
{
	CurTok = mylexer.gettok();
	ExitStatementAST* myexitstatementAST = new ExitStatementAST(ParseExpression());
	return myexitstatementAST;
}
IfStatementAST* ParseStatementIf()
{
	CurTok = mylexer.gettok();
	ExprAST* judgeexpr = ParseExpression();
	CurTok = mylexer.gettok();
	ExprAST* goto_statement = ParseExpression(); 
	IfStatementAST* myifstatementAST = new IfStatementAST(judgeexpr, goto_statement);
	return myifstatementAST;

}
LetStatementAST* ParseStatementLet()
{
	CurTok = mylexer.gettok();
	ExprAST* lvalue = ParseExpression();
	CurTok = mylexer.gettok(); // "="
	ExprAST* rvalue = ParseExpression();
	LetStatementAST *myletstatementAST = new LetStatementAST(lvalue, rvalue);
	return myletstatementAST;

}
ForStatementAST* ParseStatementFor()
{
	LetStatementAST* myletstatementAST = ParseStatementLet();
	CurTok = mylexer.gettok();
	ExprAST* myexprAST = ParseExpression();
	ForStatementAST* myforstatementAST = new ForStatementAST(myletstatementAST, myexprAST);
	while(1)
	{
		if(CurTok.tokentype != tok_number)
		{
			cout << CurTok.tokentype << endl;
			throw linenumerror();
		}
		int ln = CurTok.num;
		CurTok = mylexer.gettok();
		if(CurTok.tokentype == tok_endfor)
			break;
		myforstatementAST->push(ln, ParseStatement());
	}
	CurTok = mylexer.gettok();
	return myforstatementAST;
}
REMStatementAST* ParseStatementRem()
{
	CurTok = mylexer.gettok();
	REMStatementAST* myremstatementAST = new REMStatementAST();
	return myremstatementAST;
}
class unknownstatement{};
StatementAST* ParseStatement()
{
	if(CurTok.tokentype == tok_input)
		return ParseStatementInput();
	if(CurTok.tokentype == tok_goto)
		return ParseStatementGoto();
	if(CurTok.tokentype == tok_exit)
		return ParseStatementExit();
	if(CurTok.tokentype == tok_if)
		return ParseStatementIf();
	if(CurTok.tokentype == tok_for)
		return ParseStatementFor();
	if(CurTok.tokentype == tok_let)
		return ParseStatementLet();
	if(CurTok.tokentype == tok_rem)
		return ParseStatementRem();
	else 
		throw unknownstatement();

}
void parse(ProgramAST &myProgramAST)
{
	CurTok = mylexer.gettok();
	while(1)
	{
		if(CurTok.tokentype == tok_eof)
			break;
		if(CurTok.tokentype != tok_number)
		{
			cout << CurTok.tokentype << endl;
			throw linenumerror();
		}
		int ln = CurTok.num;
		CurTok = mylexer.gettok();
		myProgramAST.push(ln, ParseStatement());
	}
}
