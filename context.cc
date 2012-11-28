#include "context.h"
#include "symbol_table.h"

// Runtime Context

Runtime_Context::Runtime_Context()
{
   st= new SymbolTable();
};

SymbolTable *Runtime_Context::get_symboltable()
{
	return st;
}

void Runtime_Context::set_symboltable(SymbolTable *_st)
{
	st= _st;
}
void Runtime_Context::assign_symbol(Variable *var, SymbolInfo *value)  
{
   st->assign(var,value);
}

void Runtime_Context::add_symbol(SymbolInfo *info)
{
   st->add(info);
}

SymbolInfo * Runtime_Context::get_symbol(string name) 
{
   return st->get(name);
}

// Compilation Context

Compilation_Context::Compilation_Context()
{
   st = new SymbolTable();
}
SymbolTable * Compilation_Context::get_symboltable()
{
	return st;
}

void Compilation_Context::assign_symbol(Variable *var, SymbolInfo *value)  
{
   st->assign(var,value);
}

void Compilation_Context::set_symboltable(SymbolTable *_st)
{
	st= _st;
}

void Compilation_Context::add_symbol(SymbolInfo *info)
{
   st->add(info);
}

SymbolInfo * Compilation_Context::get_symbol(string name) 
{
   return st->get(name);
}
