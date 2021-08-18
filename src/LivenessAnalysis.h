#pragma once

// Ana Gavrilović, RA65/2018
// Analiza životnog veka

#include "Constants.h"
#include "InstructionList.h"
#include "IR.h"
#include "Types.h"

#include <list>

using namespace std;

class LivenessAnalysis {
public:

	// Konstruktor
	LivenessAnalysis(InstructionList& instr);

	// Poziva metode za pravljenje grafa toka upravljanja i analize životnog veka
	void Do();
	
	InstructionList& instructionList;

private:

	// Funkcija koja pravi graf toka upravljanja
	void makeFlowControlGraph();

	// Za definisanje liste USE promenljivih
	void defineUse();

	// Za definisanje liste DEF promenljivih
	void defineDef();

	// Definiše listu prethodnika i sledbenika
	void definePredSucc();

	// Obavlja analizu životnog veka
	void livenessAnalysis(Instructions instructions);

	// Proverava da li postoji određena promenljiva u listi promenljivih
	bool variableExists(Variable* var, Variables varList);

	// Vraća pokayivač na prvu instrukciju posle labele sa prosleđenim nazivom
	Instruction* findInstruction(string label);

	// Ispisuje use, def, pred, succ, in i out za svaku instrukciju
	void printInstructions();

};