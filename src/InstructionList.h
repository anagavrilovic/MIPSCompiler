#pragma once

// Ana Gavrilović, RA65/2018
// semantička analiza 

#include "LexicalAnalysis.h"
#include "Token.h"
#include "Constants.h"
#include "IR.h"

#include <list>

using namespace std;


class InstructionList
{
public:

	// konstruktor
	InstructionList(LexicalAnalysis& lex);

	// Obavlja semantičku analizu i formiranje liste instrukcija
	bool Do();

	Instructions instrList;								// Lista pokazivača na instrukcije koja se kreira metodom Do()

	Variables memList;									// Lista pokazavača na memorijske promenljive
	Variables regList;									// Lista pokazivača na registarske promenljive		
	Functions funcListFirst;							// Lista funkcija koje postoje u kodu
	Labels labelListFirst;								// Lista labela koje postoje u kodu

	Functions funcListSecond;							// Lista funkcija na koje treba skociti	(za proveru semantičke ispravnosti)						 
	Labels labelListSecond;								// Lista labela na koje treba skočiti (za proveru semantičke ispravnosti)

	Instructions& getInstructionList();

	~InstructionList();

private:

	int posMem;
	int instrPos;
	int posReg;
	string myLabel;										// Poslednja dodata labela/instrukcija (da bi se za svaku instrukciju zapamtilo u kojoj je labeli)

	LexicalAnalysis& lexicalAnalysis;

	// Iterator na listu tokena koja predstavlja izlaz leksičke analize
	TokenList::iterator tokenIterator;

	// Trenutni token
	Token currentToken;

	// Indikator greške
	bool errorFound;



	// Obavlja proveru za memorijske promenljive
	void memAnalysis();

	// Obavlja proveru za registarske promenljive
	void regAnalysis();

	// Obavlja proveru za funkcije
	void funcAnalysis();

	// Obavlja proveru za labele
	void labelAnalysis();

	// add
	void addAnalysis();

	// addi
	void addiAnalysis();

	// sub
	void subAnalysis();

	// la
	void laAnalysis();

	// lw
	void lwAnalysis();

	// li
	void liAnalysis();

	// sw
	void swAnalysis();

	// b
	void bAnalysis();

	// bltz
	void bltzAnalysis();

	// nop
	void nopAnalysis();

	// and
	void andAnalysis();

	// bne
	void bneAnalysis();

	// sll
	void sllAnalysis();

	// srl
	void srlAnalysis();



	// Ispisuje poruku o grešci
	void printSemanticErrorReg(Token token, int num);

	// Ispisuje poruku o grešci
	void printSemanticErrorMem(Token token, int num);

	// Ispisuje poruku o grešci
	void printSemanticErrorFunc0(Token token);

	// Ispisuje poruku o grešci
	void printSemanticErrorFunc1(string function);

	// Ispisuje poruku o grešci
	void printSemanticErrorLabel0(Token token);

	// Ispisuje poruku o grešci
	void printSemanticErrorLabel1(string label);

	// Ispisuje informacije o tokenu
	void printTokenInfo(Token token);

	// Vraća sledeći token iz liste tokena
	Token getNextToken();

	// Proverava koji je sledeći token iz liste tokena (korisno kada ispitujemo da li je neki id zapravo labela)
	Token checkNextToken();

	// Metoda koja prolazi kroz listu registarskih promenljivih i vraća pokazivač na željenu registarsku promeljivu
	bool findVariableReg(string value, Variable*& var);

	// Metoda koja prolazi kroz listu memorijskih promenljivih i vraća pokazivač na željenu memorijsku promeljivu
	bool findVariableMem(string value, Variable*& var);

	// Metoda koja proverava da li medju labelama na koje treba skočiti ima nepostojećih
	bool labelExists(Label& label);

	// Metoda koja proverava da li medju funkcijama na koje treba skočiti ima nepostojećih
	bool functionExists(Function& func);
};