#pragma once

// Ana Gavrilović, RA65/2018

#include "Types.h"
#include "IR.h"
#include "Constants.h"
#include <stack>


// Za pravljenje grafa smetnji
void makeInterferenceGraph(InterferenceGraph* ig, Instructions& instrList, Variables& varList);

// Štampa graf smetnji
void printInterferenceGraph(InterferenceGraph* ig);

// Uprošćavanje grafa smetnji
bool doSimplification(Stack& s, InterferenceGraph* ig, int regNumber);

// Pronalazi promenljivu na određenoj poziciji
bool findVariable(int position, Variables& varList, Variable*& var);

// Funkcija za dodelu resursa
bool doResourceAllocation(Stack* simplificationStack, InterferenceGraph* ig);

// Štampa listu promenljivih (naziv + broj registra koji joj je dodeljen
void printVariables(Variables& varList);

// Brisanje dinamički zauzete memorije
void deleteInterferenceGraph(InterferenceGraph* ig);