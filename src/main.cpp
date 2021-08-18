// Ana Gavrilović, RA65/2018
// Datum predaje: 14. 6. 2020.

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "InstructionList.h"
#include "LivenessAnalysis.h"
#include "ResourceAllocation.h"
#include "WritingOutputFile.h"

using namespace std;


int main()
{
	try
	{
		//***************************************************** LEKSIČKA ANALIZA *****************************************************

		std::string fileName = ".\\..\\examples\\simple.mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}


		//**************************************************** SINTAKSNA ANALIZA *****************************************************

		SyntaxAnalysis syntax(lex);

		retVal = false;

		cout << endl << endl << "Syntax analysis: START" << endl;
		retVal = syntax.Do();

		if (retVal)
		{
			cout << "Syntax analysis finished successfully!" << endl << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}


		//********************************************** PRAVLJENJE LISTE INSTRUKCIJA ***********************************************

		InstructionList instr(lex);

		retVal = false;

		cout << endl << endl << "Semantic analysis: START" << endl;
		retVal = instr.Do();

		if (retVal)
		{
			cout << "Semantic analysis finished successfully!" << endl;
			cout << "Instruction list made successfully!" << endl << endl;
		}
		else
		{
			throw runtime_error("\nException! Semantic analysis failed!\n");
		}


		//************************************************** ANALIZA ŽIVOTNOG VEKA ***************************************************

		LivenessAnalysis livenessAnalysis(instr);

		cout << endl << endl << "LivenessAnalysis: START" << endl;
		livenessAnalysis.Do();
		cout << "Liveness analysis finished successfully!" << endl;


		//****************************************************** DODELA RESURSA ******************************************************

		InterferenceGraph* ig = new InterferenceGraph;
		makeInterferenceGraph(ig, instr.instrList, instr.regList);
		cout << endl << endl << "Interference graph - AFTER: " << endl;
		printInterferenceGraph(ig);

		Stack simplificationStack;

		if(!doSimplification(simplificationStack, ig, __REG_NUMBER__))
		{
			throw runtime_error("\nException! Spill detected!\n");
		}
		else
		{
			cout << endl << "Stack simplified!" << endl << endl;

			cout << "Before resource allocation: " << endl;
			printVariables(instr.regList);

			if (doResourceAllocation(&simplificationStack, ig))
			{
				cout << endl << "Resource Allocation ended successfully!" << endl << endl;
				cout << "After resource allocation: " << endl;
				printVariables(instr.regList);
			}
			else
			{
				throw runtime_error("\nException! Allocation failed!\n");
			}
		}

		if (writeInFile(instr))
		{
			cout << "File written successfully!" << endl << endl << endl;
		}
		else
		{
			throw runtime_error("\nException! Could not open output file!\n");
		}



		deleteInterferenceGraph(ig);
		delete ig;
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
