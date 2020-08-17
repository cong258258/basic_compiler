#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <map>
#include "tmp.h"
#include "lexer.h"
#include "parser.h"
#include "translator.h"
Lexer mylexer(cin);
ProgramAST myProgramAST;
Instruction_Selector my_instruction_selector;
Trans_instructionman mytrans_instructionman;
Trans_registerman mytrans_registerman;
Trans_memoryman mytrans_memoryman;
Translator mytranslator;
using namespace std;
int main()
{
	parse(myProgramAST);
	myProgramAST.to_instruction();
	mytrans_memoryman.translate_it();
	mytranslator.do_it();
	return 0;
}