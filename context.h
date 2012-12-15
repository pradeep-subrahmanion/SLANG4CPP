#ifndef CONTEXT_INCLUDED
#define CONTEXT_INCLUDED

#include "ast.h"
#include "common.h"
#include "codegen.h"
#include "symbol_table.h"

class Tmodule;

using namespace llvm;

class Runtime_Context {
	SymbolInfoTable *st;
	Tmodule *prog;

public:
	Runtime_Context();
	Runtime_Context(Tmodule *mod);
	Tmodule *get_program();
	SymbolInfoTable *get_symboltable();
	void set_symboltable(SymbolInfoTable *_st);
	void add_symbol(SymbolInfo *info);
	void assign_symbol(Variable *var, SymbolInfo *value);
	SymbolInfo *get_symbol(string name);
};

class Compilation_Context {
	SymbolInfoTable *st;

public:
	Compilation_Context();
	SymbolInfoTable *get_symboltable();
	void set_symboltable(SymbolInfoTable *_st);
	void add_symbol(SymbolInfo *info);
	void assign_symbol(Variable *var, SymbolInfo *value);
	SymbolInfo *get_symbol(string name);
};

class Execution_Context {
	AllocaInstTable *st;
   map<string,Function *> function_table;
   Procedure *current_proc;
public:
	Execution_Context();
	AllocaInstTable *get_symboltable();
	void set_symboltable(AllocaInstTable *_st);
	void add_symbol(string key, AllocaInst *info);
	void assign_symbol(Variable *var, AllocaInst *value);
	AllocaInst *get_symbol(string name);
   void add_procedure(string name, Function *func);
   Function *get_procedure(string name);
   void set_current_procedure(Procedure *_proc);
   Procedure *current_procedure();
   void update_return_value(Value * val);
};

#endif

