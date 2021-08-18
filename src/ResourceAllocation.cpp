// Ana Gavrilović, RA65/2018

#include "ResourceAllocation.h"
#include "IR.h"

#include <iostream>
#include <iomanip>
#include <list>

using namespace std;


void makeInterferenceGraph(InterferenceGraph* ig, Instructions& instrList, Variables& varList)
{
	ig->variables = &varList;
	ig->size = varList.size();
	ig->values = new int*[ig->size];

	for (int i = 0; i < ig->size; i++)									// for (int i = 0; i < ig->size; ++i)
	{																	//		delete[] ig->values[i];
		ig->values[i] = new int[ig->size];								// delete[] ig->values;
	}

	for (int i = 0; i < ig->size; i++)									// Na početku nema smetnji
	{
		for (int j = 0; j < ig->size; j++)
		{
			ig->values[i][j] = __EMPTY__;
		}
	}
	cout << endl << endl << "Interference graph - BEFORE: " << endl;
	printInterferenceGraph(ig);


	for (Instructions::iterator it = instrList.begin(); it != instrList.end(); it++)
	{
		for (Variables::iterator itDef = (*it)->m_def.begin(); itDef != (*it)->m_def.end(); itDef++)
		{
			for (Variables::iterator itOut = (*it)->m_out.begin(); itOut != (*it)->m_out.end(); itOut++)
			{
				if ((*itDef)->m_name != (*itOut)->m_name)
				{
					ig->values[(*itDef)->m_position][(*itOut)->m_position] = __INTERFERENCE__;
					ig->values[(*itOut)->m_position][(*itDef)->m_position] = __INTERFERENCE__;
				}
			}
		}
	}

	int num;
	for (int i = 0; i < ig->size; i++)
	{
		num = 0;
		for (int j = 0; j < ig->size; j++)
		{
			if (ig->values[i][j] == __INTERFERENCE__)
			{
				num++;
			}
		}
		Variable* var;
		if (findVariable(i, *ig->variables, var))
			var->m_interference = num;
	}

}

void printInterferenceGraph(InterferenceGraph* ig)
{
	Variables var = *ig->variables;

	for (Variables::iterator it = var.begin(); it != var.end(); it++)
	{
		cout << "\t" << (*it)->m_name;
	}
	cout << endl;

	Variables::iterator it = var.begin();
	for (int i = 0; i < ig->size; i++, it++)
	{
		cout << (*it)->m_name;
		for (int j = 0; j < ig->size; j++)
		{
			cout << "\t" << ig->values[i][j];
		}
		cout << endl;
	}
}

bool doSimplification(Stack& s, InterferenceGraph* ig, int regNumber)
{
	Variables varList = *ig->variables;
	int k;

	while (!varList.empty())
	{
		k = -1;
		for (auto it = varList.begin(); it != varList.end(); it++)
		{
			if ((*it)->m_interference > k && (*it)->m_interference < regNumber)
			{
				k = (*it)->m_interference;
			}
		}

		if (k == -1)
		{
			return false;
		}


		for (auto it = varList.begin(); it != varList.end(); it++)
		{
			if ((*it)->m_interference == k)
			{
				Variable* var = *it;
				s.push(var);

				for (int i = 0; i < ig->size; i++)
				{
					if (ig->values[var->m_position][i] == __INTERFERENCE__ || ig->values[i][var->m_position] == __INTERFERENCE__)
					{
						Variable* var2;
						if (findVariable(i, (*ig->variables), var2))
						{
							var2->m_interference -= 1;
						}
					}
				}

				it++;
				varList.remove(var);
				break;
			}
		}
	}

	return true;
}

bool findVariable(int position, Variables& varList, Variable*& var)
{
	bool exists = false;
	for (Variables::iterator it = varList.begin(); it != varList.end(); it++)
	{
		if ((*it)->m_position == position)
		{
			var = *it;
			exists = true;
			break;
		}
	}

	return exists;
}

bool doResourceAllocation(Stack* simplificationStack, InterferenceGraph* ig)
{
	vector<vector<int>> vecPos(__REG_NUMBER__);												// pamćenje pozicija promenljivih kojima su dodeljeni registri
	bool ok;																				// fleg za proveru za matricu smetnji


	while (!simplificationStack->empty()) {
																							// prolazak kroz sve moguće boje(registre) i traženje onog koji
		for (int i = 0; i <= __REG_NUMBER__; i++) {											// je moguće dodeliti	
			ok = true;
			for (auto it = vecPos[i].begin(); it != vecPos[i].end(); it++) {				// provera da li postoji smetnja između trenutne promenljive i 
				if (ig->values[*it][simplificationStack->top()->m_position] == __INTERFERENCE__) {// svake druge kojoj je već dodeljen taj registar
					ok = false;
				}
			}

			if (ok) {
				simplificationStack->top()->m_assignment = (Regs)i;							// dodela registra
				vecPos[i].push_back(simplificationStack->top()->m_position);				// pamćenje pozicija
				simplificationStack->pop();
				break;																		// registar dodeljen, izlazi se iz for petlje
			}
		}

		if (!ok) {																			// ako se for petlja završila i ok je i dalje na false to
			return false;																	// znači da nije pronađen registar koji može da se dodeli
		}
	}

	return true;
}

void printVariables(Variables& varList)
{
	cout << "****************************************** VARIABLES ***********************************************" << endl;
	for (auto it = varList.begin(); it != varList.end(); it++)
	{
		cout << (*it)->m_name << "\t" << (*it)->m_assignment << endl;
	}
	cout << "****************************************************************************************************";
	cout << endl << endl;
}

void deleteInterferenceGraph(InterferenceGraph* ig)
{
	for (int i = 0; i < ig->size; i++)
	{
		delete[] ig->values[i];
	}

	delete[] ig->values;
}