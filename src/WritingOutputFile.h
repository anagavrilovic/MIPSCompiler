#pragma once

// Ana Gavrilović, RA65/2018

#include "InstructionList.h"
#include "IR.h"
#include "Types.h"
#include "Constants.h"

#include <fstream>

bool writeInFile(InstructionList& instrList);

// Ispisuje .globl sekciju
void writeFunctions(Functions& funcList, ofstream& file);

// Ispisuje .data sekciju
void writeMemVariables(Variables& memList, ofstream& file);

// Ispisuje .text sekciju
void writeInstructions(Instructions& instrList, ofstream& file);

void writeAdd(Instruction* instr, ostream& file);

void writeAddi(Instruction* instr, ostream& file);

void writeSub(Instruction* instr, ostream& file);

void writeLi(Instruction* instr, ostream& file);

void writeLa(Instruction* instr, ostream& file);

void writeLw(Instruction* instr, ostream& file);

void writeSw(Instruction* instr, ostream& file);

void writeBltz(Instruction* instr, ostream& file);

void writeB(Instruction* instr, ostream& file);

void writeNop(Instruction* instr, ostream& file);

void writeAnd(Instruction* instr, ostream& file);

void writeBne(Instruction* instr, ostream& file);

void writeSll(Instruction* instr, ostream& file);

void writeSrl(Instruction* instr, ostream& file);