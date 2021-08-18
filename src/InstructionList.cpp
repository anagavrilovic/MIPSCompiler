// Ana Gavrilović, RA65/2018

#include <iostream>
#include <iomanip>

#include "Constants.h"
#include "InstructionList.h"
#include "Token.h"

using namespace std;


InstructionList::InstructionList(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), tokenIterator(lexicalAnalysis.getTokenList().begin()), errorFound(false), posMem(0), posReg(0), instrPos(1)
{
}

InstructionList::~InstructionList()
{
	for (auto it = regList.begin(); it != regList.end(); it++)
	{
		delete (*it);
	}

	for (auto it = memList.begin(); it != memList.end(); it++)
	{
		delete (*it);
	}

	for (auto it = labelListFirst.begin(); it != labelListFirst.end(); it++)
	{
		delete (*it);
	}

	for (auto it = labelListSecond.begin(); it != labelListSecond.end(); it++)
	{
		delete (*it);
	}

	for (auto it = funcListFirst.begin(); it != funcListFirst.end(); it++)
	{
		delete (*it);
	}

	for (auto it = funcListSecond.begin(); it != funcListSecond.end(); it++)
	{
		delete (*it);
	}

	for (auto it = instrList.begin(); it != instrList.end(); it++)
	{
		delete (*it);
	}
}

void InstructionList::printSemanticErrorReg(Token token, int num)
{
	if (num == 0)
	{
		cout << "\tSemantic error! Variable (reg): " << token.getValue() << " already defined." << endl;
	}
	else
	{
		cout << "\tSemantic error! Variable (reg): " << token.getValue() << " not defined." << endl;
	}
}

void InstructionList::printSemanticErrorMem(Token token, int num)
{
	if (num == 0)
	{
		cout << "\tSemantic error! Variable (mem): " << token.getValue() << " already defined." << endl;
	}
	else
	{
		cout << "\tSemantic error! Variable (mem): " << token.getValue() << " not defined." << endl;
	}
}

void InstructionList::printSemanticErrorFunc0(Token token)
{
		cout << "\tSemantic error! Function: " << token.getValue() << " already defined." << endl;
}

void InstructionList::printSemanticErrorFunc1(string function)
{
	cout << "\tSemantic error! Function: " << function << " not defined." << endl;
}

void InstructionList::printSemanticErrorLabel0(Token token)
{
		cout << "\tSemantic error! Label: " << token.getValue() << " already defined." << endl;
}

void InstructionList::printSemanticErrorLabel1(string label)
{
	cout << "\tSemantic error! Label: " << label << " not defined." << endl;
}

void InstructionList::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


bool InstructionList::Do()
{
	currentToken = getNextToken();

	while (currentToken.getType() != T_END_OF_FILE && errorFound == false)
	{
		switch (currentToken.getType())
		{
		case T_MEM:
			memAnalysis();
			break;
		case T_REG:
			regAnalysis();
			break;
		case T_FUNC:
			funcAnalysis();
			break;
		case T_ID:
			labelAnalysis();
			break;
		case T_ADD:
			addAnalysis();
			break;
		case T_ADDI:
			addiAnalysis();
			break;
		case T_SUB:
			subAnalysis();
			break;
		case T_LA:
			laAnalysis();
			break;
		case T_LW:
			lwAnalysis();
			break;
		case T_LI:
			liAnalysis();
			break;
		case T_SW:
			swAnalysis();
			break;
		case T_B:
			bAnalysis();
			break;
		case T_BLTZ:
			bltzAnalysis();
			break;
		case T_NOP:
			nopAnalysis();
			break;
		case T_AND:
			andAnalysis();
			break;
		case T_BNE:
			bneAnalysis();
			break;
		case T_SLL:
			sllAnalysis();
			break;
		case T_SRL:
			srlAnalysis();
			break;
		default:
			currentToken = getNextToken();
			break;
		}
	}

	Label l;
	if (!labelExists(l))
	{
		printSemanticErrorLabel1(l.getName());
		errorFound = true;
	}

	Function f;
	if (!functionExists(f))
	{
		printSemanticErrorFunc1(f.getName());
		errorFound = true;
	}
	

	return !errorFound;
}

void InstructionList::memAnalysis() 
{
	currentToken = getNextToken();
	bool exists = false;																// Nije još uvek definisana takva promenljiva

	for (Variables::iterator it = memList.begin(); it != memList.end(); it++)
	{
		if ((*it)->m_name == currentToken.getValue())
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		Variable* var = new Variable(MEM_VAR, currentToken.getValue(), posMem);
		posMem++;
		currentToken = getNextToken();
		(*var).setConst(currentToken.getValue());

		memList.push_back(var);
		currentToken = getNextToken();
	}
	else
	{
		errorFound = true;
		printSemanticErrorMem(currentToken, 0);
	}
}

