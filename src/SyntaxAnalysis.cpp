// Ana Gavrilović, RA65/2018

#include <iostream>
#include <iomanip>

#include "Constants.h"
#include "SyntaxAnalysis.h"
#include "Token.h"

using namespace std;



SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), tokenIterator(lexicalAnalysis.getTokenList().begin()), errorFound(false)
{
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	Q();

	return !errorFound;
}

void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected." << endl;
}

void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}

void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			printTokenInfo(currentToken);

			if (currentToken.getType() != T_END_OF_FILE)
			{
				currentToken = getNextToken();
			}
		}
		else
		{
			printSyntaxError(currentToken);

			errorFound = true;
		}
	}
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

void SyntaxAnalysis::Q()
{
	S();
	eat(T_SEMI_COL);
	L();
}

void SyntaxAnalysis::S()
{
	switch (currentToken.getType())
	{
	case T_MEM:
		eat(T_MEM);
		eat(T_M_ID);
		eat(T_NUM);
		break;
	case T_REG:
		eat(T_REG);
		eat(T_R_ID);
		break;
	case T_FUNC:
		eat(T_FUNC);
		eat(T_ID);
		break;
	case T_ID:
		eat(T_ID);
		eat(T_COL);
		E();
		break;
	default: 
		E();
		break;
	}
}

void SyntaxAnalysis::L()
{
	if (currentToken.getType() == T_END_OF_FILE)
	{
		eat(T_END_OF_FILE);
	}
	else
	{
		Q();
	}
}

void SyntaxAnalysis::E()
{
	switch (currentToken.getType())
	{
	case T_ADD:
		eat(T_ADD);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;
	case T_ADDI:
		eat(T_ADDI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	case T_SUB:
		eat(T_SUB);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;
	case T_LA:
		eat(T_LA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_M_ID);
		break;
	case T_LW:
		eat(T_LW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;
	case T_LI:
		eat(T_LI);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	case T_SW:
		eat(T_SW);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		eat(T_L_PARENT);
		eat(T_R_ID);
		eat(T_R_PARENT);
		break;
	case T_B:
		eat(T_B);
		eat(T_ID);
		break;
	case T_BLTZ:
		eat(T_BLTZ);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
		break;
	case T_NOP:
		eat(T_NOP);
		break;
	case T_AND:
		eat(T_AND);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		break;
	case T_BNE:
		eat(T_BNE);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_ID);
		break;
	case T_SLL:
		eat(T_SLL);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	default:
		eat(T_SRL);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_R_ID);
		eat(T_COMMA);
		eat(T_NUM);
		break;
	}
}

