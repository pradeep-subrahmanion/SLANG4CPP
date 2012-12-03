
#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED


#include "ast.h"
#include "common.h"
#include "codegen.h"
#include "symbol_table.h"


using namespace llvm;

class Runtime_Context
{
	SymbolInfoTable *st;
  
public:
	Runtime_Context();
	SymbolInfoTable *get_symboltable();
	void set_symboltable(SymbolInfoTable *_st);
  void add_symbol(SymbolInfo *info);
  void assign_symbol(Variable *var, SymbolInfo *value);
  SymbolInfo *get_symbol(string name);
};

class Compilation_Context
{
	SymbolInfoTable *st;
  
public:
  Compilation_Context();
	SymbolInfoTable *get_symboltable();
	void set_symboltable(SymbolInfoTable *_st);
  void add_symbol(SymbolInfo *info);
  void assign_symbol(Variable *var, SymbolInfo *value);
  SymbolInfo *get_symbol(string name);
};

class Execution_Context
{
	AllocaInstTable *st;
  
public:
	Execution_Context();
	AllocaInstTable *get_symboltable();
	void set_symboltable(AllocaInstTable *_st);
  void add_symbol(AllocaInst *info);
  void assign_symbol(Variable *var, AllocaInst * *value);
  AllocaInst *get_symbol(string name);
};

#endif
