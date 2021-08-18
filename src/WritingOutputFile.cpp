// Ana Gavrilović, RA65/2018

#include "WritingOutputFile.h"

using namespace std;

bool writeInFile(InstructionList& instr)
{
	ofstream file;
	file.open("izlazna.s");

	if (!file.is_open()) {
		return false;
	}

	writeFunctions(instr.funcListFirst, file);
	writeMemVariables(instr.memList, file);
	writeInstructions(instr.instrList, file);

	file.close();
	return true;
}

void writeFunctions(Functions& funcList, ofstream& file)
{
	for (auto it = funcList.begin(); it != funcList.end(); it++)
	{
		file << ".globl " << (*it)->getName() << endl;
	}

	file << endl;
}

void writeMemVariables(Variables& memList, ofstream& file)
{
	file << ".data" << endl;

	for (auto it = memList.begin(); it != memList.end(); it++)
	{
		file << (*it)->m_name << ":\t\t.word " << (*it)->m_const << endl;
	}
	file << endl;
}

void writeInstructions(Instructions& instrList, ofstream& file)
{
	file << ".text" << endl;

	string currentLabel = " ";

	for (auto it = instrList.begin(); it != instrList.end(); it++)
	{
		if ((*it)->m_myLabel != currentLabel)
		{
			currentLabel = (*it)->m_myLabel;
			file << currentLabel << ":" << endl;
		}

		switch ((*it)->m_type)
		{
		case I_ADD:
			writeAdd(*it, file);
			break;
		case I_ADDI:
			writeAddi(*it, file);
			break;
		case I_SUB:
			writeSub(*it, file);
			break;
		case I_LA:
			writeLa(*it, file);
			break;
		case I_LI:
			writeLi(*it, file);
			break;
		case I_LW:
			writeLw(*it, file);
			break;
		case I_SW:
			writeSw(*it, file);
			break;
		case I_BLTZ:
			writeBltz(*it, file);
			break;
		case I_B:
			writeB(*it, file);
			break;
		case I_NOP:
			writeNop(*it, file);
			break;
		case I_AND:
			writeAnd(*it, file);
			break;
		case I_BNE:
			writeBne(*it, file);
			break;
		case I_SLL:
			writeSll(*it, file);
			break;
		case I_SRL:
			writeSrl(*it, file);
			break;
		}
	}

	file << endl;
}

void writeAdd(Instruction* instr, ostream& file)
{
	Variables::iterator it = instr->m_src.begin();
	
	file << "\tadd\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*it)->m_assignment << ", $t";
	it++;
	file << (*it)->m_assignment << endl;
	
}

void writeAddi(Instruction* instr, ostream& file)
{
	file << "\taddi\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*instr->m_src.begin())->m_assignment << ", " << instr->m_const << endl;
}

void writeSub(Instruction* instr, ostream& file)
{
	Variables::iterator it = instr->m_src.begin();

	file << "\tsub\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*it)->m_assignment << ", $t";
	it++;
	file << (*it)->m_assignment << endl;
}

void writeLa(Instruction* instr, ostream& file)
{
	file << "\tla\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", " << (*instr->m_src.begin())->m_name << endl;
}

void writeLi(Instruction* instr, ostream& file)
{
	file << "\tli\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", " << instr->m_const << endl;
}

void writeLw(Instruction* instr, ostream& file)
{
	file << "\tlw\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", " << instr->m_const << "($t" << (*instr->m_src.begin())->m_assignment;
	file << ")" << endl;

}

void writeSw(Instruction* instr, ostream& file)
{
	Variables::iterator it = instr->m_src.begin();
	file << "\tsw\t\t$t" << (*it)->m_assignment << ", " << instr->m_const << "($t";
	it++;
	file << (*it)->m_assignment << ")" << endl;
}

void writeBltz(Instruction* instr, ostream& file)
{
	file << "\tbltz\t$t" << (*instr->m_src.begin())->m_assignment << ", " << instr->m_branchLabel << endl;
}

void writeB(Instruction* instr, ostream& file)
{
	file << "\tb\t\t" << instr->m_branchLabel << endl;
}

void writeNop(Instruction* instr, ostream& file)
{
	file << "\tnop" << endl;
}

void writeAnd(Instruction* instr, ostream& file)
{
	Variables::iterator it = instr->m_src.begin();

	file << "\tand\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*it)->m_assignment << ", $t";
	it++;
	file << (*it)->m_assignment << endl;
}

void writeBne(Instruction* instr, ostream& file)
{
	Variables::iterator it = instr->m_src.begin();
	file << "\tbne\t\t$t" << (*it)->m_assignment << ", $t";
	it++;
	file << (*it)->m_assignment << ", " << instr->m_branchLabel;
}

void writeSll(Instruction* instr, ostream& file)
{
	file << "\tsll\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*instr->m_src.begin())->m_assignment << ", " << instr->m_const << endl;
}

void writeSrl(Instruction* instr, ostream& file)
{
	file << "\tsrl\t\t$t" << (*instr->m_dst.begin())->m_assignment << ", $t" << (*instr->m_src.begin())->m_assignment << ", " << instr->m_const << endl;
}

