#ifndef __IR__
#define __IR__

#include "Types.h"


// Klasa koja modeluje promenljivu
class Variable
{
public:

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(VariableType type, std::string name, int pos) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign), m_interference(0) {}

	void setAssignment(Regs r)
	{
		m_assignment = r;
	}

	void setConst(std::string c)
	{
		m_const = c;
	}

	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	std::string m_const;
	int m_interference;		// sa koliko drugih promenljivih ima smetnju
};

// Lista promenljivih
typedef std::list<Variable*> Variables;




// Klasa koja modeluje funkcije
class Function
{
public:
	enum FunctionType
	{
		FUNC_TYPE
	};

	Function() : m_type(FUNC_TYPE), m_name("") {}
	Function(std::string name) : m_type(FUNC_TYPE), m_name(name) {}

	std::string getName()
	{
		return m_name;
	}

private:
	FunctionType m_type;
	std::string m_name;
};

// Lista funkcija
typedef std::list<Function*> Functions;		




// Klasa koja modeluje labele
class Label
{
public:
	enum LabelType
	{
		LABEL_TYPE
	};

	Label() : m_type(LABEL_TYPE), m_name("") {}
	Label(std::string name) : m_type(LABEL_TYPE), m_name(name) {}
	
	std::string getName()
	{
		return m_name;
	}

private:
	LabelType m_type;
	std::string m_name;
};

// Lista labela
typedef std::list<Label*> Labels;




// Klasa koja modeluje instrukcije
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src, std::string label) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_myLabel(label) {}

	void setConst(std::string c)
	{
		m_const = c;
	}
	std::string getConst()
	{
		return m_const;
	}
	std::string getLabel()
	{
		return m_myLabel;
	}
	void setBranchLabel(std::string s)
	{
		m_branchLabel = s;
	}

	int m_position;
	InstructionType m_type;
	std::string m_const;
	std::string m_myLabel;			// Kojoj labeli pripada instrukcija
	std::string m_branchLabel;		// Za skokove (labela na koju treba skočiti)
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
};

// Lista instrukcija
typedef std::list<Instruction*> Instructions;


// Struktura za graf smetnji
typedef struct
{
	Variables* variables;				// Lista svih registarskih promenljivih
	int** values;						// Vrednost u matrici							 
	int size;							// Dimenzija kvadratne matrice
} InterferenceGraph;

typedef std::stack<Variable*> Stack;

#endif
