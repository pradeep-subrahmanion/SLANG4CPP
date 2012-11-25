#include "context.h"
#include "symbol_table.h"

SymbolTable *Runtime_Context::get_symboltable()
{
	return st;
}

void Runtime_Context::set_symboltable(SymbolTable *_st)
{
	st= _st;
}

Compilation_Context::Compilation_Context()
{
   st = new SymbolTable();
}
SymbolTable * Compilation_Context::get_symboltable()
{
	return st;
}

void Compilation_Context::set_symboltable(SymbolTable *_st)
{
	st= _st;
}

void Compilation_Context::add_symbol(SymbolInfo *info)
{
   st->add(info);
}