void InstructionList::regAnalysis()
{
	currentToken = getNextToken();
	bool exists = false;															// Nije još uvek definisana takva promenljiva

	for (Variables::iterator it = regList.begin(); it != regList.end(); it++)
	{
		if ((*it)->m_name == currentToken.getValue())
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		Variable* var = new Variable(REG_VAR, currentToken.getValue(), posReg);
		posReg++;

		regList.push_back(var);
		currentToken = getNextToken();
	}
	else
	{
		errorFound = true;
		printSemanticErrorReg(currentToken, 0);
	}
}

void InstructionList::funcAnalysis()
{
	currentToken = getNextToken();

	bool exists = false;
	for (Functions::iterator it = funcListFirst.begin(); it != funcListFirst.end(); it++)
	{
		if ((*it)->getName() == currentToken.getValue())
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		Function* func = new Function(currentToken.getValue());

		funcListFirst.push_back(func);
		myLabel = func->getName();
		currentToken = getNextToken();
	}
	else
	{
		printSemanticErrorFunc0(currentToken);
		errorFound = true;
	}
}

void InstructionList::labelAnalysis()
{
	Token next = checkNextToken();

	if (next.getType() != T_COL) 
	{
		currentToken = getNextToken();
		return;
	}

	bool exists = false;
	for (Labels::iterator it = labelListFirst.begin(); it != labelListFirst.end(); it++)
	{
		if ((*it)->getName() == currentToken.getValue())
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		Label* label = new Label(currentToken.getValue());

		labelListFirst.push_back(label);
		myLabel = label->getName();
		currentToken = getNextToken();
	}
	else
	{
		printSemanticErrorLabel0(currentToken);
		errorFound = true;
	}
}

