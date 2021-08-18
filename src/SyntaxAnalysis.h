#pragma once

// Ana Gavrilović, RA65/2018

#include "LexicalAnalysis.h"
#include "Token.h"
#include "Constants.h"

class SyntaxAnalysis
{
public:

	// konstruktor
	SyntaxAnalysis(LexicalAnalysis& lex);

	// obavlja sintaksnu analizu
	bool Do();

private:

	LexicalAnalysis& lexicalAnalysis;

	// Iterator na listu tokena koja predstavlja izlaz leksičke analize
	TokenList::iterator tokenIterator;

	// Trenutni token
	Token currentToken;
	
	// Indikator greške
	bool errorFound;


	
	// Ispisuje poruku o grešci
	void printSyntaxError(Token token);

	// Ispisuje informacije o tokenu
	void printTokenInfo(Token token);

	// Metoda koja služi za proveru sintaksne ispravnosti trenutnog tokena
	void eat(TokenType t);

	// Vraća sledeći token iz liste tokena
	Token getNextToken();

	// Neterminalni simboli: Q, S, L, E
	void Q();

	void S();

	void L();

	void E();

};
