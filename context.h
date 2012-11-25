#include "symbol_table.h"
#include "common.h"

#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

class Runtime_Context
{
	SymbolTable *st;

public:
	Runtime_Context();
	SymbolTable *get_symboltable();
	void set_symboltable(SymbolTable *_st);
};

class Compilation_Context
{
	SymbolTable *st;

public:
	Compilation_Context();
	SymbolTable *get_symboltable();
	void set_symboltable(SymbolTable *_st);
   void add_symbol(SymbolInfo *info);
};

#endif