void InstructionList::addAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar1;
	if (findVariableReg(currentToken.getValue(), srcVar1))
		src.push_back(srcVar1);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar2;
	if (findVariableReg(currentToken.getValue(), srcVar2))
		src.push_back(srcVar2);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}

	Instruction* instr = new Instruction(instrPos, I_ADD, dst, src, myLabel);
	instrPos++;

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::addiAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar;
	if (findVariableReg(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Instruction* instr = new Instruction(instrPos, I_ADDI, dst, src, myLabel);
	instrPos++;

	instr->setConst(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::subAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar1;
	if (findVariableReg(currentToken.getValue(), srcVar1))
		src.push_back(srcVar1);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar2;
	if (findVariableReg(currentToken.getValue(), srcVar2))
		src.push_back(srcVar2);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	Instruction* instr = new Instruction(instrPos, I_SUB, dst, src, myLabel);
	instrPos++;

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::laAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}

	currentToken = getNextToken();
	currentToken = getNextToken();
	Variable* srcVar;

	if (findVariableMem(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorMem(currentToken, 1);
		errorFound = true;
		return;
	}

	Instruction* instr = new Instruction(instrPos, I_LA, dst, src, myLabel);
	instrPos++;

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::lwAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();
	
	string constant = currentToken.getValue();


	currentToken = getNextToken();
	currentToken = getNextToken();
	Variable* srcVar;

	if (findVariableReg(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	Instruction* instr = new Instruction(instrPos, I_LW, dst, src, myLabel);
	instrPos++;
	instr->setConst(constant);

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::liAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}

	currentToken = getNextToken();
	currentToken = getNextToken();

	Instruction* instr = new Instruction(instrPos, I_LI, dst, src, myLabel);
	instrPos++;
	instr->setConst(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::swAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* srcVar1;

	if (findVariableReg(currentToken.getValue(), srcVar1))
		src.push_back(srcVar1);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	string constant = currentToken.getValue();


	currentToken = getNextToken();
	currentToken = getNextToken();
	Variable* srcVar2;

	if (findVariableReg(currentToken.getValue(), srcVar2))
		src.push_back(srcVar2);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	Instruction* instr = new Instruction(instrPos, I_SW, dst, src, myLabel);
	instrPos++;
	instr->setConst(constant);

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::bAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Label* label = new Label(currentToken.getValue());
	labelListSecond.push_back(label);

	Instruction* instr = new Instruction(instrPos, I_B, dst, src, myLabel);
	instrPos++;
	instr->setBranchLabel(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::bltzAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* srcVar;

	if (findVariableReg(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}

	
	currentToken = getNextToken();
	currentToken = getNextToken();

	Label* label = new Label(currentToken.getValue());
	labelListSecond.push_back(label);

	Instruction* instr = new Instruction(instrPos, I_BLTZ, dst, src, myLabel);
	instrPos++;
	instr->setBranchLabel(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::nopAnalysis()
{
	Variables dst;								// Ostaju prazne jer nop nema src i dst
	Variables src;

	Instruction* instr = new Instruction(instrPos, I_NOP, dst, src, myLabel);
	instrPos++;

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::andAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar1;
	if (findVariableReg(currentToken.getValue(), srcVar1))
		src.push_back(srcVar1);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar2;
	if (findVariableReg(currentToken.getValue(), srcVar2))
		src.push_back(srcVar2);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	Instruction* instr = new Instruction(instrPos, I_AND, dst, src, myLabel);
	instrPos++;

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::bneAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* srcVar1;

	if (findVariableReg(currentToken.getValue(), srcVar1))
		src.push_back(srcVar1);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();
	Variable* srcVar2;

	if (findVariableReg(currentToken.getValue(), srcVar2))
		src.push_back(srcVar2);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Label* label = new Label(currentToken.getValue());
	labelListSecond.push_back(label);

	Instruction* instr = new Instruction(instrPos, I_BNE, dst, src, myLabel);
	instrPos++;
	instr->setBranchLabel(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::sllAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar;
	if (findVariableReg(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Instruction* instr = new Instruction(instrPos, I_SLL, dst, src, myLabel);
	instrPos++;

	instr->setConst(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}

void InstructionList::srlAnalysis()
{
	Variables dst;
	Variables src;

	currentToken = getNextToken();
	Variable* dstVar;

	if (findVariableReg(currentToken.getValue(), dstVar))
		dst.push_back(dstVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Variable* srcVar;
	if (findVariableReg(currentToken.getValue(), srcVar))
		src.push_back(srcVar);
	else
	{
		printSemanticErrorReg(currentToken, 1);
		errorFound = true;
		return;
	}


	currentToken = getNextToken();
	currentToken = getNextToken();

	Instruction* instr = new Instruction(instrPos, I_SRL, dst, src, myLabel);
	instrPos++;

	instr->setConst(currentToken.getValue());

	instrList.push_back(instr);

	currentToken = getNextToken();
}


Token InstructionList::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

Token InstructionList::checkNextToken()
{
	return *tokenIterator;
}

bool InstructionList::findVariableReg(string value, Variable*& var)
{
	bool exists = false;

	for (Variables::iterator it = regList.begin(); it != regList.end(); it++)
	{
		if ((*it)->m_name == value)
		{
			var = *it;
			exists = true;			// Ta promenljiva je definisana
			break;
		}
	}

	return exists;
}

bool InstructionList::findVariableMem(string value, Variable*& var)
{
	bool exists = false;

	for (Variables::iterator it = memList.begin(); it != memList.end(); it++)
	{
		if ((*it)->m_name == value)
		{
			var = *it;
			exists = true;			// Ta promenljiva je definisana
			break;
		}
	}

	return exists;
}

bool InstructionList::labelExists(Label& label)
{
	bool exists = true;					// Ako je lista labela na koje treba skočiti prazna, sve je u redu, nema semantičke greške

	for (list<Label*>::iterator it = labelListSecond.begin(); it != labelListSecond.end(); it++)
	{
		label = **it;
		exists = false;

		for (list<Label*>::iterator it2 = labelListFirst.begin(); it2 != labelListFirst.end(); it2++)
		{
			if (label.getName() == (*it2)->getName())
			{
				exists = true;				// Postoji
				break;
			}
		}

		for (list<Function*>::iterator it2 = funcListFirst.begin(); it2 != funcListFirst.end(); it2++)
		{
			if (label.getName() == (*it2)->getName())
			{
				exists = true;				// Postoji
				break;
			}
		}

		if (!exists)
			break;							// Uvrđeno da labela na koju treba skočiti ne postoji
	}

	return exists;
}

bool InstructionList::functionExists(Function& func)
{
	bool exists = true;					// Ako je lista labela na koje treba skočiti prazna, sve je u redu, nema semantičke greške

	for (list<Function*>::iterator it = funcListSecond.begin(); it != funcListSecond.end(); it++)
	{
		func = **it;
		exists = false;

		for (list<Label*>::iterator it2 = labelListFirst.begin(); it2 != labelListFirst.end(); it2++)
		{
			if (func.getName() == (*it2)->getName())
			{
				exists = true;				// Postoji
				break;
			}
		}

		for (list<Function*>::iterator it2 = funcListFirst.begin(); it2 != funcListFirst.end(); it2++)
		{
			if (func.getName() == (*it2)->getName())
			{
				exists = true;				// Postoji
				break;
			}
		}

		if (!exists)
			break;							// Uvrđeno da labela na koju treba skočiti ne postoji
	}

	return exists;
}

Instructions& InstructionList::getInstructionList()
{
	return instrList;
}
