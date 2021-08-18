// Ana Gavrilović, RA65/2018

#include <iostream>
#include <iomanip>

#include "LivenessAnalysis.h"

using namespace std;

LivenessAnalysis::LivenessAnalysis(InstructionList& instrList) 
	: instructionList(instrList)
{
}


void LivenessAnalysis::printInstructions()
{
	cout << endl << endl << "******************************************* INSTRUCTIONS *********************************************" << endl;
	for (Instructions::iterator it = instructionList.instrList.begin(); it != instructionList.instrList.end(); it++) {

		cout << "\tINSTRUCTION: " << (*it)->m_position;

		cout << endl << "\tSRC: ";
		for (Variables::iterator itVar = (*it)->m_src.begin(); itVar != (*it)->m_src.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "\tDST: ";
		for (Variables::iterator itVar = (*it)->m_dst.begin(); itVar != (*it)->m_dst.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "\tUSE: ";
		for (Variables::iterator itVar = (*it)->m_use.begin(); itVar != (*it)->m_use.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "\tDEF: ";
		for (Variables::iterator itVar = (*it)->m_def.begin(); itVar != (*it)->m_def.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "\tPRED: ";
		for (Instructions::iterator itVar = (*it)->m_pred.begin(); itVar != (*it)->m_pred.end(); itVar++)
		{
			cout << (*itVar)->m_position << " ";
		}

		cout << endl << "\tSUCC: ";
		for (Instructions::iterator itVar = (*it)->m_succ.begin(); itVar != (*it)->m_succ.end(); itVar++)
		{
			cout << (*itVar)->m_position << " ";
		}

		cout << endl << "\tIN: ";
		for (Variables::iterator itVar = (*it)->m_in.begin(); itVar != (*it)->m_in.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "\tOUT: ";
		for (Variables::iterator itVar = (*it)->m_out.begin(); itVar != (*it)->m_out.end(); itVar++)
		{
			cout << (*itVar)->m_name << " ";
		}

		cout << endl << "-----------------------------------------------------" << endl;
	}

	cout << endl << "******************************************************************************************************" << endl << endl;
}

void LivenessAnalysis::Do()
{
	makeFlowControlGraph();
	//cout << endl << endl << "Flow control graph before liveness analysis:";
	//printInstructions();

	livenessAnalysis(instructionList.instrList);
	cout << "Flow control graph after liveness analysis:";
	printInstructions();
}

void LivenessAnalysis::makeFlowControlGraph()
{
	defineUse();
	defineDef();
	definePredSucc();
}

void LivenessAnalysis::defineUse()
{
	for (Instructions::iterator it = instructionList.instrList.begin(); it != instructionList.instrList.end(); it++)
	{
		for (Variables::iterator itVar = (*it)->m_src.begin(); itVar != (*it)->m_src.end(); itVar++)
		{
			if ((*itVar)->m_type == REG_VAR) {
				(*it)->m_use.push_back(*itVar);
			}
		}
	}
}

void LivenessAnalysis::defineDef()
{
	for (Instructions::iterator it = instructionList.instrList.begin(); it != instructionList.instrList.end(); it++)
	{
		for (Variables::iterator itVar = (*it)->m_dst.begin(); itVar != (*it)->m_dst.end(); itVar++)
		{
			if ((*itVar)->m_type == REG_VAR) {
				(*it)->m_def.push_back(*itVar);
			}
		}
	}
}

void LivenessAnalysis::definePredSucc() 
{
	Instructions::iterator it;
	Instructions::iterator it2;
	for (it = instructionList.instrList.begin(), it2 = ++instructionList.instrList.begin(); it2 != instructionList.instrList.end(); it++, it2++)
	{
		if (((*it)->m_type == I_B) || ((*it)->m_type == I_BLTZ) || ((*it)->m_type == I_BNE))
		{
			(*it)->m_succ.push_back(*it2);
			(*it2)->m_pred.push_back(*it);

			Instruction* instr = findInstruction((*it)->m_branchLabel);
			(*it)->m_succ.push_back(instr);
			instr->m_pred.push_back(*it);
		}
		else
		{
			(*it)->m_succ.push_back(*it2);
			(*it2)->m_pred.push_back(*it);
		}
	}

	// Kada je it na poslednjoj instrukciji ne postoji naredna tj. it2 pokazuje iza liste, tako da se samo definišu naslednici u slučaju da je skok
	if (((*it)->m_type == I_B) || ((*it)->m_type == I_BLTZ) || ((*it)->m_type == I_BNE))
	{
		Instruction* instr = findInstruction((*it)->m_branchLabel);
		(*it)->m_succ.push_back(instr);
		instr->m_pred.push_back(*it);
	}
}

void LivenessAnalysis::livenessAnalysis(Instructions instructions)
{
	list<Instruction*>::reverse_iterator it;											 
	bool end = false;																	

	while (!end) 
	{																		

		end = true;

		for (it = instructions.rbegin(); it != instructions.rend(); it++) 
		{				

			Variables inBefore = (*it)->m_in;											
			Variables outBefore = (*it)->m_out;											

			Variables outVar;															
			for (Instructions::iterator itSucc = (*it)->m_succ.begin(); itSucc != (*it)->m_succ.end(); itSucc++) 
			{
				for (Variables::iterator itIn = (*itSucc)->m_in.begin(); itIn != (*itSucc)->m_in.end(); itIn++) 
					outVar.push_back(*itIn);											
			}

			outVar.sort();
			outVar.unique();															

			(*it)->m_out = outVar;														



			Variables inVar;															
			for (Variables::iterator itUse = (*it)->m_use.begin(); itUse != (*it)->m_use.end(); itUse++) 
			{
				inVar.push_back(*itUse);												
			}

			for (Variables::iterator itOut = (*it)->m_out.begin(); itOut != (*it)->m_out.end(); itOut++) 
			{
				if (!variableExists(*itOut, (*it)->m_def))
					inVar.push_back(*itOut);											
			}

			inVar.sort();
			inVar.unique();

			(*it)->m_in = inVar;															


			if (inBefore != (*it)->m_in || outBefore != (*it)->m_out)						
				end = false;
		}
	}
}

bool LivenessAnalysis::variableExists(Variable* var, Variables varList)
{
	bool exists = false;

	for (Variables::iterator it = varList.begin(); it != varList.end(); it++)
	{
		if (*it == var)
		{
			exists = true;
			break;
		}
	}

	return exists;
}

Instruction* LivenessAnalysis::findInstruction(string label)
{
	for (auto it = instructionList.instrList.begin(); it != instructionList.instrList.end(); it++)
	{
		if ((*it)->m_myLabel == label) {
			return *it;
		}
	}
}


