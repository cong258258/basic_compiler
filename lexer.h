#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
using namespace std;
class Token
{
public:
	Tokentype tokentype;
	string val;
	int num;
	Optype optype;
	Token(Tokentype tt = tok_empty, string ss = "", int numm = 0, Optype optypee = op_empty): tokentype(tt), val(ss), num(numm), optype(optypee){};
	~Token() = default;
};
class Lexer
{
	istream &is;
	char LastChar = '\n';
public:
	vector<Token> tokens;
	Token gettok()
	{
		string ss = "";
		while(isspace(LastChar))
			LastChar = is.get();
		// if(LastChar == '\n')
		// {
		// 	ss += LastChar;
		// 	while(1)
		// 	{
		// 		LastChar = is.get();
		// 		if(isdigit(LastChar))
		// 			ss += LastChar;
		// 		else
		// 			break;
		// 	}
		// 	cout << "tok_linenum " << ss << endl;
		// 	return {tok_linenum};
		// }
		// if(LastChar == '\n')
		// {
		// 	int numm;
		// 	is >> numm;
		// 	if(numm == 0)
		// 	{
		// 		cout << "tok_eof" << endl;
		// 		return {tok_eof};
		// 	}
		// 	LastChar = ' ';
		// 	cout << "tok_linenum " << numm << endl;
		// 	return {tok_linenum, "", numm};
		// }
		if(isalpha(LastChar))
		{
			ss += LastChar;
			while(1) 
			{
				LastChar = is.get();
				if(!isalnum(LastChar))
					break;
				else
					ss += LastChar;
			}
			if(ss == "REM")
			{
				while(LastChar != EOF && LastChar != '\n' && LastChar != '\r')
					LastChar = getchar();
				cout << "REM" << endl;
				return {tok_rem};
			}
			else if(ss == "INPUT")
			{
				cout << "tok_input" << endl;
				return {tok_input};
			}
			else if(ss == "LET")
			{
				cout << "tok_let" << endl;
				return {tok_let};
			}
			else if(ss == "EXIT")
			{
				cout << "tok_exit" << endl;
				return {tok_exit};
			}
			else if(ss == "GOTO")
			{
				cout << "tok_goto" << endl;
				return {tok_goto};
			}
			else if(ss == "IF")
			{
				cout << "tok_if" << endl;
				return {tok_if};
			}
			else if(ss == "THEN")
			{
				cout << "tok_then" << endl;
				return {tok_then};
			}
			else if(ss == "FOR")
			{
				cout << "tok_for" << endl;
				return {tok_for};
			}
			else if(ss == "END")
			{
				LastChar = is.get();
				if(LastChar != 'F')
					cout << "expected end for" << endl;
				LastChar = is.get();
				if(LastChar != 'O')
					cout << "expected end for" << endl;
				LastChar = is.get();
				if(LastChar != 'R')
					cout << "expected end for" << endl;
				LastChar = is.get();
				cout << "tok_endfor" << endl;
				return {tok_endfor};
			}
			else
			{
				cout << "tok_identifier " << ss << endl;
				return {tok_identifier, ss};
			}
		}
		else if(isdigit(LastChar))
		{
			ss += LastChar;
			while(1)
			{
				LastChar = is.get();
				if(isdigit(LastChar))
					ss += LastChar;
				else
					break;
			}
			cout << "tok_number " << ss << endl;
			return {tok_number, "", stoi(ss)};
		}
		else if(LastChar == EOF)
		{
			cout << "tok_eof" << endl;
			return {tok_eof};
		}
		else if(LastChar == ',')
		{
			cout << "tok_comma" << endl;
			LastChar = is.get();
			return {tok_comma};
		}
		else if(LastChar == ';')
		{
			cout << "tok_fenhao" << endl;
			LastChar = is.get();
			return {tok_fenhao};
		}
		else if(LastChar == '(')
		{
			cout << "tok_zuokuohao" << endl;
			LastChar = is.get();
			return {tok_zuokuohao};
		}
		else if(LastChar == ')')
		{
			cout << "tok_youkuohao" << endl;
			LastChar = is.get();
			return {tok_youkuohao};
		}
		else
		{
			Tokentype tmptok = tok_op;
			Optype tmpop;
			if(LastChar == '+')
			{
				tmpop = op_add;
				LastChar = is.get();
			}
			else if(LastChar == '-')
			{
				LastChar = is.get();
				if(isdigit(LastChar))
				{
					ss += "-";
					ss += LastChar;
					while(1)
					{
						LastChar = is.get();
						if(isdigit(LastChar))
							ss += LastChar;
						else
							break;
					}
					cout << "tok_number " << ss << endl;
					return {tok_number, "", 123};
				}
				else
				{
					tmpop = op_sub;
					cout << "tok_op" << " " << tmpop << endl;
					return {tok_op, "", 0, tmpop};
				}	
			}
			else if(LastChar == '*')
			{
				tmpop = op_multi;
				LastChar = is.get();
			}
			else if(LastChar == '/')
			{
				tmpop = op_divide;
				LastChar = is.get();
			}
			else if(LastChar == '%')
			{
				tmpop = op_mod;
				LastChar = is.get();
			}
			else if(LastChar == '<')
			{
				LastChar = is.get();
				if(LastChar == '=')
				{
					tmpop = op_xiaoyudengyu;
					LastChar = is.get();
				}
				else
					tmpop = op_xiaoyu;
			}
			else if(LastChar == '>')
			{
				LastChar = is.get();
				if(LastChar == '=')
				{
					tmpop = op_dayudengyu;
					LastChar = is.get();
				}
				else
					tmpop = op_dayu;
			}
			else if(LastChar == '|')
			{
				LastChar = is.get();
				if(LastChar == '|')
				{
					tmpop = op_or;
					LastChar = is.get();
				}
				else
					cout << "expected ||" << endl;
			}
			else if(LastChar == '&')
			{
				LastChar = is.get();
				if(LastChar == '&')
				{
					tmpop = op_and;
					LastChar = is.get();
				}
				else
					cout << "expected &&" << endl;
			}
			else if(LastChar == '=')
			{
				LastChar = is.get();
				if(LastChar == '=')
				{
					tmpop = op_equal;
					LastChar = is.get();
				}
				else
					tmpop = op_fuzhi;
			}
			else if(LastChar == '!')
			{
				LastChar = is.get();
				if(LastChar == '=')
				{
					tmpop = op_not_equal;
					LastChar = is.get();
				}
				else
					cout << "expected !=" << endl;
			}
			cout << "tok_op" << " " << tmpop << endl;
			return {tok_op, "", 0, tmpop};
		}
	}
	Lexer(istream& _is):is(_is){};
};